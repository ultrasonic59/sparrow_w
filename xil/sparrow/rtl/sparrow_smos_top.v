//Copyright 1986-2014 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2014.4 (win64) Build 1071353 Tue Nov 18 18:29:27 MST 2014
//Date        : Sun Apr 03 20:56:37 2016
//Host        : vova-PC running 64-bit Service Pack 1  (build 7601)
//Command     : generate_target system_wrapper.bd
//Design      : system_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`include "sparrow_mmap.v"

`timescale 1 ps / 1 ps

module sparrow_smos_top
   (
   DDR_addr,
    DDR_ba,
    DDR_cas_n,
    DDR_ck_n,
    DDR_ck_p,
    DDR_cke,
    DDR_cs_n,
    DDR_dm,
    DDR_dq,
    DDR_dqs_n,
    DDR_dqs_p,
    DDR_odt,
    DDR_ras_n,
    DDR_reset_n,
    DDR_we_n,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb
//=======uart ==============
  ,i_clk24
 ///   ,o_txd_c
 ///   ,i_rxd_c
///======ext io====================
    ,io_ext
///======adc====================
   ,i_adc_dat
   ,i_adc_clko
   ,o_adc_clk_p
   ,o_adc_clk_n
    ,i_zzz
///======dac====================
    ,o_dac_clk_p
    ,o_dac_clk_m
    ,o_dac_dclk_p
    ,o_dac_dclk_m
    ,o_dac_dat_p
    ,o_dac_dat_m
 ///======serial dac adc====================
    ,o_adc_dac_sck
   ,io_adc_dac_sdio
    ,o_dac_en
    ,o_adc_en
    ,i_adc_sdout
   ,i_sync
////   ,o_zz
 ///,i_zz
 );
  inout [14:0]DDR_addr;
  inout [2:0]DDR_ba;
  inout DDR_cas_n;
  inout DDR_ck_n;
  inout DDR_ck_p;
  inout DDR_cke;
  inout DDR_cs_n;
  inout [1:0]DDR_dm;
  inout [15:0]DDR_dq;
  inout [1:0]DDR_dqs_n;
  inout [1:0]DDR_dqs_p;
  inout DDR_odt;
  inout DDR_ras_n;
  inout DDR_reset_n;
  inout DDR_we_n;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [31:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
input i_clk24;
///===================================
///======ext io====================
inout [10:0]io_ext;
///======adc====================
input [13:0]i_adc_dat;
input i_adc_clko;
output o_adc_clk_p;
output o_adc_clk_n;
input i_zzz;
///======dac====================
output o_dac_clk_p;
output o_dac_clk_m;
output o_dac_dclk_p;
output o_dac_dclk_m;
output [6:0]o_dac_dat_p;
output [6:0]o_dac_dat_m;
 ///======serial dac adc====================
 output o_adc_dac_sck;
 inout io_adc_dac_sdio;
 output o_dac_en;
 output o_adc_en;
input i_adc_sdout;
 input  i_sync;
///=======================================
////output o_zz;
///================================
  wire [14:0]DDR_addr;
  wire [2:0]DDR_ba;
  wire DDR_cas_n;
  wire DDR_ck_n;
  wire DDR_ck_p;
  wire DDR_cke;
  wire DDR_cs_n;
  wire [1:0]DDR_dm;
  wire [15:0]DDR_dq;
  wire [1:0]DDR_dqs_n;
  wire [1:0]DDR_dqs_p;
  wire DDR_odt;
  wire DDR_ras_n;
  wire DDR_reset_n;
  wire DDR_we_n;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [31:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire [3:0]fclk;
///=======================================
///wire o_txd_c;

wire o_clr_n;
///================================
reg [3:0]tst_clk;
wire ug_irq;
///wire o_zz;
///=======================================
wire             ps_sys_clk         ;
wire  [ 31: 0] ps_sys_addr        ;
///(* mark_debug = "true" *) 
wire  [ 31: 0] ps_sys_wdata       ;
///(* mark_debug = "true" *) 
wire  [  3: 0] ps_sys_sel         ;
///(* mark_debug = "true" *) 
wire             ps_sys_wen         ;
///(* mark_debug = "true" *) 
wire             ps_sys_ren         ;
///(* mark_debug = "true" *) 
wire  [ 31: 0] ps_sys_rdata       ;
///wire             ps_sys_err         ;
///wire             ps_sys_ack         ;
///(* mark_debug = "true" *) 
wire             ps_sys_cs         ;

///=========================================================
sparrow_ps_v i_ps
(
  .FIXED_IO_mio       (  FIXED_IO_mio                ),
  .FIXED_IO_ps_clk    (  FIXED_IO_ps_clk             ),
  .FIXED_IO_ps_porb   (  FIXED_IO_ps_porb            ),
  .FIXED_IO_ps_srstb  (  FIXED_IO_ps_srstb           ),
  .FIXED_IO_ddr_vrn   (  FIXED_IO_ddr_vrn            ),
  .FIXED_IO_ddr_vrp   (  FIXED_IO_ddr_vrp            ),
  .DDR_addr           (  DDR_addr                    ),
  .DDR_ba             (  DDR_ba                      ),
  .DDR_cas_n          (  DDR_cas_n                   ),
  .DDR_ck_n           (  DDR_ck_n                    ),
  .DDR_ck_p           (  DDR_ck_p                    ),
  .DDR_cke            (  DDR_cke                     ),
  .DDR_cs_n           (  DDR_cs_n                    ),
  .DDR_dm             (  DDR_dm                      ),
  .DDR_dq             (  DDR_dq                      ),
  .DDR_dqs_n          (  DDR_dqs_n                   ),
  .DDR_dqs_p          (  DDR_dqs_p                   ),
  .DDR_odt            (  DDR_odt                     ),
  .DDR_ras_n          (  DDR_ras_n                   ),
  .DDR_reset_n        (  DDR_reset_n                 ),
  .DDR_we_n           (  DDR_we_n                    ),

  .fclk_o             (  fclk               ),
  .rstn_o             (  o_clr_n              ),
  .i_irq_cpu(ug_irq),
   // system read/write channel
 .sys_clk_o       (  ps_sys_clk         ),  // system clock
 //// .sys_rstn_o      (  ps_sys_rstn        ),  // system reset - active low
  .sys_addr_o      (  ps_sys_addr        ),  // system read/write address
  .sys_wdata_o     (  ps_sys_wdata       ),  // system write data
  .sys_sel_o       (  ps_sys_sel         ),  // system write byte select
  .sys_wen_o       (  ps_sys_wen         ),  // system write enable
  .sys_ren_o       (  ps_sys_ren         ),  // system read enable
  .sys_rdata_i     (  ps_sys_rdata       ),  // system read data
 //// .sys_err_i       (  ps_sys_err         ),  // system error indicator
 ////.sys_ack_i       (  ps_sys_ack         ),  // system acknowledge signal
  .sys_cs_o         (ps_sys_cs)///,
   // SPI master
///  .spi0_ss_o        (                     ),  // select slave 0
///  .spi0_sck         (                     ),  // serial clock
 /// .spi0_mosi        (                     ),  // master out slave in
 /// .spi0_miso        (                     ),
 /// .uart0_rxd(i_rxd_c),
 /// .uart0_txd(o_txd_c)
);
//// assign o_ncs = !(ps_sys_addr[31:16]==16'h43c0);///|o_addr[15:8];///o_data[0];///o_addr[0];///o_data[0];///o_addr[1];///o_nwr;///tst_clk;////o_ncs;///tst_clk;////i_zz;
wire _clk200;
wire _clk80;
wire _clk160;
wire clk96;
////tri [11:0]io_ext;
tri [10:0]io_ext;

wire clk200;
wire clk80;
assign clk200=_clk200;
assign clk80=_clk80;

pll24_80 pll1
 (
  // Clock out ports
 .clk_out1(_clk80),
 .clk_out2(_clk160),
 .clk_out3(_clk200),
 .clk_out4(clk96),
  // Status and control signals
  .reset(1'b0),
  .locked(),
 // Clock in ports
 .clk_in1(i_clk24)
 );
///==================================== 
/*
ila_0 ila0(
 .clk(clk80),
  .probe0(ps_sys_addr),
 .probe1(ps_sys_wdata),
 .probe2(ps_sys_sel),
 .probe3(ps_sys_wen),
 .probe4(ps_sys_ren),
 .probe5(ps_sys_rdata),
 .probe6(ps_sys_cs)
 );
 */
///==================================== 
wire o_adc_dac_sck;
tri io_adc_dac_sdio;
wire o_dac_en;
wire o_adc_en;

////assign o_zz=o_adc_en;

///==========================================
/*
reg tst_clk;

///always @(posedge i_clk24) 
always @(posedge clk80) 
begin
tst_clk <= ~tst_clk;
end
*/
////assign ps_sys_err=1'b0;
////assign ps_sys_ack=1'b1;

reg  [ 31: 0] rdata       ;
assign ps_sys_rdata= rdata;
///=====================================================
///=======================================
wire t_izl;
assign	t_izl = i_sync;

wire clr;
assign clr= ~o_clr_n;
wire OnTimeCnt;
assign	OnTimeCnt =1'b1;
wire [17:0]cnt_time;

sepia_time utime_(.i_clk(clk80),.i_clr(clr),.i_psk(t_izl)
						,.i_onCnt(OnTimeCnt),.o_cnt_time(cnt_time),.tst());
///=======================================
///======dac====================
wire o_dac_clk_p;
wire o_dac_clk_m;
wire o_dac_sync_p;
wire o_dac_sync_m;
wire o_dac_dclk_p;
wire o_dac_dclk_m;
wire [6:0]o_dac_dat_p;
wire [6:0]o_dac_dat_m;
wire i_sync;
wire [3:0]tst_udac;
wire [15:0]udac_odata;

wire cs_udac;
assign cs_udac=((ps_sys_addr[15:14]== `ADDR_DAC_RAM)|| ((ps_sys_addr[15:14]== `ADDR_COMMON)
                &(ps_sys_addr[13:12]== `ADDR_DAC_REGS)))
                 &(ps_sys_cs);                      
