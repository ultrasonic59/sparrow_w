
module red_pitaya_em_defect_ch #(
   parameter RSZ = 14
)(
   // DAC
   output reg [ 14-1: 0] dac_o           ,  //!< dac data output
   input                 dac_clk_i       ,  //!< dac clock
   input                 dac_rstn_i      ,  //!< dac reset - active low
   // trigger
   input                 trig_sw_i       ,  //!< software trigger
   input                 trig_ext_i      ,  //!< external trigger
   input      [  3-1: 0] trig_src_i      ,  //!< trigger source selector
   output                trig_done_o     ,  //!< trigger event
   // buffer ctrl
   input                 buf_we_i        ,  //!< buffer write enable
   input      [ 14-1: 0] buf_addr_i      ,  //!< buffer address
   input      [ 14-1: 0] buf_wdata_i     ,  //!< buffer write data
   output reg [ 14-1: 0] buf_rdata_o     ,  //!< buffer read data
   output reg [RSZ-1: 0] buf_rpnt_o      ,  //!< buffer current read pointer
   // configuration
   input     [RSZ+15: 0] set_size_i      ,  //!< set table data size
   input     [RSZ+15: 0] set_step_i      ,  //!< set pointer step
   input     [RSZ+15: 0] set_ofs_i       ,  //!< set reset offset
   input                 set_rst_i       ,  //!< set FSM to reset
   input                 set_once_i      ,  //!< set only once  -- not used
   input                 set_wrap_i      ,  //!< set wrap enable
   input     [  14-1: 0] set_amp_i       ,  //!< set amplitude scale
   input     [  14-1: 0] set_dc_i        ,  //!< set output offset
   input                 set_zero_i      ,  //!< set output to zero
   ///input     [  16-1: 0] set_ncyc_i      ,  //!< set number of cycle
   ///input     [  16-1: 0] set_rnum_i      ,  //!< set number of repetitions
   ///input     [  32-1: 0] set_rdly_i      ,  //!< set delay between repetitions
   ////input                 set_rgate_i     ,   //!< set external gated repetition
   input     [  32-1: 0] delay_i      ,  //!< delay psk
   input     [  RSZ-1: 0] len_i       , //!< len 
   output wire dac_trigr_o,
   output wire [3: 0] tst_o        ///
   
);

//---------------------------------------------------------------------------------
//
//  DAC buffer RAM

reg   [  14-1: 0] dac_buf [0:(1<<RSZ)-1] ;
reg   [  14-1: 0] dac_rd    ;
reg   [  14-1: 0] dac_rdat  ;
reg   [ RSZ-1: 0] dac_rp    ;
reg   [RSZ+15: 0] dac_pnt   ; // read pointer
reg   [RSZ+15: 0] dac_pntp  ; // previour read pointer
wire  [RSZ+16: 0] dac_npnt  ; // next read pointer
wire  [RSZ+16: 0] dac_npnt_sub ;
wire              dac_npnt_sub_neg;

reg   [  28-1: 0] dac_mult  ;
reg   [  15-1: 0] dac_sum   ;
reg              dac_do       ;
reg  [  RSZ-1: 0] len_cnt      ;

// read
always @(posedge dac_clk_i)
begin
   buf_rpnt_o <= dac_pnt[RSZ+15:16];
   dac_rp     <= dac_pnt[RSZ+15:16];
   dac_rd     <= dac_do?dac_buf[dac_rp] :14'h1fff;
///   dac_rd     <= dac_do?14'h3fff :14'b0;
///   dac_rd     <= dac_do? len_cnt:14'h1fff;
   dac_rdat   <= dac_rd ;  // improve timing
end

// write
always @(posedge dac_clk_i)
if (buf_we_i)  dac_buf[buf_addr_i] <= buf_wdata_i[14-1:0] ;

// read-back
always @(posedge dac_clk_i)
buf_rdata_o <= dac_buf[buf_addr_i] ;
/*
// scale and offset
always @(posedge dac_clk_i)
begin
   dac_mult <= $signed(dac_rdat) * $signed({1'b0,set_amp_i}) ;
   dac_sum  <= $signed(dac_mult[28-1:13]) + $signed(set_dc_i) ;

   // saturation
   if (set_zero_i)  
        dac_o <= 14'h0;
   else             
        dac_o <= ^dac_sum[15-1:15-2] ? {dac_sum[15-1], {13{~dac_sum[15-1]}}} : dac_sum[13:0];
end
*/
always @(posedge dac_clk_i)
        dac_o <= dac_rdat;

