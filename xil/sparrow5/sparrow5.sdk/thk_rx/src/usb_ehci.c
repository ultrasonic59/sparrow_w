#include "usb_ehci.h"

#include "xil_cache.h"
#include "xil_io.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xtime_l.h"

#include <string.h>

#define USB_BASE XPAR_PS7_USB_0_BASEADDR
#define USB_CMD 0x140u
#define USB_STS 0x144u
#define USB_ASYNC 0x158u
#define USB_PORTSC 0x184u
#define USB_MODE 0x1A8u

#define CMD_RS (1u << 0)
#define CMD_RST (1u << 1)
#define CMD_ASE (1u << 5)
#define CMD_IAA (1u << 6)
#define STS_ASS (1u << 15)
#define PORT_CCS (1u << 0)
#define PORT_PE (1u << 2)
#define PORT_PR (1u << 8)
#define PORT_PP (1u << 12)
#define PORT_W1C ((1u << 1) | (1u << 3) | (1u << 5))

#define QTD_ACTIVE (1u << 7)
#define QTD_HALT (1u << 6)
#define QTD_XACT (1u << 3)
#define QTD_CERR (3u << 10)
#define QTD_DT (1u << 31)

typedef struct {
    volatile uint32_t next;
    volatile uint32_t alt;
    volatile uint32_t token;
    volatile uint32_t buf[5];
} qtd_t;

typedef struct {
    volatile uint32_t horiz;
    volatile uint32_t epchar;
    volatile uint32_t epcap;
    volatile uint32_t current;
    volatile uint32_t next;
    volatile uint32_t alt;
    volatile uint32_t token;
    volatile uint32_t buf[5];
    uint32_t pad[4];
} qh_t;

static qh_t g_qh __attribute__((aligned(64)));
static qtd_t g_qtd[3] __attribute__((aligned(64)));
static uint8_t g_setup[8] __attribute__((aligned(64)));
static uint8_t g_data[512] __attribute__((aligned(4096)));
static uint8_t g_bulk[8192] __attribute__((aligned(4096)));

static uint32_t usb_rd(uint32_t offset)
{
    return Xil_In32(USB_BASE + offset);
}

static void usb_wr(uint32_t offset, uint32_t value)
{
    Xil_Out32(USB_BASE + offset, value);
}

void ehci_wait_ms(unsigned ms)
{
    XTime start;
    XTime now;
    const XTime ticks = (XTime)ms * (COUNTS_PER_SECOND / 1000u);
    XTime_GetTime(&start);
    do {
        XTime_GetTime(&now);
    } while ((now - start) < ticks);
}

static uint32_t link_qh(const void* ptr)
{
    return ((uint32_t)ptr & ~0x1fu) | (1u << 1);
}

static uint32_t link_qtd(const void* ptr)
{
    return (uint32_t)ptr & ~0x1fu;
}

static void cache_flush(void* ptr, unsigned length)
{
    Xil_DCacheFlushRange((INTPTR)ptr, length);
}

static void cache_inval(void* ptr, unsigned length)
{
    Xil_DCacheInvalidateRange((INTPTR)ptr, length);
}

static void async_stop(void)
{
    unsigned i;
    usb_wr(USB_CMD, usb_rd(USB_CMD) & ~CMD_ASE);
    for (i = 0; i < 20 && (usb_rd(USB_STS) & STS_ASS); ++i)
        ehci_wait_ms(1);
}

static void async_start(void)
{
    unsigned i;
    usb_wr(USB_CMD, usb_rd(USB_CMD) | CMD_ASE);
    for (i = 0; i < 20 && (usb_rd(USB_STS) & STS_ASS) == 0; ++i)
        ehci_wait_ms(1);
}

static void program_qh(uint8_t addr, uint8_t ep, uint16_t mps, int control, const qtd_t* first, uint8_t speed,
                       uint8_t hub, uint8_t port)
{
    uint32_t epcap = 1u << 30;
    /* Bit 14 (DTC) makes a control transfer use the DATA0/DATA1 bit of each qTD.
       Bulk endpoints ignore that bit and take the toggle from the overlay token. */
    g_qh.epchar = (0xfu << 28) | ((uint32_t)(mps & 0x7ff) << 16) | (1u << 15) | (control ? ((1u << 27) | (1u << 14)) : 0u)
        | ((uint32_t)(speed & 3u) << 12) | ((uint32_t)(ep & 0xfu) << 8) | (addr & 0x7fu);
    if (speed != 2u)
        epcap |= ((uint32_t)(port & 0x7fu) << 23) | ((uint32_t)(hub & 0x7fu) << 16);
    g_qh.epcap = epcap;
    g_qh.current = 0;
    g_qh.next = link_qtd(first);
    g_qh.alt = 1u;
    g_qh.token = control ? 0u : (first->token & QTD_DT);
    memset((void*)g_qh.buf, 0, sizeof(g_qh.buf));
    cache_flush(&g_qh, sizeof(g_qh));
}

static void fill_buffers(qtd_t* td, const void* data)
{
    uint32_t addr = (uint32_t)data;
    unsigned i;
    td->buf[0] = addr;
    addr &= ~0xfffu;
    for (i = 1; i < 5; ++i)
        td->buf[i] = addr + i * 0x1000u;
}

