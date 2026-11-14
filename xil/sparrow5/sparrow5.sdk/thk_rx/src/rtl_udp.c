#include "rtl_udp.h"
#include "usb_com.h"
#include "usb_ehci.h"

#include "thk_app.h"

#include <string.h>

#define BOARD_IP0 192
#define BOARD_IP1 168
#define BOARD_IP2 1
#define BOARD_IP3 50

#define VID_REALTEK 0x0bda
#define PID_8152 0x8152
#define PID_8153 0x8153

#define MCU_PLA 0x0100u
#define MCU_USB 0x0000u
#define BYTE_DWORD 0xffu
#define BYTE_WORD 0x33u
#define BYTE_BYTE 0x11u
#define BYTE_SIX 0x3fu

#define PLA_IDR 0xc000u
#define PLA_RCR 0xc010u
#define PLA_RXFIFO_CTRL0 0xc0a0u
#define PLA_RXFIFO_CTRL1 0xc0a4u
#define PLA_RXFIFO_CTRL2 0xc0a8u
#define PLA_FMC 0xc0b4u
#define PLA_TEREDO_CFG 0xc0bcu
#define PLA_BDC_CR 0xd1a0u
#define PLA_REALWOW_TIMER 0xd2e8u
#define PLA_TEREDO_TIMER 0xd2ccu
#define PLA_WDT6_CTRL 0xe428u
#define PLA_TCR0 0xe610u
#define PLA_TXFIFO_CTRL 0xe618u
#define PLA_RSTTALLY 0xe800u
#define PLA_CR 0xe813u
#define PLA_CRWECR 0xe81cu
#define PLA_PHY_PWR 0xe84cu
#define PLA_OOB_CTRL 0xe84fu
#define PLA_CPCR 0xe854u
#define PLA_MISC_1 0xe85au
#define PLA_OCP_BASE 0xe86cu
#define PLA_SFF_STS_7 0xe8deu
#define PLA_PHYSTATUS 0xe908u
#define PLA_GPHY_IMR 0xe022u
#define PLA_MAC_PWR 0xe0c0u
#define PLA_EEEP_CR 0xe080u
#define PLA_RMS 0xc016u
#define USB_USB_CTRL 0xd406u
#define USB_RX_BUF_TH 0xd40cu
#define USB_TX_AGG 0xd40au
#define USB_TX_DMA 0xd434u
#define USB_UPS_CTRL 0xd800u
#define USB_PM_CTRL 0xd432u
#define OCP_ALDPS 0x2010u
#define OCP_MII 0xa400u

#define CR_RST 0x10u
#define CR_RE 0x08u
#define CR_TE 0x04u
#define RCR_AB 0x00000008u
#define RCR_APM 0x00000002u
#define RCR_AM 0x00000004u
#define RCR_ALL (0x00000001u | RCR_APM | RCR_AM | RCR_AB)
#define LINK_STATUS 0x0002u

static uint8_t g_addr;
static uint8_t g_ep_in;
static uint8_t g_ep_out;
static uint16_t g_mps_in = 512;
static uint16_t g_mps_out = 512;
static int g_tog_in;
static int g_tog_out;
static uint8_t g_mac[6];
static uint8_t g_version;
static uint16_t g_ocp_base;
static uint8_t g_frame[1600];
static uint8_t g_tx[1600];
static uint16_t g_ip_id;

static int ctrl(uint8_t addr, uint8_t type, uint8_t req, uint16_t value, uint16_t index, void* data, uint16_t length)
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
    return ehci_control(addr, setup, data, length);
}

static int reg_io(int read, uint16_t addr, uint16_t index, void* data, uint16_t size)
{
    return ctrl(g_addr, read ? 0xc0 : 0x40, 0x05, addr, index, data, size);
}

static int ocp_write(uint16_t type, uint16_t addr, uint16_t byteen, const void* data, uint16_t size)
{
    uint8_t buf[8];
    uint16_t start = (uint16_t)(byteen & 0x0fu);
    uint16_t end = (uint16_t)(byteen & 0xf0u);
    uint16_t byen = (uint16_t)(start | (start << 4));

    if (size != 4 && size != 8)
        return -1;
    memcpy(buf, data, size);
    if (byen != BYTE_DWORD) {
        if (reg_io(0, addr, (uint16_t)(type | byen), buf, 4) != 0)
            return -1;
        return 0;
    }
    byen = (uint16_t)(end | (end >> 4));
    if (byen != BYTE_DWORD)
        size = (uint16_t)(size - 4);
    if (reg_io(0, addr, (uint16_t)(type | BYTE_DWORD), buf, size) != 0)
        return -1;
    if (byen != BYTE_DWORD)
        return reg_io(0, (uint16_t)(addr + size), (uint16_t)(type | byen), buf + size, 4);
    return 0;
}

