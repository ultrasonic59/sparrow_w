#ifndef __FPGA_MMAP_H__
#define __FPGA_MMAP_H__

#define	ADDR_DEF XPAR_M_AXI_GP0_BASEADDR

#define ADDR_UG				0x2000
#define ADDR_UG_PAR			(ADDR_UG|0x1<<8)
#define ADDR_UG_SREGS		ADDR_UG
#define ADDR_UG_CONF		ADDR_UG_SREGS+0x0
#define ADDR_UG_UPR			ADDR_UG_SREGS+0x1
#define ADDR_UG_STAT		ADDR_UG_SREGS+0x2
#define ADDR_UG_PSK			ADDR_UG_SREGS+0x3
#define ADDR_UG_CLR_IRQ		ADDR_UG_SREGS+0x4
#define ADDR_RD_C0			ADDR_UG_SREGS+0x5
#define ADDR_RD_C1			ADDR_UG_SREGS+0x6
#define ADDR_SPD_PRDAC		ADDR_UG_SREGS+0x7

///#define  ADDR_POW_GEN			ADDR_UG_SREGS+0x40
#define  ADDR_MAX_TACT		ADDR_UG_SREGS+0x8
#define  ADDR_NUM_SUMM		ADDR_UG_SREGS+0x9
#define  ADDR_NUM_IZM		ADDR_UG_SREGS+0xa
///#define  ADDR_RD_C0				ADDR_UG_SREGS+0x8
///#define  ADDR_RD_C1				ADDR_UG_SREGS+0x9
#define  ADDR_POW_GEN			ADDR_UG_SREGS+0x20
///================ ug_conf =================
#define  B_ON_IRQ			(0x1<<0)
#define  B_ENA_OFF18V		(0x1<<1)


///============= ug ram param =================
///#define OFFS_ON_GEN 			0x2			///
#define OFFS_MUX_PREV		0x0	///
#define OFFS_LEN_IMP0G 		0x1	///
#define OFFS_LEN_IMP1G 		0x2	///
#define OFFS_LEN_IMP2G 		0x3	///
#define OFFS_LEN_IMP3G 		0x4	///
#define OFFS_LEN_IMP4G 		0x5	///
#define OFFS_LEN_IMP5G 		0x6	///
#define OFFS_LEN_IMP6G 		0x7	///
#define OFFS_LEN_IMP7G 		0x8	///
#define OFFS_ON_GEN 		0x9	///
#define OFFS_HI_POW 		0xa	///
#define OFFS_LEN_TACT 		0xe	///
///============== UU ===========================
#define ADDR_UU			0x4000
#define ADDR_OSC_A		(ADDR_UU+ (0x1<<10))
#define ADDR_OSC_AV		(ADDR_UU+ (0x3<<10))  ///

///======sregs ==============================
#define  OFFS_CONF_L 				0x0
#define  OFFS_UPR_U 				0x1
#define  OFFS_NUS_OSC_A  			0x2
#define  OFFS_NTACT_OSC_A  			0x3
#define  OFFS_NUS_OSC_B  			0x4
#define  OFFS_NTACT_OSC_B  			0x5

#define  OFFS_CNF_ADC 				0x06
#define  OFFS_UPR_TRK 				0x10
#define  OFFS_STAT_TRK 				0x11

#define  OFFS_BEG_RZV_A 			0x14
#define  OFFS_OSC_STP_A 			0x15
#define  OFFS_BEG_RZV_B 			0x16
#define  OFFS_OSC_STP_B 			0x17

#define  ADDR_UPR_UU		(ADDR_UU+OFFS_UPR_U)
#define  ADDR_NUS_OSC_A  	(ADDR_UU+OFFS_NUS_OSC_A)
#define  ADDR_NTACT_OSC_A  	(ADDR_UU+OFFS_NTACT_OSC_A)
#define  ADDR_NUS_OSC_B  	(ADDR_UU+OFFS_NUS_OSC_B)
#define  ADDR_NTACT_OSC_B  	(ADDR_UU+OFFS_NTACT_OSC_B)
#define  ADDR_CNF_ADC  		(ADDR_UU+OFFS_CNF_ADC)

