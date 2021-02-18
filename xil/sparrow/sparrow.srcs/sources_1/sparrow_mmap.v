`ifndef __SPARROW_MMAP__
`define __SPARROW_MMAP__
///==================================
///=======  a[15:14]===========================
`define  ADDR_COMMON		     2'h0		
`define  ADDR_DAC_RAM 		   	 2'h1   ///A[15:14]
`define  ADDR_ADC_RAM 		   	 2'h2   ///A[15:14]
///============== ADDR_COMMON ===============
///=======  a[13:12]===========================
`define  ADDR_COMMON_REGS		 2'h0		
`define  ADDR_DAC_REGS 		   	 2'h1   ///
`define  ADDR_ADC_REGS 		   	 2'h2   ///
////`define  ADDR_SNIFF_REGS 		 2'h3   ///
///========ADDR_COMMON_REGS==============
///==============ug A[6:1]===============		
///`define  OFFS_CONF 		        6'h00		
///`define  OFFS_UPR 			    6'h01
///`define  OFFS_STAT 		        6'h02
///`define  OFFS_PSK               6'h03
///`define  OFFS_CLR_IRQ           6'h04
`define  OFFS_RD_C0 		    6'h05     /// for check connect a5a5
`define  OFFS_RD_C1 		    6'h06     /// for check connect
///=========== u_spi ===================
`define  OFFS_CS_USPI 	    	2'h01       ///a[6:5]
`define  OFFS_SPI_SPD 	    	6'h11
`define  OFFS_SPI_CS_A 	        6'h12       ///DAC
`define  OFFS_SPI_CS_B 	        6'h13       ///ADC
`define  OFFS_SPI_WR_DATA 	    6'h14
`define  OFFS_SPI_REQ_DATA 	    6'h15
`define  OFFS_SPI_RD_DATA 	    6'h16
`define  OFFS_SPI_CNF           6'h17
///=========== ADDR_DAC_REGS ===================
///==============ug A[6:1]===============		
`define  OFFS_CONF 		        6'h00		
///`define  OFFS_UPR 			    6'h01
`define  OFFS_DAC_OUT 		    6'h01
`define  OFFS_B_PH_L 		    6'h02
`define  OFFS_B_PH_H 		    6'h03
`define  OFFS_DDS_L 		    6'h04
`define  OFFS_DDS_H 		    6'h05
`define  OFFS_LEN_CNT_DAC 		6'h06

`define  DELAY_DAC_L            6'h08
`define  DELAY_DAC_H            6'h09
///========= OFFS_CONF DAC==========
`define  B_ON_TST               0   ///DAC_OUT -> DAC
`define  B_ON_DDS               1
`define  B_EN_DDS               2
`define  B_DDS_PH_DAT           3   ///=1 phasa, =0 dat
`define  B_EXT_SYNC             4   ///=1 ext sync
`define  B_EXT_FAL              5   ///=1 falling edge
///==============================
///=========== ADDR_ADC_REGS ===================
///==============ug A[6:1]===============		
`define  OFFS_ADC_CONF 		        6'h00
`define  OFFS_ADC_OUT 		        6'h01
`define  OFFS_LEN_CNT_ADC 		    6'h06
		
`define  OFFS_ADC_CHECK 		    6'h07     /// for check connect a3a3
`define  OFFS_DELAY_WDAT0            6'h08	
`define  OFFS_DELAY_WDAT1            6'h09	
`define  OFFS_DELAY_WDAT2            6'h0a	
`define  OFFS_DELAY_WDAT3            6'h0b	
`define  OFFS_DELAY_WDAT4            6'h0c	
`define  OFFS_DELAY_WDAT5            6'h0d	
`define  OFFS_DELAY_WDAT6            6'h0e
	
`define  OFFS_DELAY_RDAT0            6'h10	
`define  OFFS_DELAY_RDAT1            6'h11	
`define  OFFS_DELAY_RDAT2            6'h12	
`define  OFFS_DELAY_RDAT3            6'h13	
`define  OFFS_DELAY_RDAT4            6'h14	
`define  OFFS_DELAY_RDAT5            6'h15	
`define  OFFS_DELAY_RDAT6            6'h16	
///===========================================
///========= OFFS_CONF ADC==========
`define  B_ON_TST               0   ///DAC_OUT -> DAC
`define  B_EXT_SYNC             4   ///=1 ext sync
`define  B_EXT_FAL              5   ///=1 falling edge

///=======================================
`endif ///__SPARROW_MMAP__
