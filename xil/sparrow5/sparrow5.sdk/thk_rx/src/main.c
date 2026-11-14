#include "mcp4725.h"
#include "rtl_udp.h"
#include "thk_app.h"
#include "thk_bank.h"
#include "usb_com.h"
#include "usb_ehci.h"

#include "xil_io.h"
#include "xil_printf.h"
#include "xparameters.h"

#include <string.h>

/* GP0 sparrow_ps: 0x43C00000, 64 KB window.
   Halfword C index N is byte address 2N.
   DAC RAM: byte 0x4000, bits [15:14] = ADDR_DAC_RAM.
   DAC registers: byte 0x1000, bits [13:12] = ADDR_DAC_REGS. */
#define FPGA_BASE XPAR_M_AXI_BASEADDR
#define DAC_RAM_INDEX 0x2000u
#define DAC_REG_INDEX(offs) (0x0800u | (offs))
#define OFFS_LEN 6u
#define OFFS_DELAY 7u
#define OFFS_PERIOD 8u
#define OFFS_TRIG_L 9u
#define OFFS_TRIG_H 10u
#define OFFS_CONF 0u
#define OFFS_UPR 2u
#define OFFS_B_PH_L 2u
#define OFFS_B_PH_H 3u
#define OFFS_DDS_L 4u
#define OFFS_DDS_H 5u
#define B_ON_DDS 1u
#define B_EN_DDS 2u
#define B_EXT_SYNC 4u
#define B_FREERUN 6u
#define B_ONCE 7u
#define SINE_SAMPLES 8192u
/* DDS phase accumulator runs on the 250 MHz DAC clock. */
#define DAC_CLK_HZ 250000000u

#define ADDR_SPD_SPI 0x11u
#define ADDR_SPI_CS_A 0x12u
#define ADDR_SPI_WR_DATA 0x14u
#define ADDR_CNF_SPI 0x17u
#define SPI_BUSY_BIT 0x2u

static ThkBank g_bank;

static uint16_t fpga_read(uint32_t index)
{
    return Xil_In16(FPGA_BASE + index * 2u);
}

static void fpga_write(uint32_t index, uint16_t value)
{
    Xil_Out16(FPGA_BASE + index * 2u, value);
}

static int ram_matches(const int16_t* samples, uint32_t count, int second_read)
{
    uint32_t i;
    for (i = 0; i < count; ++i) {
        uint16_t value = fpga_read(DAC_RAM_INDEX + i);
        if (second_read)
            value = fpga_read(DAC_RAM_INDEX + i);
        if (value != (uint16_t)samples[i])
            return 0;
    }
    return 1;
}

static int spi_idle(void)
{
    uint32_t n;
    for (n = 0; n < 100000u; ++n) {
        if ((fpga_read(ADDR_SPI_CS_A) & SPI_BUSY_BIT) == 0)
            return 1;
    }
    return 0;
}

static int dac_spi_write(uint8_t addr, uint16_t data)
{
    fpga_write(ADDR_CNF_SPI, 0);
    fpga_write(ADDR_SPI_CS_A, 1);
    fpga_write(ADDR_SPI_WR_DATA, (uint16_t)(addr & 0x7fu));
    if (!spi_idle())
        return 0;
    fpga_write(ADDR_SPI_WR_DATA, (uint16_t)(data >> 8));
    if (!spi_idle())
        return 0;
    fpga_write(ADDR_SPI_WR_DATA, (uint16_t)(data & 0xffu));
    if (!spi_idle())
        return 0;
    fpga_write(ADDR_SPI_CS_A, 0);
    return 1;
}

