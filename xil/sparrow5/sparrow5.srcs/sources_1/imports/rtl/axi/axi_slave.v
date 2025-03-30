

module axi_slave 
#(
parameter           AXI_DW         =  32           , // data width (8,16,...,1024)
parameter           AXI_AW         =  32           , // address width
///parameter           AXI_IW         =   8           , // ID width
parameter           AXI_SW         = AXI_DW >> 3    ///,     // strobe width - 1 bit for every data byte
///parameter           LEN_WE      =4,
////parameter           LEN_CS      =8
)
(
 // global signals
axi_clk_i      ,  //!< AXI global clock
axi_rstn_i     ,  //!< AXI global reset

 // axi write address channel
///axi_awid_i     ,  //!< AXI write address ID
axi_awaddr_i   ,  //!< AXI write address
///axi_awlen_i    ,  //!< AXI write burst length
///axi_awsize_i   ,  //!< AXI write burst size
///axi_awburst_i  ,  //!< AXI write burst type
///axi_awlock_i   ,  //!< AXI write lock type
///axi_awcache_i  ,  //!< AXI write cache type
axi_awprot_i   ,  //!< AXI write protection type
axi_awvalid_i  ,  //!< AXI write address valid
axi_awready_o  ,  //!< AXI write ready

 // axi write data channel
//axi_wid_i      ,  //!< AXI write data ID
axi_wdata_i    ,  //!< AXI write data
axi_wstrb_i    ,  //!< AXI write strobes
///axi_wlast_i    ,  //!< AXI write last
axi_wvalid_i   ,  //!< AXI write valid
axi_wready_o   ,  //!< AXI write ready

 // axi write response channel
///axi_bid_o      ,  //!< AXI write response ID
axi_bresp_o    ,  //!< AXI write response
axi_bvalid_o   ,  //!< AXI write response valid
axi_bready_i   ,  //!< AXI write response ready

 // axi read address channel
///axi_arid_i     ,  //!< AXI read address ID
axi_araddr_i   ,  //!< AXI read address
///axi_arlen_i    ,  //!< AXI read burst length
///axi_arsize_i   ,  //!< AXI read burst size
///axi_arburst_i  ,  //!< AXI read burst type
///axi_arlock_i   ,  //!< AXI read lock type
///axi_arcache_i  ,  //!< AXI read cache type
axi_arprot_i   ,  //!< AXI read protection type
axi_arvalid_i  ,  //!< AXI read address valid
axi_arready_o  ,  //!< AXI read address ready
    
 // axi read data channel
///axi_rid_o      ,  //!< AXI read response ID
axi_rdata_o    ,  //!< AXI read data
axi_rresp_o    ,  //!< AXI read response
///axi_rlast_o    ,  //!< AXI read last
axi_rvalid_o   ,  //!< AXI read response valid
axi_rready_i   ,  //!< AXI read response ready

 // RP system read/write channel
sys_cs_o     ,  //!< system bus cs.
sys_addr_o     ,  //!< system bus read/write address.
sys_wdata_o    ,  //!< system bus write data.
sys_sel_o      ,  //!< system bus write byte select.
sys_wen_o      ,  //!< system bus write enable.
sys_ren_o      ,  //!< system bus read enable.
sys_rdata_i    ////,  //!< system bus read data.
///   input                     sys_err_i      ,  //!< system bus error indicator.
////   input                     sys_ack_i         //!< system bus acknowledge signal.
);
///===================================================
 // global signals
   input                     axi_clk_i;  //!< AXI global clock
   input                     axi_rstn_i;  //!< AXI global reset

 // axi write address channel
///   input      [ AXI_IW-1: 0] axi_awid_i;  //!< AXI write address ID
   input      [ AXI_AW-1: 0] axi_awaddr_i;  //!< AXI write address
///   input      [      4-1: 0] axi_awlen_i;  //!< AXI write burst length
///   input      [      3-1: 0] axi_awsize_i;  //!< AXI write burst size
///   input      [      2-1: 0] axi_awburst_i;  //!< AXI write burst type
///   input      [      2-1: 0] axi_awlock_i;  //!< AXI write lock type
///   input      [      4-1: 0] axi_awcache_i;  //!< AXI write cache type
   input      [      3-1: 0] axi_awprot_i;  //!< AXI write protection type
   input                     axi_awvalid_i;  //!< AXI write address valid
   output                    axi_awready_o;  //!< AXI write ready

 // axi write data channel
