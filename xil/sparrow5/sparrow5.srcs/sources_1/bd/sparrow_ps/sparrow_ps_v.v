//Copyright 1986-2014 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2014.4 (win64) Build 1071353 Tue Nov 18 18:29:27 MST 2014
//Date        : Sun Apr 03 20:56:37 2016
//Host        : vova-PC running 64-bit Service Pack 1  (build 7601)
//Command     : generate_target system_wrapper.bd
//Design      : system_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module sparrow_ps_v
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
    FIXED_IO_ps_srstb,
 ///   ,
 ///======= Spi0 ===========================
 ///   spi0_miso,
///    spi0_mosi,
///    spi0_sck,
/////spi0_ss_i;
///    spi0_ss_o,
///    uart0_rxd,
////    uart0_txd,
    
 ///==================================
 fclk_o         ,
 rstn_o        ,
 i_irq_cpu,
  // system read/write channel
  sys_cs_o,
 sys_clk_o          ,  // system clock
 ///sys_rstn_o         ,  // system reset - active low
 sys_addr_o         ,  // system read/write address
 sys_wdata_o        ,  // system write data
 sys_sel_o          ,  // system write byte select
 sys_wen_o          ,  // system write enable
 sys_ren_o          ,  // system read enable
 sys_rdata_i        ,  // system read data
 sys_err_i          ,  // system error indicator
 sys_ack_i            // system acknowledge signal
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
  ///========================================
 /// input spi0_miso;
///  output spi0_mosi;
///  output spi0_sck;
///  output spi0_ss_o;///,
////  input uart0_rxd;
///  output uart0_txd;
///========================================

  output  [  3: 0] fclk_o      ;
  ////output  [  2: 0] rstn_o      ;
   output  rstn_o      ;
 
     // system read/write channel
     output      sys_cs_o     ;  //!< system bus cs.
    output             sys_clk_o          ;  // system clock
     ///output             sys_rstn_o        ;  // system reset - active low
     output  [ 32-1: 0] sys_addr_o        ;  // system read/write address
     output  [ 32-1: 0] sys_wdata_o        ;  // system write data
     output  [  4-1: 0] sys_sel_o          ;  // system write byte select
     output             sys_wen_o          ;  // system write enable
     output             sys_ren_o          ;  // system read enable
     input   [ 32-1: 0] sys_rdata_i        ;  // system read data
     input              sys_err_i          ;  // system error indicator
     input              sys_ack_i          ;  // system acknowledge signal
input i_irq_cpu;

 
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
  wire  rstn_o ;
   wire             sys_clk_o          ;  // system clock
assign sys_clk_o=fclk[0];
///wire spi0_miso;
///wire spi0_mosi;
///wire spi0_sck;
///wire spi0_ss_i;
///wire spi0_ss_o;///,
wire clr_n;
///wire uart0_txd;
///==============================
wire i_irq_cpu;
///assign spi0_ss_i=1'b1;
///===================================
///=========================================================
wire [31:0]M_AXI_GP0_araddr;
wire [1:0]M_AXI_GP0_arburst;
wire [3:0]M_AXI_GP0_arcache;
wire [11:0]M_AXI_GP0_arid;
wire [3:0]M_AXI_GP0_arlen;
wire [1:0]M_AXI_GP0_arlock;
wire [2:0]M_AXI_GP0_arprot;
wire [3:0]M_AXI_GP0_arqos;
wire M_AXI_GP0_arready;
wire [2:0]M_AXI_GP0_arsize;
wire M_AXI_GP0_arvalid;

wire [31:0]M_AXI_GP0_awaddr;
wire [1:0]M_AXI_GP0_awburst;
wire [3:0]M_AXI_GP0_awcache;
wire [11:0]M_AXI_GP0_awid;
wire [3:0]M_AXI_GP0_awlen;
wire [1:0]M_AXI_GP0_awlock;
wire [2:0]M_AXI_GP0_awprot;
wire [3:0]M_AXI_GP0_awqos;
wire M_AXI_GP0_awready;
wire [2:0]M_AXI_GP0_awsize;
wire M_AXI_GP0_awvalid;

wire [11:0]M_AXI_GP0_bid;
wire M_AXI_GP0_bready;
wire [1:0]M_AXI_GP0_bresp;
wire M_AXI_GP0_bvalid;

wire [31:0]M_AXI_GP0_rdata;
wire [11:0]M_AXI_GP0_rid;
wire M_AXI_GP0_rlast;
wire M_AXI_GP0_rready;
wire [1:0]M_AXI_GP0_rresp;
wire M_AXI_GP0_rvalid;

