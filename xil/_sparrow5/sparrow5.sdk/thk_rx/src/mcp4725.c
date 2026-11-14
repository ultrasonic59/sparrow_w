#include "mcp4725.h"

#include "xparameters.h"
#include "xstatus.h"
#include "xiicps.h"

#define MCP4725_ADDR_AMPLITUDE 0x60u
#define MCP4725_ADDR_OFFSET 0x61u
#define MCP4725_WRITE_DAC 0x40u

static XIicPs g_iic;
static int g_ready;

static uint16_t clamp_code(uint16_t code)
{
    return code > 4095u ? 4095u : code;
}

static int write_code(uint16_t addr, uint16_t code)
{
    u8 buf[3];
    code = clamp_code(code);
    buf[0] = MCP4725_WRITE_DAC;
    buf[1] = (u8)((code >> 4) & 0xffu);
    buf[2] = (u8)((code << 4) & 0xf0u);
    if (XIicPs_MasterSendPolled(&g_iic, buf, 3, addr) != XST_SUCCESS)
        return -1;
    return 0;
}

int mcp4725_init(void)
{
    XIicPs_Config* cfg = XIicPs_LookupConfig(XPAR_XIICPS_0_DEVICE_ID);
    g_ready = 0;
    if (!cfg)
        return -1;
    if (XIicPs_CfgInitialize(&g_iic, cfg, cfg->BaseAddress) != XST_SUCCESS)
        return -1;
    if (XIicPs_SetSClk(&g_iic, 100000) != XST_SUCCESS)
        return -1;
    g_ready = 1;
    return 0;
}

int mcp4725_write(uint16_t amplitude, uint16_t offset)
{
    int amp_ok;
    int off_ok;
    if (!g_ready)
        return -1;
    amp_ok = write_code(MCP4725_ADDR_AMPLITUDE, amplitude);
    off_ok = write_code(MCP4725_ADDR_OFFSET, offset);
    return (amp_ok == 0 && off_ok == 0) ? 0 : -1;
}