#define  ADDR_BEG_RZV_A 	(ADDR_UU+OFFS_BEG_RZV_A)
#define  ADDR_OSC_STP_A 	(ADDR_UU+OFFS_OSC_STP_A)
#define  ADDR_BEG_RZV_B 	(ADDR_UU+OFFS_BEG_RZV_B)
#define  ADDR_OSC_STP_B 	(ADDR_UU+OFFS_OSC_STP_B)


///===========Chans =========================
#define ADDR_UU_CHANS				(ADDR_UU|(0x1<<13))
#define ADDR_UU_CHAN0				(ADDR_UU_CHANS|(0x0<<12))
#define ADDR_UU_CHAN1				(ADDR_UU_CHANS|(0x1<<12))

#define OFFS_UU_PAR				(0x0<<11)
#define OFFS_UU_REZ				(0x1<<11)
#define ADDR_UU_PAR_CHAN(cc,tact)	(ADDR_UU_CHANS|(cc&0x1)<<12|OFFS_UU_PAR|(tact&MASK_TACT)<<SH_TACT_UU_PAR)
#define ADDR_UU_REZ_CHAN(cc)	(ADDR_UU_CHANS|(cc&0x1)<<12|OFFS_UU_REZ)
#define  OFFS_VRCH 								(0)
#define  LEN_DAT_VRCH							16
///=========== ram param ==============================
#define  OFFS_FLGS1								(OFFS_VRCH+LEN_DAT_VRCH+0)
#define  OFFS_FLGS2								(OFFS_VRCH+LEN_DAT_VRCH+1)
#define  OFFS_KUS								(OFFS_VRCH+LEN_DAT_VRCH+2)
#define  OFFS_BEGR								(OFFS_VRCH+LEN_DAT_VRCH+3)
#define  OFFS_POR0								(OFFS_VRCH+LEN_DAT_VRCH+4)
#define  OFFS_BSTR0								(OFFS_VRCH+LEN_DAT_VRCH+5)
#define  OFFS_LSTR0								(OFFS_VRCH+LEN_DAT_VRCH+6)
#define  OFFS_POR1								(OFFS_VRCH+LEN_DAT_VRCH+7)
#define  OFFS_BSTR1								(OFFS_VRCH+LEN_DAT_VRCH+8)
#define  OFFS_LSTR1								(OFFS_VRCH+LEN_DAT_VRCH+9)
#define  OFFS_POR2								(OFFS_VRCH+LEN_DAT_VRCH+10)
#define  OFFS_BSTR2								(OFFS_VRCH+LEN_DAT_VRCH+11)
#define  OFFS_LSTR2								(OFFS_VRCH+LEN_DAT_VRCH+12)
#define  OFFS_POR3								(OFFS_VRCH+LEN_DAT_VRCH+13)
#define  OFFS_BSTR3								(OFFS_VRCH+LEN_DAT_VRCH+14)
#define  OFFS_LSTR3								(OFFS_VRCH+LEN_DAT_VRCH+15)
#define  OFFS_POR4								(OFFS_VRCH+LEN_DAT_VRCH+16)
#define  OFFS_BSTR4								(OFFS_VRCH+LEN_DAT_VRCH+17)
#define  OFFS_LSTR4								(OFFS_VRCH+LEN_DAT_VRCH+18)
#define  OFFS_DELAY								(OFFS_VRCH+LEN_DAT_VRCH+19)
#define  OFFS_DELAY_V							(OFFS_VRCH+LEN_DAT_VRCH+20)

///====== FLGS1 ====================
/// .7:.0			res_stp
/// .10:.8    num_p_vrch
/// .11 			on_us
/// .12				on_vrch
///	.13				on_stp
///	.14				rej_tact 0 -amp_tim ,1 - razv A
#define  OFFS_B_ON_US			11
#define  OFFS_B_ON_VRCH			12
#define  OFFS_B_REJ_TACT		14

///#define ADDR_UPR_TRK		0x0
////#define ADDR_CONF_ADC		0x1
///#define ADDR_UPR_BT		0x2
///#define ADDR_ON_VDD_SD	        0x20
///#define  ADDR_POW_VOL		0x21

///#define ADDR_CONF_ADC		0x30
///#define ADDR_ON_CLK_ADC		0x31
///#define ADDR_INV_CLK_ADC	0x32

///#define ADDR_UPR_TRK		0x10

///=====================================
#endif ///__FPGA_MMAP_H__