static int dac_hw_init(void)
{
    volatile uint32_t wait;
    fpga_write(OFFS_UPR, 0);
    for (wait = 0; wait < 8000u; ++wait)
        ;
    fpga_write(OFFS_UPR, 1);
    fpga_write(ADDR_SPD_SPI, 18);
    fpga_write(ADDR_SPI_CS_A, 0);
    fpga_write(DAC_REG_INDEX(OFFS_CONF), 0);
    /* config0 bit 10 (twos) stays clear: the DAC reads offset binary.
       With that bit set, codes above 0x2000 are decoded as large negative values. */
    if (!dac_spi_write(0x00, 0x42FC) || !dac_spi_write(0x01, 0x401E) || !dac_spi_write(0x02, 0x3F80)
        || !dac_spi_write(0x03, 0x0001) || !dac_spi_write(0x08, 0x6000) || !dac_spi_write(0x09, 0x8000)
        || !dac_spi_write(0x0A, 0xF080) || !dac_spi_write(0x14, 0x0000) || !dac_spi_write(0x01, 0x401E)
        || !dac_spi_write(0x01, 0x403E) || !dac_spi_write(0x01, 0x401E))
        return 0;
    return 1;
}

static void dac_store(const int16_t* samples, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; ++i)
        fpga_write(DAC_RAM_INDEX + i, (uint16_t)samples[i]);
    fpga_write(DAC_REG_INDEX(OFFS_LEN), (uint16_t)count);
    fpga_write(DAC_REG_INDEX(OFFS_DELAY), 0);
    if (!ram_matches(samples, count, 0) && !ram_matches(samples, count, 1))
        g_bank.last_error = THK_ERR_RAM;
}

static int32_t sine_full(unsigned n)
{
    unsigned quad = n >> 11;
    unsigned x = n & 2047u;
    int neg = (quad & 2u) != 0;
    double t;
    double t2;
    double s;
    int32_t y;
    if (quad & 1u)
        x = 2048u - x;
    t = (double)x * (1.5707963267948966 / 2048.0);
    t2 = t * t;
    s = t * (1.0 - t2 * (1.0 / 6.0) * (1.0 - t2 * (1.0 / 20.0) * (1.0 - t2 * (1.0 / 42.0) * (1.0 - t2 / 72.0))));
    if (s > 1.0)
        s = 1.0;
    if (s < 0.0)
        s = 0.0;
    y = (int32_t)(s * 32767.0 + 0.5);
    return neg ? -y : y;
}

static void sine_start(uint32_t freq_hz, uint16_t percent)
{
    uint32_t step;
    unsigned i;
    if (freq_hz < 1000u)
        freq_hz = 1000u;
    if (freq_hz > 20000000u)
        freq_hz = 20000000u;
    if (percent > 100u)
        percent = 100u;
    step = (uint32_t)(((uint64_t)freq_hz << 32) / DAC_CLK_HZ);
    fpga_write(DAC_REG_INDEX(OFFS_CONF), 0);
    for (i = 0; i < SINE_SAMPLES; ++i) {
        int32_t y = sine_full(i) * (int32_t)percent / 100;
        fpga_write(DAC_RAM_INDEX + i, (uint16_t)(int16_t)y);
    }
    fpga_write(DAC_REG_INDEX(OFFS_B_PH_L), 0);
    fpga_write(DAC_REG_INDEX(OFFS_B_PH_H), 0);
    fpga_write(DAC_REG_INDEX(OFFS_DDS_L), (uint16_t)step);
    fpga_write(DAC_REG_INDEX(OFFS_DDS_H), (uint16_t)(step >> 16));
    fpga_write(DAC_REG_INDEX(OFFS_CONF), (uint16_t)((1u << B_ON_DDS) | (1u << B_EN_DDS)));
}

static void fill_status(ThkStatusBody* status)
{
    uint16_t low;
    uint16_t high;
    thk_bank_status(&g_bank, status);
    low = fpga_read(DAC_REG_INDEX(OFFS_TRIG_L));
    high = fpga_read(DAC_REG_INDEX(OFFS_TRIG_H));
    status->trigger_count = (uint32_t)low | ((uint32_t)high << 16);
    if (com_is_open())
        status->flags = (uint16_t)(status->flags | 1u);
}

static void com_log_bytes(const char* tag, const uint8_t* data, uint16_t len)
{
    uint16_t i;
    xil_printf("COM %s len=%u:", tag, (unsigned)len);
    for (i = 0; i < len; ++i)
        xil_printf(" %02x", (unsigned)data[i]);
    xil_printf(" |'");
    for (i = 0; i < len; ++i) {
        const uint8_t c = data[i];
        xil_printf("%c", (c >= 32u && c < 127u) ? (char)c : '.');
    }
    xil_printf("'\r\n");
}

