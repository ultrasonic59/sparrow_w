#include "xparameters.h"
#include "tx_api.h"

#define GIC_CPU   XPAR_PS7_SCUGIC_0_BASEADDR
#define GIC_DIST  XPAR_PS7_SCUGIC_0_DIST_BASEADDR
#define TIMER     XPAR_PS7_SCUTIMER_0_BASEADDR
#define TICK_ID   29U

#define REG32(addr) (*(volatile unsigned int *)(addr))

void tx_zynq_tick_init(void)
{
    unsigned int load;
    unsigned int priority;
    unsigned int shift;

    load = (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2U / TX_TIMER_TICKS_PER_SECOND) - 1U;

    REG32(TIMER + 0x08) = 0;
    REG32(TIMER + 0x00) = load;
    REG32(TIMER + 0x0C) = 1;

    REG32(GIC_DIST + 0x000) = 0;

    shift = (TICK_ID & 3U) * 8U;
    priority = REG32(GIC_DIST + 0x400 + (TICK_ID & ~3U));
    priority &= ~(0xFFU << shift);
    priority |= 0xA0U << shift;
    REG32(GIC_DIST + 0x400 + (TICK_ID & ~3U)) = priority;
    REG32(GIC_DIST + 0x100) |= 1U << TICK_ID;

    REG32(GIC_DIST + 0x000) = 1;
    REG32(GIC_CPU + 0x04) = 0xFF;
    REG32(GIC_CPU + 0x00) = 1;

    REG32(TIMER + 0x08) = 0x07;
}

unsigned int tx_zynq_irq_ack(void)
{
    return REG32(GIC_CPU + 0x0C);
}

void tx_zynq_irq_eoi(unsigned int id)
{
    REG32(GIC_CPU + 0x10) = id;
}

void tx_zynq_timer_clear(void)
{
    REG32(TIMER + 0x0C) = 1;
}
