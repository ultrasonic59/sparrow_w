#include "usb_com.h"

#include "usb_ehci.h"

#include "xil_printf.h"
#include "xtime_l.h"

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
static uint8_t g_ep_in;
static uint16_t g_mps_in;
static int g_tog_in;
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

static void remember_in(const uint8_t* p, uint8_t iface, uint8_t want_iface, int same_iface_only, int* found)
{
    uint16_t mps;
    if (*found || p[0] < 7 || p[1] != 5 || (p[3] & 3u) != 2u || !(p[2] & 0x80u))
        return;
    if (same_iface_only && iface != want_iface)
        return;
    mps = (uint16_t)((p[4] | (p[5] << 8)) & 0x7ffu);
    g_ep_in = (uint8_t)(p[2] & 0x0fu);
    g_mps_in = mps ? mps : 64;
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
    int found_out = 0;
    int found_in = 0;
    g_ep_out = 0;
    g_mps_out = 0;
    g_ep_in = 0;
    g_mps_in = 0;
    g_iface = 0;
    /* Pass 1: bulk OUT (and note its interface). */
    p = cfg;
    iface = 0;
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
            remember_out(p, iface, &found_out);
        }
        p += len;
    }
    /* Pass 2: bulk IN on the same interface as OUT (CDC data iface). */
    if (found_out) {
        p = cfg;
        iface = 0;
        while (p + 2 <= end) {
            uint8_t len = p[0];
            if (len < 2 || p + len > end)
                break;
            if (p[1] == 4 && len >= 9)
                iface = p[2];
            else
                remember_in(p, iface, g_iface, 1, &found_in);
            p += len;
        }
    }
    /* Pass 3: any bulk IN if same-iface search failed. */
    if (found_out && !found_in) {
        p = cfg;
        iface = 0;
        while (p + 2 <= end) {
            uint8_t len = p[0];
            if (len < 2 || p + len > end)
                break;
            if (p[1] == 4 && len >= 9)
                iface = p[2];
            else
                remember_in(p, iface, 0, 0, &found_in);
            p += len;
        }
    }
    if (have_comm)
        g_iface = comm;
    if (!found_out || !found_in)
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
    g_tog_in = 0;
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
    if (kind == KIND_FTDI) {
        if (ctrl(0x40, 0, 0, g_iface, 0, 0) != 0)
            return -1;
        /* Latency 1 ms so short Att replies are not held in the chip. */
        (void)ctrl(0x40, 9, 1, g_iface, 0, 0);
    }
    if (kind == KIND_PL2303)
        pl2303_init();
    if (apply_line(115200) != 0)
        return -1;
    g_baud = 115200;
    g_open = 1;
    com_log_info();
    return 0;
}

int com_is_open(void)
{
    return g_open ? 1 : 0;
}

void com_log_info(void)
{
    static const char* names[] = { "?", "CDC", "CH340", "CP210x", "FTDI", "PL2303" };
    const char* name = (g_kind >= 1 && g_kind <= 5) ? names[g_kind] : names[0];
    xil_printf("COM attach %s speed=%u hub=%u port=%u addr=%u iface=%u out=%u/%u in=%u/%u\r\n",
               name, (unsigned)g_speed, (unsigned)g_hub, (unsigned)g_port, (unsigned)g_addr,
               (unsigned)g_iface, (unsigned)g_ep_out, (unsigned)g_mps_out, (unsigned)g_ep_in,
               (unsigned)g_mps_in);
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

int com_read(uint8_t* data, uint16_t cap, uint16_t* actual, uint32_t timeout_ms)
{
    uint16_t n = 0;
    uint16_t nak_rounds = 0;
    XTime start;
    const XTime limit = (XTime)timeout_ms * (COUNTS_PER_SECOND / 1000u);
    if (actual)
        *actual = 0;
    if (!g_open || !data || cap == 0 || g_ep_in == 0)
        return -1;
    /* Do not purge/clear here: Att may already have replied into the adapter FIFO. */
    XTime_GetTime(&start);
    while (n < cap) {
        uint8_t chunk[64];
        uint16_t got = 0;
        uint16_t want = g_mps_in;
        uint32_t slice;
        int rc;
        uint16_t i;
        XTime now;
        XTime_GetTime(&now);
        if ((now - start) >= limit)
            break;
        slice = (uint32_t)(((limit - (now - start)) * 1000u) / COUNTS_PER_SECOND);
        if (slice == 0)
            slice = 1;
        if (slice > 100u)
            slice = 100u;
        if (want > sizeof(chunk))
            want = (uint16_t)sizeof(chunk);
        if (g_speed == 2)
            rc = ehci_bulk_in(g_addr, g_ep_in, g_mps_in, chunk, want, &got, &g_tog_in, slice);
        else
            rc = ehci_bulk_in_tt(g_addr, g_speed, g_hub, g_port, g_ep_in, g_mps_in, chunk, want, &got, &g_tog_in,
                                 slice);
        if (rc != 0) {
            ++nak_rounds;
            if (nak_rounds <= 3u || (nak_rounds % 5u) == 0u)
                xil_printf("COM in rc=%d got=%u\r\n", rc, (unsigned)got);
            if (rc == -1) {
                ctrl(0x02, 1, 0, (uint16_t)(0x80u | g_ep_in), 0, 0);
                g_tog_in = 0;
            }
            continue;
        }
        xil_printf("COM in ok got=%u:", (unsigned)got);
        for (i = 0; i < got && i < 16u; ++i)
            xil_printf(" %02x", (unsigned)chunk[i]);
        xil_printf("\r\n");
        if (got == 0)
            continue;
        {
            uint8_t* p = chunk;
            uint16_t len = got;
            if (g_kind == KIND_FTDI) {
                /* Normal FTDI bulk IN: 2 status bytes then payload.
                   TT short-packet paths may deliver payload only (got=1: '1').
                   ASCII '1' (0x31) and 'a' (0x61) also have low nibble 0x1 —
                   so "10" must not be stripped as a modem/line status header. */
                if (len >= 2u && (p[0] & 0x0fu) == 0x01u) {
                    const uint8_t s1 = p[1];
                    const int s1_att = (s1 == (uint8_t)'\r' || s1 == (uint8_t)'\n'
                                        || (s1 >= (uint8_t)'0' && s1 <= (uint8_t)'9')
                                        || (s1 >= (uint8_t)'a' && s1 <= (uint8_t)'f')
                                        || (s1 >= (uint8_t)'A' && s1 <= (uint8_t)'F'));
                    if (len > 2u || !s1_att) {
                        p += 2;
                        len = (uint16_t)(len - 2u);
                        if (len == 0)
                            continue;
                    }
                }
            }
            while (len && n < cap) {
                const uint8_t b = *p++;
                --len;
                /* USB-UART adapters sometimes prepend NUL / status padding. */
                if (b == 0)
                    continue;
                data[n++] = b;
                if (b == '\r') {
                    if (actual)
                        *actual = n;
                    return 0;
                }
            }
        }
    }
    if (actual)
        *actual = n;
    if (n == 0)
        xil_printf("COM in empty nak_rounds=%u\r\n", (unsigned)nak_rounds);
    return n > 0 ? 0 : -1;
}