static uint32_t ocp_read_dword(uint16_t type, uint16_t addr)
{
    uint32_t value = 0;
    reg_io(1, addr, type, &value, 4);
    return value;
}

static void ocp_write_dword(uint16_t type, uint16_t addr, uint32_t value)
{
    ocp_write(type, addr, BYTE_DWORD, &value, 4);
}

static uint16_t ocp_read_word(uint16_t type, uint16_t addr)
{
    /* Byte enables belong to writes. A read index is only the PLA/USB type. */
    uint32_t value = ocp_read_dword(type, (uint16_t)(addr & ~3u));
    return (uint16_t)(value >> (8u * (addr & 3u)));
}

static void ocp_write_word(uint16_t type, uint16_t addr, uint16_t value)
{
    uint32_t tmp = value;
    uint16_t byen = BYTE_WORD;
    if (addr & 2u) {
        byen = (uint16_t)(byen << 2);
        tmp <<= 16;
        addr = (uint16_t)(addr & ~2u);
    }
    ocp_write(type, addr, byen, &tmp, 4);
}

static uint8_t ocp_read_byte(uint16_t type, uint16_t addr)
{
    uint32_t value = ocp_read_dword(type, (uint16_t)(addr & ~3u));
    return (uint8_t)(value >> (8 * (addr & 3u)));
}

static void ocp_write_byte(uint16_t type, uint16_t addr, uint8_t value)
{
    uint32_t tmp = value;
    uint16_t byen = BYTE_BYTE;
    uint8_t shift = (uint8_t)(addr & 3u);
    tmp <<= 8 * shift;
    byen = (uint16_t)(byen << shift);
    ocp_write(type, (uint16_t)(addr & ~3u), byen, &tmp, 4);
}

static void mdio_write(uint16_t reg, uint16_t value)
{
    uint16_t addr = (uint16_t)(OCP_MII + reg * 2u);
    uint16_t base = (uint16_t)(addr & 0xf000u);
    if (base != g_ocp_base) {
        ocp_write_word(MCU_PLA, PLA_OCP_BASE, base);
        g_ocp_base = base;
    }
    ocp_write_word(MCU_PLA, (uint16_t)((addr & 0x0fffu) | 0xb000u), value);
}

static uint16_t mdio_read(uint16_t reg)
{
    uint16_t addr = (uint16_t)(OCP_MII + reg * 2u);
    uint16_t base = (uint16_t)(addr & 0xf000u);
    if (base != g_ocp_base) {
        ocp_write_word(MCU_PLA, PLA_OCP_BASE, base);
        g_ocp_base = base;
    }
    return ocp_read_word(MCU_PLA, (uint16_t)((addr & 0x0fffu) | 0xb000u));
}

static int read_version(void)
{
    uint32_t raw = ocp_read_dword(MCU_PLA, PLA_TCR0);
    uint32_t code = (raw >> 16) & 0x7cf0u;
    if (code == 0x4c00u)
        g_version = 1;
    else if (code == 0x4c10u)
        g_version = 2;
    else if (code == 0x4800u)
        g_version = 7;
    else {
        g_version = 0;
        return -1;
    }
    return 0;
}

static void aldps(int enable)
{
    uint16_t addr = OCP_ALDPS;
    uint16_t base = (uint16_t)(addr & 0xf000u);
    uint16_t value = enable ? (uint16_t)(0x8000u | 0x0200u | 0x0100u | 0x0010u)
                            : (uint16_t)(0x0200u | 0x0100u | 0x0010u);
    if (base != g_ocp_base) {
        ocp_write_word(MCU_PLA, PLA_OCP_BASE, base);
        g_ocp_base = base;
    }
    ocp_write_word(MCU_PLA, (uint16_t)((addr & 0x0fffu) | 0xb000u), value);
    if (!enable)
        ehci_wait_ms(20);
}

static void nic_reset(void)
{
    unsigned i;
    ocp_write_byte(MCU_PLA, PLA_CR, CR_RST);
    for (i = 0; i < 100; ++i) {
        if ((ocp_read_byte(MCU_PLA, PLA_CR) & CR_RST) == 0)
            return;
        ehci_wait_ms(1);
    }
}