///   input      [ AXI_IW-1: 0] axi_wid_i;  //!< AXI write data ID
   input      [ AXI_DW-1: 0] axi_wdata_i;  //!< AXI write data
   input      [ AXI_SW-1: 0] axi_wstrb_i;  //!< AXI write strobes
///   input                     axi_wlast_i;  //!< AXI write last
   input                     axi_wvalid_i;  //!< AXI write valid
   output                    axi_wready_o;  //!< AXI write ready

 // axi write response channel
///   output     [ AXI_IW-1: 0] axi_bid_o;  //!< AXI write response ID
   output  [      2-1: 0] axi_bresp_o;  //!< AXI write response
   output                 axi_bvalid_o;  //!< AXI write response valid
   input                     axi_bready_i;  //!< AXI write response ready

 // axi read address channel
///   input      [ AXI_IW-1: 0] axi_arid_i;  //!< AXI read address ID
   input      [ AXI_AW-1: 0] axi_araddr_i;  //!< AXI read address
///   input      [      4-1: 0] axi_arlen_i;  //!< AXI read burst length
///   input      [      3-1: 0] axi_arsize_i;  //!< AXI read burst size
///   input      [      2-1: 0] axi_arburst_i;  //!< AXI read burst type
///   input      [      2-1: 0] axi_arlock_i;  //!< AXI read lock type
////   input      [      4-1: 0] axi_arcache_i;  //!< AXI read cache type
   input      [      3-1: 0] axi_arprot_i;  //!< AXI read protection type
   input                     axi_arvalid_i;  //!< AXI read address valid
   output                    axi_arready_o;  //!< AXI read address ready
    
 // axi read data channel
////   output     [ AXI_IW-1: 0] axi_rid_o;  //!< AXI read response ID
   output  [ AXI_DW-1: 0] axi_rdata_o;  //!< AXI read data
   output  [      2-1: 0] axi_rresp_o;  //!< AXI read response
////   output                 axi_rlast_o;  //!< AXI read last
   output                 axi_rvalid_o;  //!< AXI read response valid
   input                     axi_rready_i;  //!< AXI read response ready

 // RP system read/write channel
   output      sys_cs_o;  //!< system bus cs.
   output     [ AXI_AW-1: 0] sys_addr_o;  //!< system bus read/write address.
   output     [ AXI_DW-1: 0] sys_wdata_o;  //!< system bus write data.
   output  [ AXI_SW-1: 0] sys_sel_o;  //!< system bus write byte select.
   output                 sys_wen_o;  //!< system bus write enable.
   output                 sys_ren_o;  //!< system bus read enable.
   input      [ AXI_DW-1: 0] sys_rdata_i;  //!< system bus read data.
///===================================================
    // axi write address channel
(* mark_debug = "true" *)
wire      [ AXI_AW-1: 0] axi_awaddr_i;  //!< AXI write address
(* mark_debug = "true" *)
wire      [      3-1: 0] axi_awprot_i;  //!< AXI write protection type
(* mark_debug = "true" *)
wire                     axi_awvalid_i;  //!< AXI write address valid
(* mark_debug = "true" *)
wire                    axi_awready_o;  //!< AXI write ready

// axi write data channel
(* mark_debug = "true" *)
wire      [ AXI_DW-1: 0] axi_wdata_i;  //!< AXI write data
(* mark_debug = "true" *)
wire      [ AXI_SW-1: 0] axi_wstrb_i;  //!< AXI write strobes
(* mark_debug = "true" *)
wire                     axi_wvalid_i;  //!< AXI write valid
(* mark_debug = "true" *)
wire                    axi_wready_o;  //!< AXI write ready

(* mark_debug = "true" *)
reg     [1: 0] axi_bresp_o;  //!< AXI write response
(* mark_debug = "true" *)
reg      axi_bvalid_o;  //!< AXI write response valid
(* mark_debug = "true" *)
wire                     axi_bready_i;  //!< AXI write response ready
(* mark_debug = "true" *)
wire      [ AXI_AW-1: 0] axi_araddr_i;  //!< AXI read address
(* mark_debug = "true" *)
wire      [      3-1: 0] axi_arprot_i;  //!< AXI read protection type
(* mark_debug = "true" *)
wire                     axi_arvalid_i;  //!< AXI read address valid
(* mark_debug = "true" *)
wire                    axi_arready_o;  //!< AXI read address ready
    // axi read data channel
