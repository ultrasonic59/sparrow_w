#include "usb_com.h"

#include "usb_ehci.h"
#include "xil_printf.h"

#include <string.h>

#define KIND_CDC 1
#define KIND_CH340 2
#define KIND_CP210X 3
#define KIND_FTDI 4
#define KIND_PL2303 5

static uint8_t g_open;
static uint8_t g_addr;
static uint8_t g_speed;
static uint8_t g_hub;
static uint8_t g_port;
static uint16_t g_mps0;
static uint8_t g_ep_out;
static uint16_t g_mps_out;
static int g_tog_out;
static uint8_t g_iface;
static uint8_t g_kind;
static uint32_t g_baud;

static int ctrl(uint8_t type, uint8_t req, uint16_t value, uint16_t index, void* data, uint16_t length)
{
    uint8_t setup[8];
    setup[0] = type;
    setup[1] = req;
    setup[2] = (uint8_t)value;
    setup[3] = (uint8_t)(value >> 8);
    setup[4] = (uint8_t)index;
    setup[5] = (uint8_t)(index >> 8);
    setup[6] = (uint8_t)length;
    setup[7] = (uint8_t)(length >> 8);
    if (g_speed == 2)
        return ehci_control(g_addr, setup, data, length);
    return ehci_control_tt(g_addr, g_speed, g_hub, g_port, g_mps0, setup, data, length);
}

static void remember_out(const uint8_t* p, uint8_t iface, int* found)
{
    uint16_t mps;
    if (*found || p[0] < 7 || p[1] != 5 || (p[3] & 3u) != 2u || (p[2] & 0x80u))
        return;
    mps = (uint16_t)((p[4] | (p[5] << 8)) & 0x7ffu);
    g_ep_out = (uint8_t)(p[2] & 0x0fu);
    g_mps_out = mps ? mps : 64;
    g_iface = iface;
    *found = 1;
}

static uint8_t detect(uint16_t vid, uint16_t pid, uint8_t device_class, const uint8_t* cfg, uint16_t cfg_len)
{
    const uint8_t* p = cfg;
    const uint8_t* end = cfg + cfg_len;
    uint8_t iface = 0;
    uint8_t comm = 0;
    int have_comm = 0;
    int cdc = 0;
    int found = 0;
    g_ep_out = 0;
    g_mps_out = 0;
    g_iface = 0;
    while (p + 2 <= end) {
        uint8_t len = p[0];
        if (len < 2 || p + len > end)
            break;
        if (p[1] == 4 && len >= 9) {
            iface = p[2];
            if (p[5] == 2) {
                cdc = 1;
                comm = iface;
                have_comm = 1;
            } else if (p[5] == 0x0a) {
                cdc = 1;
            }
        } else {
            remember_out(p, iface, &found);
        }
        p += len;
    }
    if (have_comm)
        g_iface = comm;
    if (!found)
        return 0;
    if (vid == 0x1a86 && (pid == 0x7523 || pid == 0x5523 || pid == 0x7522))
        return KIND_CH340;
    if (vid == 0x10c4 && (pid == 0xea60 || pid == 0xea61 || pid == 0xea70 || pid == 0xea71))
        return KIND_CP210X;
    if (vid == 0x0403 && (pid == 0x6001 || pid == 0x6010 || pid == 0x6011 || pid == 0x6014 || pid == 0x6015)) {
        /* FT2232 and later select the port with bInterfaceNumber + 1. */
        if (pid != 0x6001)
            g_iface = (uint8_t)(g_iface + 1u);
        return KIND_FTDI;
    }
    if (vid == 0x067b && pid == 0x2303)
        return KIND_PL2303;
    if (cdc || device_class == 2)
        return KIND_CDC;
    return 0;
}