static void exit_oob(void)
{
    uint32_t value;
    unsigned i;
    value = ocp_read_dword(MCU_PLA, PLA_RCR);
    ocp_write_dword(MCU_PLA, PLA_RCR, value & ~RCR_ALL);
    value = ocp_read_word(MCU_PLA, PLA_MISC_1);
    ocp_write_word(MCU_PLA, PLA_MISC_1, (uint16_t)(value | 0x0008u));
    value = ocp_read_word(MCU_PLA, PLA_TEREDO_CFG);
    ocp_write_word(MCU_PLA, PLA_TEREDO_CFG, (uint16_t)(value & ~(0x8000u | 0x00feu | 0x0001u)));
    ocp_write_word(MCU_PLA, PLA_WDT6_CTRL, 0x0010u);
    ocp_write_word(MCU_PLA, PLA_REALWOW_TIMER, 0);
    ocp_write_dword(MCU_PLA, PLA_TEREDO_TIMER, 0);
    ocp_write_byte(MCU_PLA, PLA_CRWECR, 0x00);
    ocp_write_byte(MCU_PLA, PLA_CR, 0x00);
    value = ocp_read_byte(MCU_PLA, PLA_OOB_CTRL);
    ocp_write_byte(MCU_PLA, PLA_OOB_CTRL, (uint8_t)(value & ~0x80u));
    value = ocp_read_word(MCU_PLA, PLA_SFF_STS_7);
    ocp_write_word(MCU_PLA, PLA_SFF_STS_7, (uint16_t)(value & ~0x4000u));
    for (i = 0; i < 200; ++i) {
        if (ocp_read_byte(MCU_PLA, PLA_OOB_CTRL) & 0x02u)
            break;
        ehci_wait_ms(1);
    }
    value = ocp_read_word(MCU_PLA, PLA_SFF_STS_7);
    ocp_write_word(MCU_PLA, PLA_SFF_STS_7, (uint16_t)(value | 0x8000u));
    for (i = 0; i < 200; ++i) {
        if (ocp_read_byte(MCU_PLA, PLA_OOB_CTRL) & 0x02u)
            break;
        ehci_wait_ms(1);
    }
    nic_reset();
    ocp_write_dword(MCU_PLA, PLA_RXFIFO_CTRL0, 0x00080002u);
    ocp_write_dword(MCU_PLA, PLA_RXFIFO_CTRL1, 0x00000038u);
    ocp_write_dword(MCU_PLA, PLA_RXFIFO_CTRL2, 0x00000048u);
    ocp_write_dword(MCU_PLA, PLA_TXFIFO_CTRL, 0x01000008u);
    ocp_write_byte(MCU_USB, USB_TX_AGG, 0x03);
    ocp_write_dword(MCU_USB, USB_RX_BUF_TH, 0x7a120180u);
    ocp_write_dword(MCU_USB, USB_TX_DMA, 0x00000101u);
    value = ocp_read_word(MCU_PLA, PLA_CPCR);
    ocp_write_word(MCU_PLA, PLA_CPCR, (uint16_t)(value | 0x0040u));
    ocp_write_word(MCU_PLA, PLA_RMS, 1518);
    value = ocp_read_word(MCU_PLA, PLA_TCR0);
    ocp_write_word(MCU_PLA, PLA_TCR0, (uint16_t)(value | 0x0080u));
}

static void rtl_init_mac(void)
{
    uint16_t bmcr = mdio_read(0);
    uint32_t value;
    if (bmcr & 0x0800u)
        mdio_write(0, (uint16_t)(bmcr & ~0x0800u));
    aldps(0);
    value = ocp_read_word(MCU_USB, USB_UPS_CTRL);
    ocp_write_word(MCU_USB, USB_UPS_CTRL, (uint16_t)(value & ~0x0100u));
    value = ocp_read_word(MCU_USB, USB_PM_CTRL);
    ocp_write_word(MCU_USB, USB_PM_CTRL, (uint16_t)(value & ~0x0001u));
    value = ocp_read_word(MCU_PLA, PLA_PHY_PWR);
    ocp_write_word(MCU_PLA, PLA_PHY_PWR, (uint16_t)(value | 0x00c0u));
    value = ocp_read_dword(MCU_PLA, PLA_MAC_PWR);
    value &= ~0x0f0f0f0fu;
    value |= 0x07010f07u | 0x00004000u;
    ocp_write_dword(MCU_PLA, PLA_MAC_PWR, value);
    ocp_write_word(MCU_PLA, PLA_GPHY_IMR, 0x000fu);
    value = ocp_read_word(MCU_PLA, PLA_RSTTALLY);
    ocp_write_word(MCU_PLA, PLA_RSTTALLY, (uint16_t)(value | 0x0001u));
    value = ocp_read_word(MCU_USB, USB_USB_CTRL);
    ocp_write_word(MCU_USB, USB_USB_CTRL, (uint16_t)(value & ~(0x0010u | 0x0080u)));
}