///==== delay cnt =====================
wire dly_cnt_zero;
reg  [ 2: 0] z_dly_cnt_zero    ;

assign dly_cnt_zero=(dly_cnt==32'h0);

reg on_cnt_dly;
reg  [  32-1: 0] dly_cnt      ;
///reg  [  32-1: 0] prev_dly_cnt      ;
wire             dac_trig     ;
reg              dac_trigr    ;
reg set_cnt_len;
///wire set_cnt_len;
/* 
always @(posedge dac_clk_i) 
if (dac_rstn_i == 1'b0) 
    set_cnt_len   <= 1'b0 ;
else 
     set_cnt_len <= (!z_dly_cnt_zero[2])&z_dly_cnt_zero[1] ;
     */
always @*
   set_cnt_len <= (!z_dly_cnt_zero[2]) & z_dly_cnt_zero[1] ;
///assign   set_cnt_len = (!z_dly_cnt_zero[2]) & z_dly_cnt_zero[0] ;

always @(posedge dac_clk_i) 
if (dac_rstn_i == 1'b0) 
    z_dly_cnt_zero   <= 3'b0 ;
else 
     z_dly_cnt_zero <= {z_dly_cnt_zero[1:0],dly_cnt_zero} ;

always @(posedge dac_clk_i) 
begin
   if (dac_rstn_i == 1'b0) 
    begin
      dly_cnt   <= delay_i ;
      on_cnt_dly   <=  1'b0 ;
 ////     set_cnt_len <=  1'b0 ;
    end
   else 
     begin
     if (dac_trigr )
        begin
        dly_cnt   <= delay_i ;
        on_cnt_dly <= 1'b1 ;
        end
      else if (on_cnt_dly)
      begin
      if((dly_cnt==32'h0))
        begin
  ///      set_cnt_len <=  1'b1 ;
        on_cnt_dly   <=  1'b0 ;
        end
      else
        begin
 ////       set_cnt_len <=  1'b0 ;
        dly_cnt<=  dly_cnt-1'b1 ;
        end  
      end
    end
end
///=========== cnt len ===========================
////reg  [  RSZ-1: 0] len_cnt      ;
reg on_cnt_len;
///reg              dac_do       ;

always @(posedge dac_clk_i) 
begin
   if (dac_rstn_i == 1'b0) 
    begin
      len_cnt   <= len_i ;
      on_cnt_len   <=  1'b0 ;
      dac_do <=  1'b0 ;
    end
   else 
     begin
     if (set_cnt_len )
        begin
        len_cnt   <= len_i ;
        on_cnt_len <= 1'b1 ;
        dac_do <=  1'b1 ;
        end
      else if (on_cnt_len)
      begin
      if((len_cnt==14'h0)||set_rst_i||dac_trigr)
        begin
        on_cnt_len   <=  1'b0 ;
        dac_do <=  1'b0 ;
        end
      else
        begin
        len_cnt<=  len_cnt-1'b1 ;
        end  
      end
    end
end

//---------------------------------------------------------------------------------
//
//  read pointer & state machine

reg              trig_in      ;
wire             ext_trig_p   ;
wire             ext_trig_n   ;

///reg  [  RSZ-1: 0] len_cnt      ;

////reg  [  32-1: 0] dly_cnt      ;

////reg  [   8-1: 0] dly_tick     ;

///reg              dac_do       ;
///wire             dac_trig     ;
///reg              dac_trigr    ;

// state machine
always @(posedge dac_clk_i) 
begin
   if (dac_rstn_i == 1'b0) 
   begin
 ///     len_cnt   <= RSZ'h100 ;
      trig_in   <=  1'b0 ;
      dac_pntp  <= {RSZ+16{1'b0}} ;
      dac_trigr <=  1'b0 ;
   end
   else begin
      // count number of table read cycles
      dac_pntp  <= dac_pnt;
      dac_trigr <= dac_trig; // ignore trigger when count
      // trigger arrived
      case (trig_src_i)
 ///         3'd1 : trig_in <= trig_sw_i   ; // sw
          3'd2 : trig_in <= ext_trig_p  ; // external positive edge
          3'd3 : trig_in <= ext_trig_n  ; // external negative edge
 ///      default : trig_in <= 1'b0        ;
       default : trig_in <= ext_trig_p        ;
      endcase
    end
end

assign dac_trig =  trig_in ;

///assign dac_npnt_sub = dac_npnt - {1'b0,set_size_i} - 1;
///assign dac_npnt_sub_neg = dac_npnt_sub[RSZ+16];

// read pointer logic
always @(posedge dac_clk_i)
if (dac_rstn_i == 1'b0) begin
   dac_pnt  <= {RSZ+16{1'b0}};
end else begin
 ///  if (set_rst_i || (dac_trig && !dac_do)) // manual reset or start
  if (set_rst_i || (set_cnt_len)) // manual reset or start
 ///     dac_pnt <= set_ofs_i;
     dac_pnt <= {RSZ+16{1'b0}};
   else if (dac_do) 
    begin
 ///     if (~dac_npnt_sub_neg)  
 ///       dac_pnt <= set_wrap_i ? dac_npnt_sub : set_ofs_i; // wrap or go to start
 ////     else                    
        dac_pnt <= dac_npnt[RSZ+15:0]; // normal increase
   end
end

////???assign dac_npnt = dac_pnt + set_step_i;
assign dac_npnt = dac_pnt + 32'h10000;
assign trig_done_o = trig_in;

//---------------------------------------------------------------------------------
//
//  External trigger
reg  [  3-1: 0] ext_trig_in    ;
reg  [  2-1: 0] ext_trig_dp    ;
reg  [  2-1: 0] ext_trig_dn    ;
reg  [ 20-1: 0] ext_trig_debp  ;
reg  [ 20-1: 0] ext_trig_debn  ;

always @(posedge dac_clk_i) begin
   if (dac_rstn_i == 1'b0) begin
      ext_trig_in   <=  3'h0 ;
      ext_trig_dp   <=  2'h0 ;
      ext_trig_dn   <=  2'h0 ;
      ext_trig_debp <= 20'h0 ;
      ext_trig_debn <= 20'h0 ;
   end
   else begin
      //----------- External trigger
      // synchronize FFs
      ext_trig_in <= {ext_trig_in[1:0],trig_ext_i} ;

      // look for input changes
      if ((ext_trig_debp == 20'h0) && (ext_trig_in[1] && !ext_trig_in[2]))
         ext_trig_debp <= 20'd100 ; // ~0.5ms
      else if (ext_trig_debp != 20'h0)
         ext_trig_debp <= ext_trig_debp - 20'd1 ;

      if ((ext_trig_debn == 20'h0) && (!ext_trig_in[1] && ext_trig_in[2]))
         ext_trig_debn <= 20'd100 ; // ~0.5ms
      else if (ext_trig_debn != 20'h0)
         ext_trig_debn <= ext_trig_debn - 20'd1 ;

      // update output values
      ext_trig_dp[1] <= ext_trig_dp[0] ;
      if (ext_trig_debp == 20'h0)
         ext_trig_dp[0] <= ext_trig_in[1] ;

      ext_trig_dn[1] <= ext_trig_dn[0] ;
      if (ext_trig_debn == 20'h0)
         ext_trig_dn[0] <= ext_trig_in[1] ;
   end
end

assign ext_trig_p = (ext_trig_dp == 2'b01) ;
assign ext_trig_n = (ext_trig_dn == 2'b10) ;

assign tst_o[0] = trig_ext_i ;///dac_trigr ;///trig_ext_i ;
assign tst_o[1] = on_cnt_dly ;///dac_trigr ;
assign tst_o[2] = dac_rp[0];///set_cnt_len;///dac_do ;///on_cnt_dly ;
///assign tst_o[2] = on_cnt_len ;
assign tst_o[3] = dac_pnt[16];///dac_do ;
assign dac_trigr_o=dac_trigr;
endmodule