///      output     [ AXI_IW-1: 0] axi_rid_o;  //!< AXI read response ID
(* mark_debug = "true" *)
reg [ AXI_DW-1: 0] axi_rdata_o;  //!< AXI read data
(* mark_debug = "true" *)
reg [      2-1: 0] axi_rresp_o;  //!< AXI read response
(* mark_debug = "true" *)
////reg                axi_rlast_o;  //!< AXI read last
(* mark_debug = "true" *)
reg                axi_rvalid_o;  //!< AXI read response valid
(* mark_debug = "true" *)
wire axi_rready_i ;  

(* mark_debug = "true" *)
wire sys_cs_o;
(* mark_debug = "true" *)
wire     [ AXI_AW-1: 0] sys_addr_o; 
(* mark_debug = "true" *)
wire     [ AXI_DW-1: 0] sys_wdata_o;  //!< system bus write data.
(* mark_debug = "true" *)
reg [ AXI_SW-1: 0] sys_sel_o;  //!< system bus write byte select.
(* mark_debug = "true" *)
reg                sys_wen_o;  //!< system bus write enable.
(* mark_debug = "true" *)
reg                sys_ren_o;  //!< system bus read enable.
(* mark_debug = "true" *)
wire      [ AXI_DW-1: 0] sys_rdata_i;  //!< system bus read data.


//---------------------------------------------------------------------------------
//
//  AXI slave Module
///wire                 ack         ;
///reg   [      6-1: 0] ack_cnt     ;
wire                 end_rd_do         ;

(* mark_debug = "true" *)
reg                  rd_do       ;
////reg   [ AXI_IW-1: 0] rd_arid     ;
(* mark_debug = "true" *)
reg   [ AXI_AW-1: 0] rd_araddr   ;
///reg                  rd_error    ;
///wire                 rd_errorw   ;

wire                 end_wr_do         ;
(* mark_debug = "true" *)
reg                  wr_do       ;
///reg   [ AXI_IW-1: 0] wr_awid     ;
reg   [ AXI_AW-1: 0] wr_awaddr   ;
///reg   [ AXI_IW-1: 0] wr_wid      ;
reg   [ AXI_DW-1: 0] wr_wdata    ;
///reg                  wr_error    ;
///wire                 wr_errorw   ;


///=====================================================
///(* mark_debug = "true" *)
///wire      [ AXI_IW-1: 0] axi_arid_i;  //!< AXI read address ID

///=====================================================

reg r_axi_arready; 
assign sys_cs_o = rd_do |wr_do; ///
///assign wr_errorw = 1'b0;///(axi_awlen_i != 4'h0) || (axi_awsize_i != 3'b010); // error if write burst and more/less than 4B transfer
///assign rd_errorw = 1'b0;///(axi_arlen_i != 4'h0) || (axi_arsize_i != 3'b010); // error if read burst and more/less than 4B transfer