static void rtl_enable(void)
{
    uint16_t value = ocp_read_word(MCU_PLA, PLA_FMC);
    ocp_write_word(MCU_PLA, PLA_FMC, (uint16_t)(value & ~0x0001u));
    ocp_write_word(MCU_PLA, PLA_FMC, (uint16_t)(value | 0x0001u));
    value = ocp_read_byte(MCU_PLA, PLA_CR);
    ocp_write_byte(MCU_PLA, PLA_CR, (uint8_t)(value | CR_RE | CR_TE));
    value = ocp_read_word(MCU_PLA, PLA_MISC_1);
    ocp_write_word(MCU_PLA, PLA_MISC_1, (uint16_t)(value & ~0x0008u));
    value = ocp_read_dword(MCU_PLA, PLA_RCR);
    value &= ~RCR_ALL;
    value |= RCR_ALL;
    {
        uint32_t filter[2] = { 0xffffffffu, 0xffffffffu };
        ocp_write(MCU_PLA, 0xcd00u, BYTE_DWORD, filter, 8);
        ocp_write_dword(MCU_PLA, PLA_RCR, value);
    }
    /* One ARP is far below the 384-byte aggregate mark. Push every frame out at once. */
    ocp_write_dword(MCU_USB, USB_RX_BUF_TH, 0x00010001u);
}

static int write_mac(const uint8_t mac[6])
{
    uint8_t raw[8];
    memset(raw, 0, sizeof(raw));
    memcpy(raw, mac, 6);
    ocp_write_byte(MCU_PLA, PLA_CRWECR, 0xc0);
    if (ocp_write(MCU_PLA, PLA_IDR, BYTE_SIX, raw, 8) != 0)
        return -1;
    ocp_write_byte(MCU_PLA, PLA_CRWECR, 0x00);
    return 0;
}

static int load_mac(void)
{
    uint8_t raw[8];
    unsigned i;
    int blank = 1;
    memset(raw, 0, sizeof(raw));
    if (reg_io(1, PLA_IDR, MCU_PLA, raw, 8) != 0)
        memset(raw, 0, sizeof(raw));
    memcpy(g_mac, raw, 6);
    for (i = 0; i < 6; ++i) {
        if (g_mac[i] != 0)
            blank = 0;
    }
    if (blank || (g_mac[0] & 1u)) {
        g_mac[0] = 0x02;
        g_mac[1] = 0xe0;
        g_mac[2] = 0x4c;
        g_mac[3] = 0x00;
        g_mac[4] = 0x81;
        g_mac[5] = 0x52;
    }
    if (write_mac(g_mac) != 0)
        return -1;
    memset(raw, 0, sizeof(raw));
    if (reg_io(1, PLA_IDR, MCU_PLA, raw, 8) != 0 || memcmp(raw, g_mac, 6) != 0)
        return -1;
    return 0;
}

static void advertise(void)
{
    unsigned i;
    mdio_write(0, 0x8000u);
    for (i = 0; i < 100; ++i) {
        if ((mdio_read(0) & 0x8000u) == 0)
            break;
        ehci_wait_ms(1);
    }
    mdio_write(4, 0x01e1u);
    mdio_write(0, 0x1200u);
}

static int wait_link(void)
{
    unsigned i;
    for (i = 0; i < 80; ++i) {
        if (ocp_read_word(MCU_PLA, PLA_PHYSTATUS) & LINK_STATUS)
            return 0;
        ehci_wait_ms(100);
    }
    return -1;
}

static int send_eth(const uint8_t dst[6], uint16_t type, const uint8_t* payload, uint16_t length)
{
    uint16_t frame = (uint16_t)(14u + length);
    uint32_t opts;
    unsigned i;
    if (frame > 1514)
        return -1;
    if (frame < 60)
        frame = 60;
    memset(g_tx, 0, 8u + frame);
    opts = frame | (1u << 31) | (1u << 30);
    g_tx[0] = (uint8_t)opts;
    g_tx[1] = (uint8_t)(opts >> 8);
    g_tx[2] = (uint8_t)(opts >> 16);
    g_tx[3] = (uint8_t)(opts >> 24);
    memcpy(g_tx + 8, dst, 6);
    memcpy(g_tx + 14, g_mac, 6);
    g_tx[20] = (uint8_t)(type >> 8);
    g_tx[21] = (uint8_t)type;
    if (length)
        memcpy(g_tx + 22, payload, length);
    for (i = 0; i < 3; ++i) {
        if (ehci_bulk_out(g_addr, g_ep_out, g_mps_out, g_tx, (uint16_t)(8u + frame), &g_tog_out) == 0)
            return 0;
        ehci_wait_ms(1);
    }
    return -1;
}

