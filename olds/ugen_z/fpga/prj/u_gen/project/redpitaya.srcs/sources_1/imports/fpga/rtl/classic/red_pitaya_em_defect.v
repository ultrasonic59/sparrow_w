
module red_pitaya_em_defect (
  // DAC
  output     [ 14-1: 0] dac_a_o   ,  // DAC data CHA
  output     [ 14-1: 0] dac_b_o   ,  // DAC data CHB
  input                 dac_clk_i ,  // DAC clock
  input                 dac_rstn_i,  // DAC reset - active low
  input                 trig_a_i  ,  // starting trigger CHA
  input                 trig_b_i  ,  // starting trigger CHB
  output                trig_out_o,  // notification trigger
  output reg [ 16-1: 0] pwm_cnf_o         ,  // values used for
  output reg [ 16-1: 0] pwm_pre_o         ,  // values used for
  output reg [ 16-1: 0] pwm_rng_o         ,  // 
  output reg [ 16-1: 0] pwm_data_o         ,  // 
  output wire           o_pwm_long,
  output wire           [3:0]tst_a,
  output wire           [3:0]tst_b,

  // System bus
  input      [ 32-1: 0] sys_addr  ,  // bus address
  input      [ 32-1: 0] sys_wdata ,  // bus write data
  input                 sys_wen   ,  // bus write enable
  input                 sys_ren   ,  // bus read enable
  output reg [ 32-1: 0] sys_rdata ,  // bus read data
  output reg            sys_err   ,  // bus error indicator
  output reg            sys_ack,      // bus acknowledge signal
  output wire dac_trigr_o_a,
  output wire dac_trigr_o_b
);

//---------------------------------------------------------------------------------
//
// generating signal from DAC table 

localparam RSZ = 14 ;  // RAM size 2^RSZ

reg   [RSZ+15: 0] set_a_size   , set_b_size   ;
reg   [RSZ+15: 0] set_a_step   , set_b_step   ;
reg   [RSZ+15: 0] set_a_ofs    , set_b_ofs    ;
reg               set_a_rst    , set_b_rst    ;
reg               set_a_once   , set_b_once   ;
reg               set_a_wrap   , set_b_wrap   ;
reg   [  14-1: 0] set_a_amp    , set_b_amp    ;
reg   [  14-1: 0] set_a_dc     , set_b_dc     ;
reg               set_a_zero   , set_b_zero   ;
///reg   [  16-1: 0] set_a_ncyc   , set_b_ncyc   ;
reg   [  16-1: 0] len_a   , len_b   ;
reg   [  32-1: 0] set_a_rdly   , set_b_rdly   ;
reg               set_a_rgate  , set_b_rgate  ;
reg               buf_a_we     , buf_b_we     ;
reg   [ RSZ-1: 0] buf_a_addr   , buf_b_addr   ;
wire  [  14-1: 0] buf_a_rdata  , buf_b_rdata  ;
wire  [ RSZ-1: 0] buf_a_rpnt   , buf_b_rpnt   ;
reg   [  32-1: 0] buf_a_rpnt_rd, buf_b_rpnt_rd;
reg               trig_a_sw    , trig_b_sw    ;
reg   [   3-1: 0] trig_a_src   , trig_b_src   ;
wire              trig_a_done  , trig_b_done  ;
///logic [15:0]pwm_pre;
///logic [15:0]pwm_rng;
///logic [15:0]pwm_data;
///wire o_pwm_long;
///=====================================
wire ext_trig_a;
assign ext_trig_a=o_pwm_long?trig_a_i:1'b0;
///=====================================
red_pitaya_em_defect_ch  #(.RSZ (RSZ)) ch [1:0] (
  // DAC
  .dac_o           ({dac_b_o          , dac_a_o          }),  // dac data output
  .dac_clk_i       ({dac_clk_i        , dac_clk_i        }),  // dac clock
  .dac_rstn_i      ({dac_rstn_i       , dac_rstn_i       }),  // dac reset - active low
  // trigger
  .trig_sw_i       ({trig_b_sw        , trig_a_sw        }),  // software trigger
  .trig_ext_i      ({trig_b_i         , ext_trig_a         }),  // external trigger
  .trig_src_i      ({trig_b_src       , trig_a_src       }),  // trigger source selector
  .trig_done_o     ({trig_b_done      , trig_a_done      }),  // trigger event
  // buffer ctrl
  .buf_we_i        ({buf_b_we         , buf_a_we         }),  // buffer buffer write
  .buf_addr_i      ({buf_b_addr       , buf_a_addr       }),  // buffer address
  .buf_wdata_i     ({sys_wdata[14-1:0], sys_wdata[14-1:0]}),  // buffer write data
  .buf_rdata_o     ({buf_b_rdata      , buf_a_rdata      }),  // buffer read data
  .buf_rpnt_o      ({buf_b_rpnt       , buf_a_rpnt       }),  // buffer current read pointer
  // configuration
  .set_size_i      ({set_b_size       , set_a_size       }),  // set table data size
  .set_step_i      ({set_b_step       , set_a_step       }),  // set pointer step
  .set_ofs_i       ({set_b_ofs        , set_a_ofs        }),  // set reset offset
  .set_rst_i       ({set_b_rst        , set_a_rst        }),  // set FMS to reset
  .set_once_i      ({set_b_once       , set_a_once       }),  // set only once
  .set_wrap_i      ({set_b_wrap       , set_a_wrap       }),  // set wrap pointer
  .set_amp_i       ({set_b_amp        , set_a_amp        }),  // set amplitude scale
  .set_dc_i        ({set_b_dc         , set_a_dc         }),  // set output offset
  .set_zero_i      ({set_b_zero       , set_a_zero       }),  // set output to zero
 /// .set_ncyc_i      ({set_b_ncyc       , set_a_ncyc       }),  // set number of cycle
 /// .set_rnum_i      ({set_b_rnum       , set_a_rnum       }),  // set number of repetitions