u_dac u_dac_(.i_ps_clk(ps_sys_clk)
            ,.i_clk(_clk160),.i_clr(clr)
 			,.i_cs(cs_udac),.i_we(ps_sys_wen),.i_addr(ps_sys_addr)
			,.i_data(ps_sys_wdata[15:0]),.o_data(udac_odata)
           
            ,.o_dac_clk_p(o_dac_clk_p),.o_dac_clk_m(o_dac_clk_m)
            ,.o_dac_dclk_p(o_dac_dclk_p),.o_dac_dclk_m(o_dac_dclk_m)
           ,.o_dac_dat_p(o_dac_dat_p),.o_dac_dat_m(o_dac_dat_m)
           ,.i_sync(i_sync)
		   ,.tst(tst_udac));
///======adc====================
wire o_adc_clk_p;
wire o_adc_clk_n;
wire [15:0]uadc_odata;

wire cs_uadc;
assign cs_uadc= ((ps_sys_addr[15:14]== `ADDR_ADC_RAM)|| ((ps_sys_addr[15:14]== `ADDR_COMMON)
                &(ps_sys_addr[13:12]== `ADDR_ADC_REGS)))
                 &(ps_sys_cs);                      

u_adc_cmos u_adc_(.i_ps_clk(ps_sys_clk),.i_clk(clk80),.i_clr(clr)
            ,.i_delay_clk(fclk[1])
  			,.i_cs(cs_uadc),.i_we(ps_sys_wen),.i_addr(ps_sys_addr)
			,.i_data(ps_sys_wdata[15:0]),.o_data(uadc_odata)
             ,.i_adc_clk(i_adc_clko)
             ,.i_data_ad(i_adc_dat)
            ,.o_adc_clk_p(o_adc_clk_p),.o_adc_clk_n(o_adc_clk_n)
           ,.i_sync(i_sync)
		      ,.tst());
		      
///========================================
wire [3:0]uspi_tst;
wire [15:0]uspi_odata;
wire [1:0]o_cs_spi;
wire o_sck_spi;
wire cs_uspi;
assign cs_uspi= (ps_sys_addr[15:14]== `ADDR_COMMON)
                &(ps_sys_addr[13:12]== `ADDR_COMMON_REGS)
                &(ps_sys_addr[6:5]== `OFFS_CS_USPI)
                &(ps_sys_cs);                      
u_spi u_spi_(.i_clk(ps_sys_clk),.i_clr(clr)	
			,.i_cs(cs_uspi),.i_we(ps_sys_wen),.i_addr(ps_sys_addr[6:1])
			,.i_data(ps_sys_wdata[15:0]),.o_data(uspi_odata)
			///=== spi ======
			,.o_cs_spi(o_cs_spi),.i_sdat(i_adc_sdout)
			,.io_sdat(io_adc_dac_sdio),.o_sck(o_sck_spi),.o_busy()
			///======= test ================
			,.tst(uspi_tst)
				);
assign o_adc_dac_sck=o_sck_spi;	
assign o_dac_en=~o_cs_spi[0];
assign o_adc_en=~o_cs_spi[1];	
///======================================= 
/*
ila_0 ila0(
 .clk(clk80),
  .probe0(ps_sys_addr),
 .probe1(ps_sys_wdata),
 .probe2(ps_sys_sel),
 .probe3(ps_sys_wen),
 .probe4(ps_sys_ren),
 .probe5(ps_sys_rdata),
 .probe6(ps_sys_cs),
 .probe7(cs_uspi)
 );
*/
    
//++++++++++++++ test ++++++++++++++++
wire [15:0]odat_us;
///=====================================
always @*
case (ps_sys_addr[15:14])
    `ADDR_DAC_RAM:	
	  rdata[15:0]<=udac_odata[15:0];						// 
    `ADDR_ADC_RAM:	
	  rdata[15:0]<=uadc_odata[15:0];						// 
    `ADDR_COMMON:
        case (ps_sys_addr[13:12])
        `ADDR_DAC_REGS:
	       rdata[15:0]<=udac_odata[15:0];						// 
        `ADDR_ADC_REGS:
	       rdata[15:0]<=uadc_odata[15:0];						// 
        `ADDR_COMMON_REGS:
            case (ps_sys_addr[6:5])
            `OFFS_CS_USPI:
	               rdata[15:0]<=uspi_odata;	
                default:
    	           case (ps_sys_addr[6:1])
                    `OFFS_RD_C0:
	                   rdata[15:0]<=16'ha5a5;	
                    `OFFS_RD_C1:
	                   rdata[15:0]<=16'h5a5a;	
                    default:
	                   rdata[15:0]<=16'h7654;	
	           endcase
            endcase
          endcase  
	default:
	  rdata[15:0]<=odat_us[15:0];						// 
///	  rdata[15:0]<=cnt_time[17:2];						// 
endcase
///==========================================

///reg [3:0]tst_clk;
///always @(posedge fclk[0])
always @(posedge clk96)
tst_clk<=tst_clk+1'b1;

wire [3:0]t_ext;
///assign o_ext=uspi_tst;		      

assign t_ext[0]=clk80;		      
assign t_ext[1]=tst_clk[0];		      
assign t_ext[2]=tst_clk[1];		      
assign t_ext[3]=tst_clk[2];		      
///=====================================================
endmodule