static uint16_t ip_sum(const uint8_t* data, unsigned length)
{
    uint32_t sum = 0;
    unsigned i;
    for (i = 0; i + 1 < length; i += 2)
        sum += ((uint32_t)data[i] << 8) | data[i + 1];
    if (length & 1u)
        sum += (uint32_t)data[length - 1] << 8;
    while (sum >> 16)
        sum = (sum & 0xffffu) + (sum >> 16);
    return (uint16_t)~sum;
}

static int our_ip(const uint8_t* ip)
{
    return ip[0] == BOARD_IP0 && ip[1] == BOARD_IP1 && ip[2] == BOARD_IP2 && ip[3] == BOARD_IP3;
}

static void reply_arp(const uint8_t* frame)
{
    uint8_t payload[28];
    memcpy(payload, frame + 14, 28);
    payload[6] = 0;
    payload[7] = 2;
    memcpy(payload + 8, g_mac, 6);
    payload[14] = BOARD_IP0;
    payload[15] = BOARD_IP1;
    payload[16] = BOARD_IP2;
    payload[17] = BOARD_IP3;
    memcpy(payload + 18, frame + 22, 6);
    memcpy(payload + 24, frame + 28, 4);
    (void)send_eth(frame + 6, 0x0806, payload, 28);
}

static void reply_icmp(const uint8_t* ip, uint16_t ip_len)
{
    uint8_t pkt[576];
    uint16_t icmp_len;
    uint16_t sum;
    if (ip_len < 28 || ip_len > sizeof(pkt))
        return;
    memcpy(pkt, ip, ip_len);
    memcpy(pkt + 12, ip + 16, 4);
    memcpy(pkt + 16, ip + 12, 4);
    pkt[8] = 64;
    pkt[10] = 0;
    pkt[11] = 0;
    sum = ip_sum(pkt, 20);
    pkt[10] = (uint8_t)(sum >> 8);
    pkt[11] = (uint8_t)sum;
    icmp_len = (uint16_t)(ip_len - 20);
    pkt[20] = 0;
    pkt[22] = 0;
    pkt[23] = 0;
    sum = ip_sum(pkt + 20, icmp_len);
    pkt[22] = (uint8_t)(sum >> 8);
    pkt[23] = (uint8_t)sum;
    (void)send_eth(g_frame + 6, 0x0800, pkt, ip_len);
}

static void reply_udp(const uint8_t* ip, const uint8_t* udp, uint16_t ulen)
{
    uint8_t pkt[1600];
    uint16_t reply_len = 0;
    /* TEXT ACK may append up to 64 COM RX bytes after ThkAckBody. */
    uint8_t reply[sizeof(ThkHeader) + sizeof(ThkAckBody) + 64];
    uint16_t sport = (uint16_t)((udp[0] << 8) | udp[1]);
    uint16_t dport = (uint16_t)((udp[2] << 8) | udp[3]);
    const uint8_t* payload = udp + 8;
    uint16_t plen = (uint16_t)(ulen - 8);
    if (dport != THK_UDP_PORT || plen < sizeof(ThkHeader))
        return;
    reply_len = thk_handle(payload, plen, reply, sizeof(reply));
    if (reply_len == 0 || 20u + 8u + reply_len > sizeof(pkt))
        return;
    memset(pkt, 0, 28);
    pkt[0] = 0x45;
    pkt[2] = (uint8_t)((28u + reply_len) >> 8);
    pkt[3] = (uint8_t)(28u + reply_len);
    g_ip_id++;
    pkt[4] = (uint8_t)(g_ip_id >> 8);
    pkt[5] = (uint8_t)g_ip_id;
    pkt[8] = 64;
    pkt[9] = 17;
    memcpy(pkt + 12, ip + 16, 4);
    memcpy(pkt + 16, ip + 12, 4);
    {
        uint16_t sum = ip_sum(pkt, 20);
        pkt[10] = (uint8_t)(sum >> 8);
        pkt[11] = (uint8_t)sum;
    }
    pkt[20] = (uint8_t)(dport >> 8);
    pkt[21] = (uint8_t)dport;
    pkt[22] = (uint8_t)(sport >> 8);
    pkt[23] = (uint8_t)sport;
    pkt[24] = (uint8_t)((8u + reply_len) >> 8);
    pkt[25] = (uint8_t)(8u + reply_len);
    memcpy(pkt + 28, reply, reply_len);
    {
        uint16_t udp_len = (uint16_t)(8u + reply_len);
        uint32_t sum = 0;
        unsigned i;
        sum += ((uint32_t)pkt[12] << 8) | pkt[13];
        sum += ((uint32_t)pkt[14] << 8) | pkt[15];
        sum += ((uint32_t)pkt[16] << 8) | pkt[17];
        sum += ((uint32_t)pkt[18] << 8) | pkt[19];
        sum += 17u;
        sum += udp_len;
        for (i = 0; i + 1u < udp_len; i += 2u)
            sum += ((uint32_t)pkt[20u + i] << 8) | pkt[21u + i];
        if (udp_len & 1u)
            sum += (uint32_t)pkt[20u + udp_len - 1u] << 8;
        while (sum >> 16)
            sum = (sum & 0xffffu) + (sum >> 16);
        sum = ~sum & 0xffffu;
        if (sum == 0)
            sum = 0xffffu;
        pkt[26] = (uint8_t)(sum >> 8);
        pkt[27] = (uint8_t)sum;
    }
    send_eth(g_frame + 6, 0x0800, pkt, (uint16_t)(28u + reply_len));
}