always @(posedge axi_clk_i) 
    begin
    if (axi_rstn_i == 1'b0) 
        begin
        rd_do    <= 1'b0 ;
 ////       r_axi_arready<=1'b0;
///       rd_error <= 1'b0 ;
        end
    else 
        begin
        if (axi_arvalid_i && !rd_do && !axi_awvalid_i && !wr_do) // accept just one read request - write has priority
         rd_do  <= 1'b1 ;
        else if (axi_rready_i && rd_do && end_rd_do)
            rd_do  <= 1'b0 ;
/*
 ///       if (axi_arvalid_i && axi_arready_o)  // latch ID and address
        if (axi_arvalid_i &&(!rd_do))  // latch ID and address
            begin 
            r_axi_arready<=1'b1;
////            rd_arid   <= axi_arid_i   ;
 ///           rd_araddr <= axi_araddr_i ;
 ///           rd_error  <= rd_errorw    ;
            end
 */           
        end
end
///===============================
always @(posedge axi_clk_i)
begin
if(axi_rstn_i == 1'b0)
    r_axi_arready<=1'b0;
if (axi_arvalid_i) 
    begin
    rd_araddr <= axi_araddr_i ;
    r_axi_arready<=1'b1;
    end
if (rd_do) 
    r_axi_arready<=1'b0;
end

///===============================
reg [3:0]z_wr_do;
always @(posedge axi_clk_i) 
begin
if (axi_rstn_i == 1'b0) 
   begin
   z_wr_do    <= 4'b0 ;
   end
else if(wr_do)
   z_wr_do    <= {z_wr_do[2:0],wr_do} ;
else 
    z_wr_do     <= 4'b0 ;
end
///===============================
reg [3:0]z_rd_do;
always @(posedge axi_clk_i) 
begin
if (axi_rstn_i == 1'b0) 
   begin
   z_rd_do    <= 4'b0 ;
   end
else if(rd_do) 
   z_rd_do    <= {z_rd_do[2:0],rd_do} ;
else 
      z_rd_do    <= 4'b0 ;
end

always @(posedge axi_clk_i) 
begin
if (axi_rstn_i == 1'b0) 
   begin
   wr_do    <= 1'b0 ;
 ///  wr_error <= 1'b0 ;
   end
else 
    begin
    if (axi_awvalid_i && !wr_do && !rd_do) // accept just one write request - if idle
         wr_do  <= 1'b1 ;
    else if (axi_bready_i && wr_do && end_wr_do)
         wr_do  <= 1'b0 ;

if (axi_awvalid_i && axi_awready_o) // latch ID and address
    begin 
 ///   wr_awid   <= axi_awid_i   ;
    wr_awaddr <= axi_awaddr_i ;
 ///   wr_error  <= wr_errorw    ;
    end

if (axi_wvalid_i && wr_do) 
    begin // latch ID and write data
 ///   wr_wid    <= axi_wid_i    ;
    case(axi_wstrb_i)
 ///       4'h1,4'h0:
 ///           wr_wdata  <= {axi_wdata_i[7:0],axi_wdata_i[7:0],axi_wdata_i[7:0],axi_wdata_i[7:0]}  ;
 ///       4'h2:
 ///           wr_wdata  <= {axi_wdata_i[15:8],axi_wdata_i[15:8],axi_wdata_i[15:8],axi_wdata_i[15:8]}  ;
 ///       4'h3:
 ///          wr_wdata  <= {axi_wdata_i[15:0],axi_wdata_i[15:0]}  ;
 ///       4'h4:
 ///           wr_wdata  <= {axi_wdata_i[23:16],axi_wdata_i[23:16],axi_wdata_i[23:16],axi_wdata_i[23:16]}  ;
 ///       4'h8:
 ///           wr_wdata  <= {axi_wdata_i[31:24],axi_wdata_i[31:24],axi_wdata_i[31:24],axi_wdata_i[31:24]}  ;
        4'hc:
             wr_wdata  <= {axi_wdata_i[31:16],axi_wdata_i[31:16]}  ;
        default:
           wr_wdata  <= axi_wdata_i ;
    endcase
    end
   end
end





assign axi_awready_o = !wr_do && !rd_do                      ;
///assign axi_wready_o  = (wr_do && axi_wvalid_i) || (wr_errorw && axi_wvalid_i)    ;
assign axi_wready_o  = (wr_do && axi_wvalid_i)   ;
///assign axi_bid_o     = wr_awid                               ;
//assign axi_bresp_o   = {wr_error,1'b0}                       ;  // 2'b10 SLVERR 
//assign axi_bvalid_o  = (sys_wen_o && axi_bready_i) || (wr_error && axi_bready_i)      ;
///reg r_arready;
///assign axi_arready_o = 1'b1  ;
///assign axi_arready_o = !rd_do && !wr_do && !axi_awvalid_i     ;
assign axi_arready_o = r_axi_arready     ;

////assign axi_rid_o     = rd_arid                                ;
//assign axi_rdata_o   = sys_rdata_i                            ;

always @(posedge axi_clk_i) 
    begin
    if (axi_rstn_i == 1'b0) 
        begin
        axi_bvalid_o  <= 1'b0 ;
        axi_bresp_o   <= 2'h0 ;
////        axi_rlast_o   <= 1'b0 ;
        axi_rvalid_o  <= 1'b0 ;
        axi_rresp_o   <= 2'h0 ;
    end
   else begin
      axi_bvalid_o  <= wr_do && end_wr_do  ;
////      axi_bresp_o   <= {(wr_error || ack_cnt[5]),1'b0} ;  // 2'b10 SLVERR    2'b00 OK
      axi_bresp_o   <= 2'b00 ;  // 2'b10 SLVERR    2'b00 OK
////      axi_rlast_o   <= rd_do && end_rd_do  ;
      axi_rvalid_o  <= rd_do && end_rd_do  ;
 ////     axi_rresp_o   <= {(rd_error || ack_cnt[5]),1'b0} ;  // 2'b10 SLVERR    2'b00 OK
      axi_rresp_o   <= 2'b0;  // 2'b10 SLVERR    2'b00 OK
      if(rd_araddr[1])
        axi_rdata_o   <= {sys_rdata_i[15:0],sys_rdata_i[15:0]}   ;
      else
        axi_rdata_o   <= sys_rdata_i   ;
     
   end
end
/*
// acknowledge protection
always @(posedge axi_clk_i) 
begin
if (axi_rstn_i == 1'b0) 
   begin
   ack_cnt   <= 6'h0 ;
   end
else 
    begin
    if ((axi_arvalid_i && axi_arready_o) || (axi_awvalid_i && axi_awready_o))  // rd || wr request
         ack_cnt <= 6'h1 ;
      else if (ack)
         ack_cnt <= 6'h0 ;
      else if (|ack_cnt)
         ack_cnt <= ack_cnt + 6'h1 ;
   end
end
*/
///assign end_rd_do = z_rd_do[1];
///assign end_rd_do = z_rd_do[2];
assign end_rd_do = z_rd_do[3];
///assign end_rd_do = rd_do;

///assign end_wr_do = z_wr_do[2];
assign end_wr_do = z_wr_do[3];
///assign end_rd_do = 1'b1;






//------------------------------------------
//  Simple slave interface

always @(posedge axi_clk_i) 
begin
if (axi_rstn_i == 1'b0) 
    begin
    sys_wen_o  <= 1'b0 ;
    sys_ren_o  <= 1'b0 ;
    sys_sel_o  <= {AXI_SW{1'b0}} ;
    end
else 
    begin
 ////     sys_wen_o  <= wr_do && axi_wvalid_i && !wr_errorw ;
 ////   sys_wen_o  <= wr_do ;///&& axi_wvalid_i;
    sys_wen_o  <= |z_wr_do[1:0]&(!(|z_wr_do[3:2])) ;///&& axi_wvalid_i;
 ///   sys_ren_o  <= axi_arvalid_i && axi_arready_o && !rd_errorw ;
   sys_ren_o  <= axi_arvalid_i && axi_arready_o  ;
///     sys_sel_o  <= {AXI_SW{1'b1}} ;
   sys_sel_o  <= axi_wstrb_i ;
   end
end

///assign sys_addr_o  = rd_do ? rd_araddr : wr_awaddr  ;
assign sys_addr_o  = wr_do ? wr_awaddr:rd_araddr  ;
assign sys_wdata_o = wr_wdata;
///------------------------------------
////(* mark_debug = "true" *)
wire cs_us;

 assign cs_us=sys_cs_o&(sys_addr_o[15:14]==2'h2);
/*
ila_1 ila1(
 .clk(axi_clk_i),
  .probe0(axi_awaddr_i)   ,  //!< AXI write address
  .probe1(axi_awprot_i)   ,  //!< AXI write protection type
  .probe2(axi_awvalid_i)  ,  //!< AXI write address valid
  .probe3(axi_awready_o)  ,  //!< AXI write ready
 
  // axi write data channel
  .probe4(axi_wdata_i)    ,  //!< AXI write data
  .probe5(axi_wstrb_i)    ,  //!< AXI write strobes
  .probe6(axi_wvalid_i )  ,  //!< AXI write valid
  .probe7(axi_wready_o )  ,  //!< AXI write ready
 
  // axi write response channel
  .probe8(axi_bresp_o )   ,  //!< AXI write response
  .probe9(axi_bvalid_o)   ,  //!< AXI write response valid
  .probe10(axi_bready_i )  ,  //!< AXI write response ready
 
  // axi read address channel
  .probe11(axi_araddr_i )  ,  //!< AXI read address
  .probe12(axi_arprot_i )  ,  //!< AXI read protection type
  .probe13(axi_arvalid_i ) ,  //!< AXI read address valid
  .probe14(axi_arready_o)  ,  //!< AXI read address ready
     
  // axi read data channel
  .probe15(axi_rdata_o)    ,  //!< AXI read data
  .probe16(axi_rresp_o )   ,  //!< AXI read response
  .probe17(axi_rvalid_o )  ,  //!< AXI read response valid
  .probe18(axi_rready_i)   ,  //!< AXI read response ready
 
  // RP system read/write channel
  .probe19(sys_cs_o )    ,  //!< system bus cs.
  .probe20(sys_addr_o )    ,  //!< system bus read/write address.
  .probe21(sys_wdata_o )   ,  //!< system bus write data.
  .probe22(sys_sel_o )     ,  //!< system bus write byte select.
  .probe23(sys_wen_o)      ,  //!< system bus write enable.
  .probe24(sys_ren_o )     ,  //!< system bus read enable.
  .probe25(sys_rdata_i),    ////,  //!< system bus read data.
  .probe26(rd_do),
  .probe27(rd_araddr)
  
);
*/


endmodule