///  .set_rdly_i      ({set_b_rdly       , set_a_rdly       }),  // set delay between repetitions
///  .set_rgate_i     ({set_b_rgate      , set_a_rgate      })   // set external gated repetition
   .delay_i         ({set_b_rdly       , set_a_rdly       }),  //!< delay psk
    .len_i        ({len_b[14-1:0]       , len_a[14-1:0]       }),//!< len 
    .dac_trigr_o    ({dac_trigr_o_b,dac_trigr_o_a}),
    .tst_o         ({tst_b       , tst_a       })

);

always @(posedge dac_clk_i)
begin
   buf_a_we   <= sys_wen && (sys_addr[19:RSZ+2] == 'h1);
   buf_b_we   <= sys_wen && (sys_addr[19:RSZ+2] == 'h2);
   buf_a_addr <= sys_addr[RSZ+1:2] ;  // address timing violation
   buf_b_addr <= sys_addr[RSZ+1:2] ;  // can change only synchronous to write clock
end

assign trig_out_o = trig_a_done ;

//---------------------------------------------------------------------------------
//
//  System bus connection

reg  [3-1: 0] ren_dly ;
reg           ack_dly ;

always @(posedge dac_clk_i)
if (dac_rstn_i == 1'b0) begin
   trig_a_sw   <=  1'b0    ;
   trig_a_src  <=  3'h0    ;
   set_a_amp   <= 14'h2000 ;
   set_a_dc    <= 14'h0    ;
   set_a_zero  <=  1'b0    ;
   set_a_rst   <=  1'b0    ;
   set_a_once  <=  1'b0    ;
   set_a_wrap  <=  1'b0    ;
   set_a_size  <= {RSZ+16{1'b1}} ;
   set_a_ofs   <= {RSZ+16{1'b0}} ;
   set_a_step  <={{RSZ+15{1'b0}},1'b0} ;
///   set_a_ncyc  <= 16'h0    ;
   len_a  <= 16'h10    ;
   set_a_rdly  <= 32'h200    ;
   set_a_rgate <=  1'b0    ;
   trig_b_sw   <=  1'b0    ;
   trig_b_src  <=  3'h0    ;
   set_b_amp   <= 14'h2000 ;
   set_b_dc    <= 14'h0    ;
   set_b_zero  <=  1'b0    ;
   set_b_rst   <=  1'b0    ;
   set_b_once  <=  1'b0    ;
   set_b_wrap  <=  1'b0    ;
   set_b_size  <= {RSZ+16{1'b1}} ;
   set_b_ofs   <= {RSZ+16{1'b0}} ;
   set_b_step  <={{RSZ+15{1'b0}},1'b0} ;
///   set_b_ncyc  <= 16'h0    ;
   len_b  <= 16'h10    ;
   set_b_rdly  <= 32'h200    ;
   set_b_rgate <=  1'b0    ;
   ren_dly     <=  3'h0    ;
   ack_dly     <=  1'b0    ;
   pwm_cnf_o     <= 16'h000 ;
   pwm_pre_o     <= 16'h1000 ;
   pwm_rng_o     <= 16'h1000 ;
   pwm_data_o     <= 16'h800 ;

end else begin
   trig_a_sw  <= sys_wen && (sys_addr[19:0]==20'h0) && sys_wdata[0]  ;
   if (sys_wen && (sys_addr[19:0]==20'h0))
      trig_a_src <= sys_wdata[2:0] ;

   trig_b_sw  <= sys_wen && (sys_addr[19:0]==20'h0) && sys_wdata[16]  ;
   if (sys_wen && (sys_addr[19:0]==20'h0))
      trig_b_src <= sys_wdata[19:16] ;

   if (sys_wen) begin
      if (sys_addr[19:0]==20'h0)   {set_a_rgate, set_a_zero, set_a_rst, set_a_once, set_a_wrap} <= sys_wdata[ 8: 4] ;
      if (sys_addr[19:0]==20'h0)   {set_b_rgate, set_b_zero, set_b_rst, set_b_once, set_b_wrap} <= sys_wdata[24:20] ;

      if (sys_addr[19:0]==20'h4)   set_a_amp  <= sys_wdata[  0+13: 0] ;
      if (sys_addr[19:0]==20'h4)   set_a_dc   <= sys_wdata[ 16+13:16] ;
      if (sys_addr[19:0]==20'h8)   set_a_size <= sys_wdata[RSZ+15: 0] ;
      if (sys_addr[19:0]==20'hC)   set_a_ofs  <= sys_wdata[RSZ+15: 0] ;
      if (sys_addr[19:0]==20'h10)  set_a_step <= sys_wdata[RSZ+15: 0] ;
///      if (sys_addr[19:0]==20'h18)  set_a_ncyc <= sys_wdata[  16-1: 0] ;
      if (sys_addr[19:0]==20'h1C)  set_a_rdly <= sys_wdata[  32-1: 0] ;
      if (sys_addr[19:0]==20'h20)  len_a <= sys_wdata[  16-1: 0] ;

      if (sys_addr[19:0]==20'h24)  set_b_amp  <= sys_wdata[  0+13: 0] ;
      if (sys_addr[19:0]==20'h24)  set_b_dc   <= sys_wdata[ 16+13:16] ;
      if (sys_addr[19:0]==20'h28)  set_b_size <= sys_wdata[RSZ+15: 0] ;
      if (sys_addr[19:0]==20'h2C)  set_b_ofs  <= sys_wdata[RSZ+15: 0] ;
      if (sys_addr[19:0]==20'h30)  set_b_step <= sys_wdata[RSZ+15: 0] ;
 ///     if (sys_addr[19:0]==20'h38)  set_b_ncyc <= sys_wdata[  16-1: 0] ;
      if (sys_addr[19:0]==20'h3C)  set_b_rdly <= sys_wdata[  32-1: 0] ;
      if (sys_addr[19:0]==20'h40)  len_a <= sys_wdata[  16-1: 0] ;
      if (sys_addr[19:0]==20'h80)   pwm_cnf_o <= sys_wdata[16-1: 0] ;
      if (sys_addr[19:0]==20'h84)   pwm_pre_o <= sys_wdata[16-1: 0] ;
      if (sys_addr[19:0]==20'h88)   pwm_rng_o <= sys_wdata[16-1: 0] ;
      if (sys_addr[19:0]==20'h8c)   pwm_data_o <= sys_wdata[16-1: 0] ;
     
      
   end

   if (sys_ren) begin
      buf_a_rpnt_rd <= {{32-RSZ-2{1'b0}},buf_a_rpnt,2'h0};
      buf_b_rpnt_rd <= {{32-RSZ-2{1'b0}},buf_b_rpnt,2'h0};
   end

   ren_dly <= {ren_dly[3-2:0], sys_ren};
   ack_dly <=  ren_dly[3-1] || sys_wen ;
end

wire [32-1: 0] r0_rd = {7'h0,set_b_rgate, set_b_zero,set_b_rst,set_b_once,set_b_wrap, 1'b0,trig_b_src,
                        7'h0,set_a_rgate, set_a_zero,set_a_rst,set_a_once,set_a_wrap, 1'b0,trig_a_src };

wire sys_en;
assign sys_en = sys_wen | sys_ren;

always @(posedge dac_clk_i)
if (dac_rstn_i == 1'b0) begin
   sys_err <= 1'b0 ;
   sys_ack <= 1'b0 ;
end else begin
   sys_err <= 1'b0 ;

   casez (sys_addr[19:0])
     20'h00000 : begin sys_ack <= sys_en;          sys_rdata <= r0_rd                              ; end

     20'h00004 : begin sys_ack <= sys_en;          sys_rdata <= {2'h0, set_a_dc, 2'h0, set_a_amp}  ; end
     20'h00008 : begin sys_ack <= sys_en;          sys_rdata <= {{32-RSZ-16{1'b0}},set_a_size}     ; end
     20'h0000C : begin sys_ack <= sys_en;          sys_rdata <= {{32-RSZ-16{1'b0}},set_a_ofs}      ; end
     20'h00010 : begin sys_ack <= sys_en;          sys_rdata <= {{32-RSZ-16{1'b0}},set_a_step}     ; end
     20'h00014 : begin sys_ack <= sys_en;          sys_rdata <= buf_a_rpnt_rd                      ; end
 ///    20'h00018 : begin sys_ack <= sys_en;          sys_rdata <= {{32-16{1'b0}},set_a_ncyc}         ; end
      20'h00018 : begin sys_ack <= sys_en;          sys_rdata <= 32'h87654321                       ; end
     20'h0001C : begin sys_ack <= sys_en;          sys_rdata <= set_a_rdly                          ; end
     20'h00020 : begin sys_ack <= sys_en;          sys_rdata <= {{32-16{1'b0}},len_a}               ; end

     20'h00024 : begin sys_ack <= sys_en;          sys_rdata <= {2'h0, set_b_dc, 2'h0, set_b_amp}  ; end
     20'h00028 : begin sys_ack <= sys_en;          sys_rdata <= {{32-RSZ-16{1'b0}},set_b_size}     ; end
     20'h0002C : begin sys_ack <= sys_en;          sys_rdata <= {{32-RSZ-16{1'b0}},set_b_ofs}      ; end
     20'h00030 : begin sys_ack <= sys_en;          sys_rdata <= {{32-RSZ-16{1'b0}},set_b_step}     ; end
     20'h00034 : begin sys_ack <= sys_en;          sys_rdata <= buf_b_rpnt_rd                      ; end
///     20'h00038 : begin sys_ack <= sys_en;          sys_rdata <= {{32-16{1'b0}},set_b_ncyc}         ; end
     20'h0003C : begin sys_ack <= sys_en;          sys_rdata <= set_b_rdly         ; end
     20'h00040 : begin sys_ack <= sys_en;          sys_rdata <= {{32-16{1'b0}},len_b}                          ; end
     20'h00044 : begin sys_ack <= sys_en;          sys_rdata <= 32'h12345678                          ; end///for test
     
     20'h0080 : begin sys_ack <= sys_en;            sys_rdata <= {{32-16{1'b0}}, pwm_cnf_o}          ; end
      20'h0084 : begin sys_ack <= sys_en;         sys_rdata <= {{32-16{1'b0}}, pwm_pre_o}          ; end
      20'h0088 : begin sys_ack <= sys_en;         sys_rdata <= {{32-16{1'b0}}, pwm_rng_o}          ; end
      20'h008c : begin sys_ack <= sys_en;         sys_rdata <= {{32-16{1'b0}}, pwm_data_o}          ; end
    
    

     20'h1zzzz : begin sys_ack <= ack_dly;         sys_rdata <= {{32-14{1'b0}},buf_a_rdata}        ; end
     20'h2zzzz : begin sys_ack <= ack_dly;         sys_rdata <= {{32-14{1'b0}},buf_b_rdata}        ; end

       default : begin sys_ack <= sys_en;          sys_rdata <=  32'h0                             ; end
   endcase
end

pwm_long pwm_long
(
  // system signals
  .clk(dac_clk_i) ,  // clock
  .rstn(dac_rstn_i),  // reset (active low)
  .cke(1'b1) ,  // clock enable (synchronous)
  // configuration
  .ena(1'b1),   // enable
  .rng(pwm_rng_o),   // range
  .pre(pwm_pre_o),   // range
  // stream input
  .str_dat(pwm_data_o),  // data
  .str_rdy(),  // ready
  .end_pre(),  // 
  // PWM output
  .pwm(o_pwm_long)
);

endmodule