static void on_ethernet(const uint8_t* frame, uint16_t length)
{
    uint16_t type;
    uint16_t ip_len;
    uint8_t ihl;
    if (length < 14)
        return;
    memcpy(g_frame, frame, length < sizeof(g_frame) ? length : sizeof(g_frame));
    type = (uint16_t)((frame[12] << 8) | frame[13]);
    if (type == 0x0806 && length >= 42) {
        if (frame[14] == 0 && frame[15] == 1 && frame[16] == 8 && frame[20] == 0 && frame[21] == 1
            && our_ip(frame + 38))
            reply_arp(frame);
        return;
    }
    if (type != 0x0800 || length < 34 || frame[14] != 0x45)
        return;
    ip_len = (uint16_t)((frame[16] << 8) | frame[17]);
    if (ip_len < 20 || 14u + ip_len > length)
        return;
    if (!our_ip(frame + 30))
        return;
    ihl = (uint8_t)((frame[14] & 0x0fu) * 4u);
    if (ihl < 20 || ihl > ip_len)
        return;
    if (frame[23] == 1 && frame[34] == 8)
        reply_icmp(frame + 14, ip_len);
    else if (frame[23] == 17 && ip_len >= (uint16_t)(ihl + 8)) {
        reply_udp(frame + 14, frame + 14 + ihl, (uint16_t)(ip_len - ihl));
    }
}

static int ethernet_type_at(const uint8_t* buf, uint16_t length)
{
    uint16_t type;
    if (length < 42)
        return 0;
    type = (uint16_t)((buf[12] << 8) | buf[13]);
    return type == 0x0800 || type == 0x0806;
}

static void on_rx(const uint8_t* buf, uint16_t length)
{
    uint16_t offset = 0;
    int parsed = 0;
    if (length >= 42 && buf[0] == 0xff && buf[1] == 0xff && buf[2] == 0xff && buf[3] == 0xff && buf[4] == 0xff
        && buf[5] == 0xff) {
        on_ethernet(buf, length);
        return;
    }
    while (offset + 24 < length) {
        uint32_t opts;
        uint16_t pkt;
        uint16_t avail;
        uint16_t step;
        const uint8_t* eth;
        memcpy(&opts, buf + offset, 4);
        pkt = (uint16_t)(opts & 0x7fffu);
        if (offset + 24u >= length)
            break;
        avail = (uint16_t)(length - offset - 24u);
        if (pkt > avail)
            break;
        eth = buf + offset + 24;
        if (pkt >= 34 && pkt < avail && eth[12] == 0x08 && eth[13] == 0x00 && eth[14] == 0x45) {
            uint16_t ip_len = (uint16_t)((eth[16] << 8) | eth[17]);
            uint16_t need = (uint16_t)(14u + ip_len);
            if (need > pkt && need <= avail && need <= 1514)
                pkt = need;
        }
        if (pkt < 14)
            break;
        on_ethernet(eth, pkt);
        parsed++;
        step = (uint16_t)(24u + pkt);
        if (pkt & 7u) {
            uint16_t pad = (uint16_t)(8u - (pkt & 7u));
            if (offset + step + pad > length)
                break;
            step = (uint16_t)(step + pad);
        }
        offset = (uint16_t)(offset + step);
    }
    /* A short unicast frame can arrive without a descriptor. Its first byte is the
       destination MAC, which the descriptor walker reads as a tiny length and skips. */
    if (parsed == 0 && ethernet_type_at(buf, length))
        on_ethernet(buf, length);
}