static int wait_qtd(qtd_t* td, uint32_t timeout_ms, uint16_t requested, uint16_t* actual)
{
    XTime start;
    XTime now;
    const XTime limit = (XTime)timeout_ms * (COUNTS_PER_SECOND / 1000u);
    XTime_GetTime(&start);
    for (;;) {
        uint32_t token;
        cache_inval(td, sizeof(*td));
        token = td->token;
        if ((token & QTD_ACTIVE) == 0) {
            const uint32_t left = (token >> 16) & 0x7fffu;
            if (actual)
                *actual = left >= requested ? 0 : (uint16_t)(requested - left);
            if (token & (QTD_HALT | QTD_XACT))
                return -1;
            return 0;
        }
        XTime_GetTime(&now);
        if ((now - start) >= limit)
            return -2;
    }
}

static int run_chain(uint8_t addr, uint8_t ep, uint16_t mps, int control, int count, uint32_t timeout_ms,
                     uint16_t data_len, uint16_t* actual, uint8_t speed, uint8_t hub, uint8_t port)
{
    int i;
    int wait;
    cache_flush(g_qtd, sizeof(g_qtd));
    async_stop();
    program_qh(addr, ep, mps, control, &g_qtd[0], speed, hub, port);
    async_start();
    wait = wait_qtd(&g_qtd[count - 1], timeout_ms, data_len, actual);
    if (wait != 0) {
        uint32_t kept;
        async_stop();
        cache_inval(&g_qh, sizeof(g_qh));
        kept = g_qh.token & QTD_DT;
        g_qh.next = 1u;
        g_qh.token = kept | QTD_HALT;
        cache_flush(&g_qh, sizeof(g_qh));
        for (i = 0; i < count; ++i)
            g_qtd[i].token = QTD_HALT;
        cache_flush(g_qtd, sizeof(g_qtd));
        return wait;
    }
    for (i = 0; i < count; ++i) {
        cache_inval(&g_qtd[i], sizeof(g_qtd[i]));
        if (g_qtd[i].token & (QTD_HALT | QTD_XACT | QTD_ACTIVE))
            return -1;
    }
    return 0;
}

static void make_qtd(qtd_t* td, const qtd_t* next, uint32_t pid, uint32_t dt, const void* data, uint16_t length)
{
    memset(td, 0, sizeof(*td));
    td->next = next ? link_qtd(next) : 1u;
    td->alt = 1u;
    td->token = dt | QTD_CERR | pid | QTD_ACTIVE | ((uint32_t)length << 16);
    if (data && length)
        fill_buffers(td, data);
}

int ehci_init(void)
{
    uint32_t port;
    unsigned i;

    memset(&g_qh, 0, sizeof(g_qh));
    g_qh.horiz = link_qh(&g_qh);
    g_qh.next = 1u;
    g_qh.token = QTD_HALT;
    cache_flush(&g_qh, sizeof(g_qh));

    usb_wr(USB_CMD, usb_rd(USB_CMD) & ~CMD_RS);
    ehci_wait_ms(2);
    usb_wr(USB_CMD, CMD_RST);
    for (i = 0; i < 1000 && (usb_rd(USB_CMD) & CMD_RST); ++i)
        ehci_wait_ms(1);
    usb_wr(USB_MODE, 0x3u);
    usb_wr(USB_ASYNC, (uint32_t)&g_qh);
    usb_wr(USB_CMD, CMD_RS | CMD_ASE);
    for (i = 0; i < 100 && (usb_rd(USB_STS) & STS_ASS) == 0; ++i)
        ehci_wait_ms(1);

    if ((usb_rd(0x100) & 0xffu) == 0) {
        xil_printf("usb: controller off %08x\r\n", usb_rd(0x100));
        return -1;
    }

    port = usb_rd(USB_PORTSC);
    port &= ~PORT_W1C;
    port &= ~(1u << 23);
    port &= ~(3u << 30);
    port |= (2u << 30) | PORT_PP;
    usb_wr(USB_PORTSC, port);
    usb_wr(0x170, 0x80000000u);

    for (i = 0; i < 2000; ++i) {
        if (usb_rd(USB_PORTSC) & PORT_CCS)
            break;
        ehci_wait_ms(1);
    }
    if ((usb_rd(USB_PORTSC) & PORT_CCS) == 0) {
        xil_printf("usb: no connect portsc %08x\r\n", usb_rd(USB_PORTSC));
        return -1;
    }
    ehci_wait_ms(100);

    port = usb_rd(USB_PORTSC);
    port &= ~PORT_W1C;
    port |= PORT_PR;
    usb_wr(USB_PORTSC, port);
    ehci_wait_ms(50);
    port = usb_rd(USB_PORTSC);
    port &= ~PORT_W1C;
    port &= ~PORT_PR;
    usb_wr(USB_PORTSC, port);
    for (i = 0; i < 200; ++i) {
        port = usb_rd(USB_PORTSC);
        if ((port & PORT_PR) == 0 && (port & PORT_PE))
            break;
        ehci_wait_ms(1);
    }
    port = usb_rd(USB_PORTSC);
    if ((port & PORT_PE) == 0) {
        xil_printf("usb: port disabled portsc %08x\r\n", port);
        return -1;
    }
    if ((((port >> 26) & 3u) != 2u) && (port & (1u << 9)) == 0) {
        xil_printf("usb: not high-speed portsc %08x\r\n", port);
        return -1;
    }
    ehci_wait_ms(20);
    return 0;
}