static int cdc_line(uint32_t baud)
{
    uint8_t line[7];
    line[0] = (uint8_t)baud;
    line[1] = (uint8_t)(baud >> 8);
    line[2] = (uint8_t)(baud >> 16);
    line[3] = (uint8_t)(baud >> 24);
    line[4] = 0;
    line[5] = 0;
    line[6] = 8;
    if (ctrl(0x21, 0x20, 0, g_iface, line, sizeof(line)) != 0)
        return -1;
    return ctrl(0x21, 0x22, 3, g_iface, 0, 0);
}

static int ch340_line(uint32_t baud)
{
    uint32_t factor;
    uint8_t divisor = 3;
    uint16_t a;
    uint8_t b;
    if (baud == 0)
        return -1;
    factor = 1532620800u / baud;
    while (factor > 0xfff0u && divisor) {
        factor >>= 3;
        --divisor;
    }
    if (factor > 0xfff0u)
        return -1;
    factor = 0x10000u - factor;
    a = (uint16_t)((factor & 0xff00u) | divisor);
    b = (uint8_t)(factor & 0xffu);
    if (ctrl(0x40, 0x9a, 0x1312, a, 0, 0) != 0)
        return -1;
    if (ctrl(0x40, 0x9a, 0x0f2c, b, 0, 0) != 0)
        return -1;
    return ctrl(0x40, 0x9a, 0x2518, 0x00c3, 0, 0);
}

static int ch340_init(void)
{
    uint8_t ver[2];
    ctrl(0xc0, 0x5f, 0, 0, ver, sizeof(ver));
    if (ctrl(0x40, 0xa1, 0, 0, 0, 0) != 0)
        return -1;
    return ctrl(0x40, 0xa4, (uint16_t)~(uint8_t)0x60, 0, 0, 0);
}

static int cp210x_line(uint32_t baud)
{
    uint8_t rate[4];
    rate[0] = (uint8_t)baud;
    rate[1] = (uint8_t)(baud >> 8);
    rate[2] = (uint8_t)(baud >> 16);
    rate[3] = (uint8_t)(baud >> 24);
    if (ctrl(0x41, 0x1e, 0, g_iface, rate, sizeof(rate)) != 0)
        return -1;
    if (ctrl(0x41, 0x03, 0x0800, g_iface, 0, 0) != 0)
        return -1;
    return ctrl(0x41, 0x07, 0x0303, g_iface, 0, 0);
}

static uint16_t ftdi_divisor(uint32_t baud)
{
    static const uint8_t frac[8] = { 0, 3, 2, 4, 1, 5, 6, 7 };
    uint32_t denom;
    uint32_t divisor3;
    uint16_t divisor;
    if (baud < 50)
        baud = 50;
    denom = baud * 2u;
    divisor3 = (48000000u + denom / 2u) / denom;
    divisor = (uint16_t)(divisor3 >> 3);
    divisor |= (uint16_t)frac[divisor3 & 7u] << 14;
    if (divisor == 1)
        divisor = 0;
    return divisor;
}

static int ftdi_line(uint32_t baud)
{
    uint16_t divisor = ftdi_divisor(baud);
    uint16_t index = g_iface;
    if (ctrl(0x40, 3, divisor, index, 0, 0) != 0)
        return -1;
    if (ctrl(0x40, 4, 8, g_iface, 0, 0) != 0)
        return -1;
    return ctrl(0x40, 1, 0x0303, g_iface, 0, 0);
}

static int pl2303_init(void)
{
    uint8_t byte = 0;
    ctrl(0xc0, 1, 0x8484, 0, &byte, 1);
    ctrl(0x40, 1, 0x0404, 0, 0, 0);
    ctrl(0xc0, 1, 0x8484, 0, &byte, 1);
    ctrl(0xc0, 1, 0x8383, 0, &byte, 1);
    ctrl(0xc0, 1, 0x8484, 0, &byte, 1);
    ctrl(0x40, 1, 0x0404, 1, 0, 0);
    ctrl(0xc0, 1, 0x8484, 0, &byte, 1);
    ctrl(0xc0, 1, 0x8383, 0, &byte, 1);
    ctrl(0x40, 1, 0, 1, 0, 0);
    ctrl(0x40, 1, 1, 0, 0, 0);
    ctrl(0x40, 1, 2, 0x44, 0, 0);
    ctrl(0x40, 1, 8, 0, 0, 0);
    ctrl(0x40, 1, 9, 0, 0, 0);
    return 0;
}