static uint16_t seal_reply(uint16_t type, uint32_t sequence, const void* payload, uint16_t length, uint8_t* out,
                           uint16_t out_cap)
{
    ThkHeader header;
    if ((uint32_t)sizeof(header) + length > out_cap)
        return 0;
    memset(&header, 0, sizeof(header));
    header.signature = THK_SIGNATURE;
    header.version = THK_VERSION;
    header.type = type;
    header.sequence = sequence;
    header.length = length;
    memcpy(out, &header, sizeof(header));
    if (length)
        memcpy(out + sizeof(header), payload, length);
    thk_seal(out, sizeof(header) + length);
    return (uint16_t)(sizeof(header) + length);
}

/* STATUS payload: ThkStatusBody + uint16_t THK_RX_FW_VERSION (LE). */
static uint16_t seal_status(uint32_t sequence, uint8_t* out, uint16_t out_cap)
{
    ThkStatusBody status;
    uint8_t body[sizeof(ThkStatusBody) + sizeof(uint16_t)];
    uint16_t ver = (uint16_t)THK_RX_FW_VERSION;
    fill_status(&status);
    memcpy(body, &status, sizeof(status));
    memcpy(body + sizeof(status), &ver, sizeof(ver));
    return seal_reply(THK_STATUS, sequence, body, (uint16_t)sizeof(body), out, out_cap);
}