static int control_xfer(uint8_t addr, uint8_t speed, uint8_t hub, uint8_t port, uint16_t mps, const uint8_t setup[8],
                        void* data, uint16_t length)
{
    const int data_in = (setup[0] & 0x80u) != 0;
    uint16_t actual = 0;
    int count = 2;

    if (length > sizeof(g_data) || mps == 0)
        return -1;
    memcpy(g_setup, setup, 8);
    cache_flush(g_setup, sizeof(g_setup));
    if (!data_in && length) {
        memcpy(g_data, data, length);
        cache_flush(g_data, length);
    } else if (data_in && length) {
        cache_inval(g_data, length);
    }

    make_qtd(&g_qtd[0], length ? &g_qtd[1] : &g_qtd[1], 2u << 8, 0, g_setup, 8);
    if (length) {
        make_qtd(&g_qtd[1], &g_qtd[2], (data_in ? 1u : 0u) << 8, QTD_DT, g_data, length);
        make_qtd(&g_qtd[2], 0, (data_in ? 0u : 1u) << 8, QTD_DT, 0, 0);
        count = 3;
    } else {
        make_qtd(&g_qtd[1], 0, 1u << 8, QTD_DT, 0, 0);
    }

    if (run_chain(addr, 0, mps, 1, count, speed == 2u ? 500u : 1000u, 0, 0, speed, hub, port) != 0)
        return -1;
    if (data_in && length && data) {
        const uint32_t left = (g_qtd[1].token >> 16) & 0x7fffu;
        actual = left >= length ? 0 : (uint16_t)(length - left);
        cache_inval(g_data, length);
        memcpy(data, g_data, actual);
    }
    return 0;
}

int ehci_control(uint8_t addr, const uint8_t setup[8], void* data, uint16_t length)
{
    return control_xfer(addr, 2, 0, 0, 64, setup, data, length);
}

int ehci_control_tt(uint8_t addr, uint8_t speed, uint8_t hub, uint8_t port, uint16_t mps, const uint8_t setup[8],
                    void* data, uint16_t length)
{
    return control_xfer(addr, speed, hub, port, mps, setup, data, length);
}

static void prepare_bulk_qtd(uint32_t pid, uint32_t dt, uint16_t length)
{
    make_qtd(&g_qtd[0], 0, pid, dt ? QTD_DT : 0, g_bulk, length);
}

static int bulk_out_xfer(uint8_t addr, uint8_t speed, uint8_t hub, uint8_t port, uint8_t ep, uint16_t mps,
                         const void* data, uint16_t length, int* toggle)
{
    if (length > sizeof(g_bulk) || !toggle || mps == 0)
        return -1;
    memcpy(g_bulk, data, length);
    cache_flush(g_bulk, length);
    prepare_bulk_qtd(0u << 8, *toggle ? 1u : 0u, length);
    if (run_chain(addr, ep, mps, 0, 1, speed == 2u ? 500u : 1000u, length, 0, speed, hub, port) != 0)
        return -1;
    cache_inval(&g_qh, sizeof(g_qh));
    *toggle = (g_qh.token & QTD_DT) ? 1 : 0;
    return 0;
}

int ehci_bulk_out(uint8_t addr, uint8_t ep, uint16_t mps, const void* data, uint16_t length, int* toggle)
{
    return bulk_out_xfer(addr, 2, 0, 0, ep, mps, data, length, toggle);
}

int ehci_bulk_out_tt(uint8_t addr, uint8_t speed, uint8_t hub, uint8_t port, uint8_t ep, uint16_t mps, const void* data,
                     uint16_t length, int* toggle)
{
    return bulk_out_xfer(addr, speed, hub, port, ep, mps, data, length, toggle);
}

int ehci_bulk_in(uint8_t addr, uint8_t ep, uint16_t mps, void* data, uint16_t length, uint16_t* actual, int* toggle,
                 uint32_t timeout_ms)
{
    uint16_t got = 0;
    int rc;
    if (length > sizeof(g_bulk) || !toggle)
        return -1;
    cache_flush(g_bulk, length);
    cache_inval(g_bulk, length);
    prepare_bulk_qtd(1u << 8, *toggle ? 1u : 0u, length);
    rc = run_chain(addr, ep, mps, 0, 1, timeout_ms, length, &got, 2, 0, 0);
    if (rc != 0)
        return rc;
    cache_inval(g_bulk, got ? got : length);
    cache_inval(&g_qh, sizeof(g_qh));
    *toggle = (g_qh.token & QTD_DT) ? 1 : 0;
    if (actual)
        *actual = got;
    if (data && got)
        memcpy(data, g_bulk, got);
    return 0;
}
