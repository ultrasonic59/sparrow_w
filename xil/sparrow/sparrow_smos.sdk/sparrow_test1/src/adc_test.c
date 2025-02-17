#include "xparameters.h"
#include "sleep.h"
#include "xiicps.h"
#include "xgpiops.h"
#include "xil_printf.h"
#include "xplatform_info.h"
#include "sparrow_fpga_mm.h"
///==========================================================
#define MAX_READY_CNT 2000

/// #define DELAY_SPI_ADDA 10000
extern void put_rst_adc(uint8_t on_off);
extern void wr_dat_trk(uint16_t addr,uint16_t dat);
extern uint16_t rd_dat_trk(uint16_t addr);

///==========================================================
#define rd_dat_fpga rd_dat_trk
#define wr_dat_fpga wr_dat_trk
///==========================================================
void reset_adc(void)
{
put_rst_adc(0x1);
usleep(20);
put_rst_adc(0x0);
}
uint16_t rd_data_ad(void)
{
return rd_dat_fpga(ADDR_ADC_OUT);
}

int wait_ready_spi_adda(void)
{
int cnt_ready=0;
uint16_t t_dat;
usleep(4);

for(cnt_ready=0;cnt_ready<MAX_READY_CNT;cnt_ready++)
	{
	t_dat=rd_dat_fpga(ADDR_SPI_RDY);
	if((t_dat&SPI_BUSY_BIT)==0)
		break;
	else
		{
		;////xil_printf(" \n\r [mod]wait_ready[%x] ",t_dat );
		}
	}
if(cnt_ready<MAX_READY_CNT)
	return 0;
else
	return -1;
}

///==========================================================
///===========================================
int mod_put_spi_ad(u8 i_addr,u8 i_data)
{
int rdy;
int nn=0;
wr_dat_trk(ADDR_CNF_SPI,SPI_INV_SCK);

rdy=wait_ready_spi_adda();
if(rdy!=0)
	{
	goto ret;
///	return rdy;
	}
wr_dat_fpga(ADDR_SPI_CS_B,0x1); ///ncs=0
wr_dat_fpga(ADDR_SPI_WR_DATA,i_addr);
nn++;
rdy=wait_ready_spi_adda();
if(rdy!=0)
	{
	wr_dat_fpga(ADDR_SPI_CS_B,0x0);
	goto ret;
///	return rdy;
	}
wr_dat_fpga(ADDR_SPI_WR_DATA,i_data);
rdy=wait_ready_spi_adda();
wr_dat_fpga(ADDR_SPI_CS_B,0x0);
nn++;
ret:
////	xil_printf("\n\r[mod]mod_put_spi_ad[[%d]:%x:%x[%d]]===",nn,i_addr,i_data,rdy );
return rdy;
}
int mod_wr_spi_ad(u8 i_addr,u8 i_data)
{
int rdy;
///rdy=mod_put_spi_ad(0x0,0x0);///clr readout
///if(rdy!=0)
///	return rdy;
rdy=mod_put_spi_ad(i_addr,i_data);
return rdy;

}

int mod_rd_spi_ad(u8 i_addr,u8 *odata )
{
int rdy;
u16 _tst;
////rdy=mod_put_spi_ad(0x0,0x1);///set readout
////if(rdy!=0)
////	return rdy;
rdy=mod_put_spi_ad(i_addr,0xff);
if(rdy!=0)
	return rdy;
_tst=rd_dat_fpga(ADDR_SPI_RD_DATA);

/////rdy=mod_put_spi_ad(0x0,0x0);///clr readout

*odata=_tst&0xff;
return rdy;
}
///================================================================
void init_adc(void)
{
reset_adc();
mod_wr_spi_ad(0x3,0x3);///hi perf mode 1

mod_wr_spi_ad(0x3d,(0x3<<6));
////mod_wr_spi_ad(0x41,(0x1<<6));///ddr lvds
mod_wr_spi_ad(0x41,(0x3<<6));///cmos
mod_wr_spi_ad(0x42,(0x1<<3)); ///for test pattern
///mod_wr_spi_ad(0x43,0x3);		///en lvds swing

////mod_wr_spi_ad(0xdf,(0x3<<4)); ///low speed

////mod_wr_spi_ad(0x0,0x1); ///ena read
}