uint16_t thk_handle(const uint8_t* frame, uint16_t frame_len, uint8_t* out, uint16_t out_cap)
{
    ThkHeader header;
    const uint8_t* payload;
    if (frame_len < sizeof(header) || !thk_check(frame, frame_len))
        return 0;
    memcpy(&header, frame, sizeof(header));
    if ((uint32_t)sizeof(header) + header.length != frame_len)
        return 0;
    payload = frame + sizeof(header);
    if (header.type == THK_HELLO) {
        return seal_status(header.sequence, out, out_cap);
    }
    if (header.type == THK_WAVE) {
        ThkAckBody ack;
        uint16_t ack_status = thk_bank_wave(&g_bank, payload, header.length);
        if (ack_status == THK_ACK_OK && g_bank.complete)
            dac_store(g_bank.samples, g_bank.total);
        memset(&ack, 0, sizeof(ack));
        ack.ack_sequence = header.sequence;
        ack.status = ack_status;
        if (header.length >= sizeof(ThkWaveInfo)) {
            ThkWaveInfo info;
            memcpy(&info, payload, sizeof(info));
            ack.chunk_index = info.chunk_index;
        }
        return seal_reply(THK_ACK, header.sequence, &ack, sizeof(ack), out, out_cap);
    }
    if (header.type == THK_CTRL) {
        ThkCtrlBody command;
        if (header.length >= sizeof(command)) {
            memcpy(&command, payload, sizeof(command));
            if (command.command == THK_CTRL_STOP) {
                fpga_write(DAC_REG_INDEX(OFFS_CONF), 0);
            } else if (command.command == THK_CTRL_FREERUN) {
                uint32_t pause_us = command.period_us;
                if (pause_us > 65535u)
                    pause_us = 65535u;
                fpga_write(DAC_REG_INDEX(OFFS_PERIOD), (uint16_t)pause_us);
                fpga_write(DAC_REG_INDEX(OFFS_CONF), (uint16_t)(1u << B_FREERUN));
            } else if (command.command == THK_CTRL_ARM) {
                fpga_write(DAC_REG_INDEX(OFFS_DELAY), 0);
                fpga_write(DAC_REG_INDEX(OFFS_CONF), (uint16_t)(1u << B_EXT_SYNC));
            } else if (command.command == THK_CTRL_ONCE) {
                fpga_write(DAC_REG_INDEX(OFFS_DELAY), 0);
                fpga_write(DAC_REG_INDEX(OFFS_CONF), (uint16_t)((1u << B_EXT_SYNC) | (1u << B_ONCE)));
            } else if (command.command == THK_CTRL_SINE) {
                sine_start(command.period_us, command.reserved);
            } else if (command.command == THK_CTRL_ANALOG) {
                uint16_t amplitude = (uint16_t)command.period_us;
                if (mcp4725_write(amplitude, command.reserved) != 0)
                    g_bank.last_error = THK_ERR_I2C;
                else if (g_bank.last_error == THK_ERR_I2C)
                    g_bank.last_error = 0;
            }
        }
        return seal_status(header.sequence, out, out_cap);
    }
    if (header.type == THK_TEXT) {
        ThkAckBody ack;
        uint8_t body[sizeof(ThkAckBody) + 64];
        uint8_t rx[64];
        uint16_t rx_len = 0;
        uint16_t ack_status = THK_ACK_FORMAT;
        uint16_t body_len;
        uint16_t sealed;
        uint32_t baud = 0;
        uint16_t wait_ms = 100;
        uint16_t text_off = 4;
        const uint8_t* text;
        uint16_t text_len;
        memset(&ack, 0, sizeof(ack));
        memset(rx, 0, sizeof(rx));
        if (header.length >= 4u) {
            memcpy(&baud, payload, sizeof(baud));
            /* New ThkTextInfo is 8 bytes (baud + rx_timeout_ms + reserved). */
            if (header.length >= (uint16_t)sizeof(ThkTextInfo)) {
                ThkTextInfo info;
                memcpy(&info, payload, sizeof(info));
                baud = info.baud;
                if (info.rx_timeout_ms != 0)
                    wait_ms = info.rx_timeout_ms;
                text_off = (uint16_t)sizeof(ThkTextInfo);
            }
            if (wait_ms > 800u)
                wait_ms = 800u;
            text = payload + text_off;
            text_len = (uint16_t)(header.length - text_off);
            xil_printf("COM TEXT open=%d baud=%u wait=%u\r\n",
                       com_is_open(), (unsigned)baud, (unsigned)wait_ms);
            com_log_info();
            /* Drop stale UART bytes so Att "N\\r" is not mixed with noise. */
            {
                uint16_t drain = 0;
                com_read(rx, (uint16_t)sizeof(rx), &drain, 20);
                if (drain)
                    com_log_bytes("drain", rx, drain);
                memset(rx, 0, sizeof(rx));
            }
            com_log_bytes("tx", text, text_len);
            if (com_write(baud, text, text_len) == 0) {
                ack_status = THK_ACK_OK;
                /* Short turnaround only — do not purge RX after TX (would drop Att reply). */
                ehci_wait_ms(2);
                /* Att: "0\\r" -> "1\\r". Send COM uses short wait; Att probe asks ~500 ms. */
                com_read(rx, (uint16_t)sizeof(rx), &rx_len, wait_ms);
                com_log_bytes("rx", rx, rx_len);
            } else {
                ack_status = THK_ERR_COM;
                xil_printf("COM tx fail\r\n");
            }
        }
        ack.ack_sequence = header.sequence;
        ack.chunk_index = rx_len;
        ack.status = ack_status;
        memcpy(body, &ack, sizeof(ack));
        if (rx_len > 0)
            memcpy(body + sizeof(ack), rx, rx_len);
        body_len = (uint16_t)(sizeof(ack) + rx_len);
        sealed = seal_reply(THK_ACK, header.sequence, body, body_len, out, out_cap);
        if (sealed == 0) {
            /* Always return ACK even if RX tail does not fit the UDP reply buffer. */
            ack.chunk_index = 0;
            sealed = seal_reply(THK_ACK, header.sequence, &ack, (uint16_t)sizeof(ack), out, out_cap);
        }
        return sealed;
    }
    return 0;
}


int main(void)
{
    thk_bank_reset(&g_bank);
    xil_printf("thk_rx fw %u.%u\r\n",
               (unsigned)(THK_RX_FW_VERSION >> 8),
               (unsigned)(THK_RX_FW_VERSION & 0xffu));
    if (!dac_hw_init())
        g_bank.last_error = THK_ERR_DAC;
    (void)mcp4725_init();
    eth_run();
    return 0;
}