wire [31:0]M_AXI_GP0_wdata;
wire [11:0]M_AXI_GP0_wid;
wire M_AXI_GP0_wlast;
wire M_AXI_GP0_wready;
wire [3:0]M_AXI_GP0_wstrb;
wire M_AXI_GP0_wvalid;
wire            M_AXI_GP0_aclk        ;
wire [3:0]fclk_o;
wire sys_cs_o;
///==============================
BUFG i_fclk0_buf  (.O(fclk_o[0]), .I(fclk[0]));
BUFG i_fclk1_buf  (.O(fclk_o[1]), .I(fclk[1]));
BUFG i_fclk2_buf  (.O(fclk_o[2]), .I(fclk[2]));
BUFG i_fclk3_buf  (.O(fclk_o[3]), .I(fclk[3]));

assign rstn_o    = clr_n      ;

///==============================

axi_slave #(
  .AXI_DW     (  32     ), // data width (8,16,...,1024)
  .AXI_AW     (  32     )///, // address width
///  .AXI_IW     (  12     )  // ID width
)
i_gp0_slave
(
 // global signals
  .axi_clk_i        (sys_clk_o),  // global clock
  .axi_rstn_i       ( rstn_o   ),  // global reset

 // axi write address channel
///  .axi_awid_i       (  M_AXI_GP0_awid           ),  // write address ID
  .axi_awaddr_i     (  M_AXI_GP0_awaddr         ),  // write address
///  .axi_awlen_i      (  M_AXI_GP0_awlen          ),  // write burst length
///  .axi_awsize_i     (  M_AXI_GP0_awsize         ),  // write burst size
///  .axi_awburst_i    (  M_AXI_GP0_awburst        ),  // write burst type
///  .axi_awlock_i     (  M_AXI_GP0_awlock         ),  // write lock type
///  .axi_awcache_i    (  M_AXI_GP0_awcache        ),  // write cache type
  .axi_awprot_i     (  M_AXI_GP0_awprot         ),  // write protection type
  .axi_awvalid_i    (  M_AXI_GP0_awvalid        ),  // write address valid
  .axi_awready_o    (  M_AXI_GP0_awready        ),  // write ready

 // axi write data channel
///  .axi_wid_i        (  M_AXI_GP0_wid            ),  // write data ID
  .axi_wdata_i      (  M_AXI_GP0_wdata          ),  // write data
  .axi_wstrb_i      (  M_AXI_GP0_wstrb          ),  // write strobes
///  .axi_wlast_i      (  M_AXI_GP0_wlast          ),  // write last
  .axi_wvalid_i     (  M_AXI_GP0_wvalid         ),  // write valid
  .axi_wready_o     (  M_AXI_GP0_wready         ),  // write ready

 // axi write response channel
 //// .axi_bid_o        (  M_AXI_GP0_bid            ),  // write response ID
  .axi_bresp_o      (  M_AXI_GP0_bresp          ),  // write response
  .axi_bvalid_o     (  M_AXI_GP0_bvalid         ),  // write response valid
  .axi_bready_i     (  M_AXI_GP0_bready         ),  // write response ready

 // axi read address channel
///  .axi_arid_i       (  M_AXI_GP0_arid           ),  // read address ID
  .axi_araddr_i     (  M_AXI_GP0_araddr         ),  // read address
///  .axi_arlen_i      (  M_AXI_GP0_arlen          ),  // read burst length
///  .axi_arsize_i     (  M_AXI_GP0_arsize         ),  // read burst size
///  .axi_arburst_i    (  M_AXI_GP0_arburst        ),  // read burst type
///  .axi_arlock_i     (  M_AXI_GP0_arlock         ),  // read lock type
////  .axi_arcache_i    (  M_AXI_GP0_arcache        ),  // read cache type
  .axi_arprot_i     (  M_AXI_GP0_arprot         ),  // read protection type
  .axi_arvalid_i    (  M_AXI_GP0_arvalid        ),  // read address valid
  .axi_arready_o    (  M_AXI_GP0_arready        ),  // read address ready
    
 // axi read data channel
///  .axi_rid_o        (  M_AXI_GP0_rid            ),  // read response ID
  .axi_rdata_o      (  M_AXI_GP0_rdata          ),  // read data
  .axi_rresp_o      (  M_AXI_GP0_rresp          ),  // read response
////  .axi_rlast_o      (  M_AXI_GP0_rlast          ),  // read last
  .axi_rvalid_o     (  M_AXI_GP0_rvalid         ),  // read response valid
  .axi_rready_i     (  M_AXI_GP0_rready         ),  // read response ready

 // system read/write channel
 .sys_cs_o(sys_cs_o),
  .sys_addr_o       (  sys_addr_o              ),  // system read/write address
  .sys_wdata_o      (  sys_wdata_o             ),  // system write data
  .sys_sel_o        (  sys_sel_o               ),  // system write byte select
  .sys_wen_o        (  sys_wen_o               ),  // system write enable
  .sys_ren_o        (  sys_ren_o               ),  // system read enable
  .sys_rdata_i      (  sys_rdata_i             )///,  // system read data
////  .sys_err_i        (  sys_err_i               ),  // system error indicator
////  .sys_ack_i        (  sys_ack_i               )   // system acknowledge signal
);