static int apply_line(uint32_t baud)
{
    if (baud < 50 || baud > 3000000u)
        return -1;
    if (g_kind == KIND_CDC || g_kind == KIND_PL2303)
        return cdc_line(baud);
    if (g_kind == KIND_CH340)
        return ch340_line(baud);
    if (g_kind == KIND_CP210X)
        return cp210x_line(baud);
    if (g_kind == KIND_FTDI)
        return ftdi_line(baud);
    return -1;
}

static const char* kind_name(uint8_t kind)
{
    if (kind == KIND_CDC)
        return "cdc";
    if (kind == KIND_CH340)
        return "ch340";
    if (kind == KIND_CP210X)
        return "cp210x";
    if (kind == KIND_FTDI)
        return "ftdi";
    if (kind == KIND_PL2303)
        return "pl2303";
    return "?";
}

int com_attach(uint8_t addr, uint8_t speed, uint8_t hub, uint8_t port, uint16_t mps0, const uint8_t desc[18],
               const uint8_t* cfg, uint16_t cfg_len)
{
    uint16_t vid;
    uint16_t pid;
    uint8_t kind;
    if (g_open || !desc || !cfg || cfg_len < 9 || mps0 == 0)
        return g_open ? 0 : 1;
    vid = (uint16_t)(desc[8] | (desc[9] << 8));
    pid = (uint16_t)(desc[10] | (desc[11] << 8));
    g_addr = addr;
    g_speed = speed;
    g_hub = hub;
    g_port = port;
    g_mps0 = mps0;
    g_tog_out = 0;
    kind = detect(vid, pid, desc[4], cfg, cfg_len);
    if (kind == 0)
        return 1;
    g_kind = kind;
    if (ctrl(0x00, 9, cfg[5], 0, 0, 0) != 0)
        return -1;
    if (kind == KIND_CH340 && ch340_init() != 0)
        return -1;
    if (kind == KIND_CP210X && ctrl(0x41, 0x00, 1, g_iface, 0, 0) != 0)
        return -1;
    if (kind == KIND_FTDI && ctrl(0x40, 0, 0, g_iface, 0, 0) != 0)
        return -1;
    if (kind == KIND_PL2303)
        pl2303_init();
    if (apply_line(115200) != 0)
        return -1;
    g_baud = 115200;
    g_open = 1;
    xil_printf("usb-com %s ep %d\r\n", kind_name(kind), g_ep_out);
    return 0;
}

int com_write(uint32_t baud, const uint8_t* data, uint16_t length)
{
    if (!g_open)
        return -1;
    if (baud != 0 && baud != g_baud) {
        if (apply_line(baud) != 0)
            return -1;
        g_baud = baud;
    }
    if (length == 0)
        return 0;
    if (!data)
        return -1;
    if (g_speed == 2) {
        if (ehci_bulk_out(g_addr, g_ep_out, g_mps_out, data, length, &g_tog_out) != 0) {
            ctrl(0x02, 1, 0, g_ep_out, 0, 0);
            g_tog_out = 0;
            return -1;
        }
        return 0;
    }
    if (ehci_bulk_out_tt(g_addr, g_speed, g_hub, g_port, g_ep_out, g_mps_out, data, length, &g_tog_out) != 0) {
        ctrl(0x02, 1, 0, g_ep_out, 0, 0);
        g_tog_out = 0;
        return -1;
    }
    return 0;
}