static int take_bulk_endpoints(const uint8_t* cfg, uint16_t total)
{
    const uint8_t* p = cfg;
    const uint8_t* end = cfg + total;
    g_ep_in = 0;
    g_ep_out = 0;
    while (p + 2 <= end) {
        uint8_t len = p[0];
        if (len < 2 || p + len > end)
            break;
        if (p[1] == 5 && len >= 7 && (p[3] & 3u) == 2u) {
            uint16_t mps = (uint16_t)((p[4] | (p[5] << 8)) & 0x7ffu);
            if (p[2] & 0x80u) {
                g_ep_in = (uint8_t)(p[2] & 0x0fu);
                g_mps_in = mps ? mps : 512;
            } else {
                g_ep_out = (uint8_t)(p[2] & 0x0fu);
                g_mps_out = mps ? mps : 512;
            }
        }
        p += len;
    }
    return g_ep_in && g_ep_out ? 0 : -1;
}

static int set_address(uint8_t from, uint8_t to)
{
    if (ctrl(from, 0x00, 5, to, 0, 0, 0) != 0)
        return -1;
    ehci_wait_ms(20);
    return 0;
}

static int read_device(uint8_t addr, uint8_t desc[18])
{
    return ctrl(addr, 0x80, 6, 0x0100, 0, desc, 18);
}

static int read_config(uint8_t addr, uint8_t* cfg, uint16_t cap, uint16_t* total)
{
    if (ctrl(addr, 0x80, 6, 0x0200, 0, cfg, 9) != 0)
        return -1;
    *total = (uint16_t)(cfg[2] | (cfg[3] << 8));
    if (*total < 9 || *total > cap)
        return -1;
    return ctrl(addr, 0x80, 6, 0x0200, 0, cfg, *total);
}

static int configure(uint8_t addr, uint8_t value)
{
    return ctrl(addr, 0x00, 9, value, 0, 0, 0);
}

static int open_nic(uint8_t addr, const uint8_t desc[18])
{
    uint8_t cfg[256];
    uint16_t total = 0;
    uint16_t vid = (uint16_t)(desc[8] | (desc[9] << 8));
    uint16_t pid = (uint16_t)(desc[10] | (desc[11] << 8));
    if (vid != VID_REALTEK || (pid != PID_8152 && pid != PID_8153))
        return -1;
    g_addr = addr;
    g_tog_in = 0;
    g_tog_out = 0;
    g_ocp_base = 0;
    if (read_config(addr, cfg, sizeof(cfg), &total) != 0)
        return -1;
    if (configure(addr, cfg[5]) != 0)
        return -1;
    if (take_bulk_endpoints(cfg, total) != 0)
        return -1;
    if (read_version() != 0)
        return -1;
    rtl_init_mac();
    aldps(0);
    exit_oob();
    if (load_mac() != 0)
        return -1;
    advertise();
    (void)wait_link();
    aldps(0);
    rtl_enable();
    (void)ocp_read_word(MCU_PLA, PLA_PHYSTATUS);
    return 0;
}

static int ctrl_pipe(uint8_t addr, uint8_t speed, uint8_t hub, uint8_t port, uint16_t mps, uint8_t type, uint8_t req,
                     uint16_t value, uint16_t index, void* data, uint16_t length)
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
    if (speed == 2)
        return ehci_control(addr, setup, data, length);
    return ehci_control_tt(addr, speed, hub, port, mps, setup, data, length);
}

static int set_address_pipe(uint8_t from, uint8_t to, uint8_t speed, uint8_t hub, uint8_t port, uint16_t mps)
{
    if (ctrl_pipe(from, speed, hub, port, mps, 0x00, 5, to, 0, 0, 0) != 0)
        return -1;
    ehci_wait_ms(20);
    return 0;
}