///==============================
sparrow_ps_wrapper system_i
       (.DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        ///==========================================
         .M_AXI_araddr(M_AXI_GP0_araddr),
 ///       .M_AXI_GP0_arburst(M_AXI_GP0_arburst),
 ///       .M_AXI_GP0_arcache(M_AXI_GP0_arcache),
 ///       .M_AXI_GP0_arid(M_AXI_GP0_arid),
 ///       .M_AXI_GP0_arlen(M_AXI_GP0_arlen),
 ///       .M_AXI_GP0_arlock(M_AXI_GP0_arlock),
        .M_AXI_arprot(M_AXI_GP0_arprot),
  ////      .M_AXI_GP0_arqos(M_AXI_GP0_arqos),
        .M_AXI_arready(M_AXI_GP0_arready),
  ///      .M_AXI_GP0_arsize(M_AXI_GP0_arsize),
        .M_AXI_arvalid(M_AXI_GP0_arvalid),
        .M_AXI_awaddr(M_AXI_GP0_awaddr),
  ///      .M_AXI_GP0_awburst(M_AXI_GP0_awburst),
  ///      .M_AXI_GP0_awcache(M_AXI_GP0_awcache),
  ///      .M_AXI_GP0_awid(M_AXI_GP0_awid),
  ///      .M_AXI_GP0_awlen(M_AXI_GP0_awlen),
  ///      .M_AXI_GP0_awlock(M_AXI_GP0_awlock),
        .M_AXI_awprot(M_AXI_GP0_awprot),
  ///      .M_AXI_GP0_awqos(M_AXI_GP0_awqos),
        .M_AXI_awready(M_AXI_GP0_awready),
  ///      .M_AXI_GP0_awsize(M_AXI_GP0_awsize),
        .M_AXI_awvalid(M_AXI_GP0_awvalid),
  ////      .M_AXI_GP0_bid(M_AXI_GP0_bid),
        .M_AXI_bready(M_AXI_GP0_bready),
        .M_AXI_bresp(M_AXI_GP0_bresp),
        .M_AXI_bvalid(M_AXI_GP0_bvalid),
        .M_AXI_rdata(M_AXI_GP0_rdata),
 ////       .M_AXI_GP0_rid(M_AXI_GP0_rid),
 ///       .M_AXI_GP0_rlast(M_AXI_GP0_rlast),
        .M_AXI_rready(M_AXI_GP0_rready),
        .M_AXI_rresp(M_AXI_GP0_rresp),
        .M_AXI_rvalid(M_AXI_GP0_rvalid),
        .M_AXI_wdata(M_AXI_GP0_wdata),
 ////       .M_AXI_GP0_wid(M_AXI_GP0_wid),
 ////       .M_AXI_GP0_wlast(M_AXI_GP0_wlast),
        .M_AXI_wready(M_AXI_GP0_wready),
        .M_AXI_wstrb(M_AXI_GP0_wstrb),
        .M_AXI_wvalid(M_AXI_GP0_wvalid),
        ///=================================
        .irq_cpu(i_irq_cpu),
///=========================================
        .fclk0(fclk[0]),
        .fclk1(fclk[1]),
        .fclk2(fclk[2]),
        .fclk3(fclk[3]),
       .o_clr_n(clr_n)///,
///        .o_timer0(o_timer0),
///        .o_timer1(o_timer1),
 ///       .spi0_miso(spi0_miso),
 ///       .spi0_mosi(spi0_mosi),
 ///       .spi0_sck(spi0_sck),
 ///       .spi0_ss_i(spi0_ss_i),
 ///       .spi0_ss_o(spi0_ss_o),
 ///       .gpio_in(gpio_in),              ///,
 ///       .uart0_rxd(uart0_rxd),
 ///       .uart0_txd(uart0_txd)
);
endmodule
