`ifndef __SPARROW5_MMAP__
`define __SPARROW5_MMAP__
///==================================
`define  VERS0    16'h2026
`define  VERS1    16'h1903

///=======  a[15:14]===========================
`define  ADDR_COMMON		     2'h0		
`define  ADDR_DAC_RAM 		   	 2'h1   ///A[15:14]
///============== ADDR_COMMON ===============
///=======  a[13:12]===========================
`define  ADDR_COMMON_REGS		 2'h0		
`define  ADDR_DAC_REGS 		   	 2'h1   ///
///========ADDR_COMMON_REGS==============
///==============ug A[6:1]===============		
`define  OFFS_VERS0 		        6'h00		
`define  OFFS_VERS1			        6'h01
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
`define  OFFS_DELAY_CNT_DAC 	6'h07

///========= OFFS_CONF DAC==========
`define  B_ON_TST               0   ///DAC_OUT -> DAC
`define  B_ON_DDS               1
`define  B_EN_DDS               2
`define  B_DDS_PH_DAT           3   ///=1 phasa, =0 dat
`define  B_EXT_SYNC             4   ///=1 ext sync
`define  B_EXT_FAL              5   ///=1 falling edge
///==============================
///=======================================
`endif ///__SPARROW5_MMAP__
