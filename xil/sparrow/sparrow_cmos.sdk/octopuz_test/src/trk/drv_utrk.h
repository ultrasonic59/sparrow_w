#ifndef __DRV_UTRK_H__
#define __DRV_UTRK_H__

#include "fpga_mmap.h"
///#include "drv_spi.h"
///#define	ADDR_DEF XPAR_M_AXI_GP0_BASEADDR

#define	VRCH_DELT	1
#define	MAX_KUS_DB      75
#define	NUM_POW         3        

///#define PC_POW_LOW      255
///#define PC_POW_MID      60
///#define PC_POW_HI       3

////#define  OFFS_BEG_RZV_B 		0x2B
////#define  OFFS_OSC_STP_B 		0x2C

#define VREJ_RADIO		0
#define VREJ_VIDEO		1
#define VREJ_REZ		2


///conf_adc.0 	adc s1
///conf_adc.1	  adc s2
///#define STANDBY_A_B	        0x0
///#define NORM_OP			0x1
///#define STANDBY_B		0x2
///#define DATA_ALIGN	        0x3

#define DEF_LEN_OSC		256
#define NUM_SUMM_OSC    2

///====== ugen ===================

///#define ADDR_UG						(ADDR_DEF|0x2000)

#define MASK_ON_GEN 	0xff	///
///#define MAX_NUM_GEN 	0x8	///
#define MASK_TACT 		0x7	///
#define SH_TACT_UG 		5	///
///============ uu =====================
#define SH_TACT_UU_PAR 		6	///

///#define ADDR_UU						(ADDR_DEF|0x4000)

#define  SH_NUS 								(11)
#define  MASK_NUS 							0x1


///=====================================
////#define  DEF_SPD_PSPI	 0x18

extern uint8_t osc_rez[];
extern uint8_t cur_ngen ;
extern uint8_t cur_nus ;

///extern struct rt_spi_device *utrk_spi_device;
////extern int utrk_init(void);
extern void psk_trk(void);
////extern void spi_set_addr(uint16_t i_addr);
///extern uint16_t spi_get_addr(void);
///extern void spi_set_dat(uint16_t i_dat);
///extern uint16_t spi_get_dat(void);
extern uint16_t rd_dat_trk(uint16_t addr);
extern uint32_t rd_dat32_trk(uint16_t addr);
extern void rd_dat_blok_trk(uint16_t addr,uint16_t len,uint16_t *obuf);
extern void rd_osc(uint16_t addr_osc,uint16_t len,uint8_t *obuf);


extern void wr_dat_trk(uint16_t addr,uint16_t dat);
extern void wr_dat32_trk(uint16_t addr,uint32_t dat);
extern void wr_dat_blok_trk(uint16_t addr,uint16_t len,uint16_t *ibuf);

///extern void rd_osc(uint16_t addr_osc,uint16_t len,uint8_t *obuf);

////extern void on_off_5v(uint8_t on_off);
extern void set_kus_tact(uint8_t i_tact,uint8_t i_num,uint8_t i_dat);
extern void off_us_tact(uint8_t i_tact,uint8_t i_num);

extern uint8_t get_kus_tact(uint8_t i_tact,uint8_t i_num);
extern void set_flgs1_tact(uint8_t i_tact,uint8_t i_num,uint16_t i_dat);
extern uint16_t get_flgs1_tact(uint8_t i_tact,uint8_t i_num);
extern void set_flgs2_tact(uint8_t i_tact,uint8_t i_num,uint16_t i_dat);
extern uint16_t get_flgs2_tact(uint8_t i_tact,uint8_t i_num);
extern uint8_t check_ready_dat(void);
extern void set_adc_clk(void);
extern void set_max_summ(uint8_t i_izm);
extern void set_all_pow(void);

///=======================================
#endif