static int hub_child(uint8_t hub)
{
    uint8_t hub_desc[9];
    uint8_t ports;
    uint8_t port;
    uint8_t next_addr = 2;
    int nic = -1;
    int have_com = 0;
    if (ctrl(hub, 0xa0, 6, 0x2900, 0, hub_desc, sizeof(hub_desc)) != 0)
        return -1;
    ports = hub_desc[2];
    if (ports > 4)
        ports = 4;
    for (port = 1; port <= ports; ++port)
        ctrl(hub, 0x23, 3, 8, port, 0, 0);
    ehci_wait_ms(100);
    for (port = 1; port <= ports; ++port) {
        uint8_t status[4];
        uint8_t desc[18];
        uint8_t cfg[256];
        uint16_t total;
        uint16_t vid;
        uint16_t pid;
        uint16_t mps;
        uint8_t speed;
        uint8_t addr;
        unsigned i;
        int attached;
        ctrl(hub, 0xa3, 0, 0, port, status, 4);
        if ((status[0] & 0x01u) == 0)
            continue;
        if (ctrl(hub, 0x23, 3, 4, port, 0, 0) != 0)
            continue;
        for (i = 0; i < 100; ++i) {
            if (ctrl(hub, 0xa3, 0, 0, port, status, 4) != 0)
                break;
            if ((status[0] & 0x10u) == 0)
                break;
            ehci_wait_ms(1);
        }
        ctrl(hub, 0x23, 1, 20, port, 0, 0);
        ctrl(hub, 0x23, 1, 16, port, 0, 0);
        if ((status[0] & 0x02u) == 0)
            continue;
        if (status[1] & 0x02u)
            speed = 1;
        else if (status[1] & 0x04u)
            speed = 2;
        else
            speed = 0;
        mps = (speed == 2) ? 64 : 8;
        if (speed != 2) {
            if (ctrl_pipe(0, speed, hub, port, 8, 0x80, 6, 0x0100, 0, desc, 8) != 0)
                continue;
            if (desc[7] == 8 || desc[7] == 16 || desc[7] == 32 || desc[7] == 64)
                mps = desc[7];
        }
        if (ctrl_pipe(0, speed, hub, port, mps, 0x80, 6, 0x0100, 0, desc, 18) != 0)
            continue;
        if (next_addr > 6)
            continue;
        addr = next_addr++;
        if (set_address_pipe(0, addr, speed, hub, port, mps) != 0)
            continue;
        if (ctrl_pipe(addr, speed, hub, port, mps, 0x80, 6, 0x0100, 0, desc, 18) != 0)
            continue;
        vid = (uint16_t)(desc[8] | (desc[9] << 8));
        pid = (uint16_t)(desc[10] | (desc[11] << 8));
        if (ctrl_pipe(addr, speed, hub, port, mps, 0x80, 6, 0x0200, 0, cfg, 9) != 0)
            continue;
        total = (uint16_t)(cfg[2] | (cfg[3] << 8));
        if (total < 9 || total > sizeof(cfg))
            continue;
        if (ctrl_pipe(addr, speed, hub, port, mps, 0x80, 6, 0x0200, 0, cfg, total) != 0)
            continue;
        if (nic != 0 && speed == 2 && vid == VID_REALTEK && (pid == PID_8152 || pid == PID_8153)) {
            nic = open_nic(addr, desc);
            continue;
        }
        if (have_com)
            continue;
        attached = com_attach(addr, speed, hub, port, mps, desc, cfg, total);
        if (attached == 0)
            have_com = 1;
    }
    (void)have_com;
    return nic == 0 ? 0 : -1;
}

static int enumerate(void)
{
    uint8_t desc[18];
    uint8_t cfg[64];
    uint16_t total = 0;
    if (read_device(0, desc) != 0)
        return -1;
    if (set_address(0, 1) != 0)
        return -1;
    if (read_device(1, desc) != 0)
        return -1;
    if ((desc[8] | (desc[9] << 8)) == VID_REALTEK)
        return open_nic(1, desc);
    if (read_config(1, cfg, sizeof(cfg), &total) != 0)
        return -1;
    if (configure(1, cfg[5]) != 0)
        return -1;
    if (desc[4] == 9 || (total > 14 && cfg[14] == 9))
        return hub_child(1);
    return -1;
}

static uint8_t g_rx[4096];

static void clear_in(void)
{
    ctrl(g_addr, 0x02, 1, 0, (uint16_t)(g_ep_in | 0x80u), 0, 0);
    g_tog_in = 0;
}

static int pump(void)
{
    uint16_t actual = 0;
    int idle = 0;
    for (;;) {
        const int got = ehci_bulk_in(g_addr, g_ep_in, g_mps_in, g_rx, sizeof(g_rx), &actual, &g_tog_in, 1000);
        if (got == -2)
            continue;
        if (got != 0) {
            clear_in();
            if (++idle > 20)
                return -1;
            continue;
        }
        idle = 0;
        if (!actual)
            continue;
        on_rx(g_rx, actual);
    }
}

void eth_run(void)
{
    for (;;) {
        if (ehci_init() != 0 || enumerate() != 0) {
            ehci_wait_ms(1000);
            continue;
        }
        (void)pump();
    }
}
