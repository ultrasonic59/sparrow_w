module i_sniffer #(
  parameter RSZ = 14  // RAM size 2^RSZ
)(
   // ADC
   input                 clk_i       ,  // ADC clock
   input                 rstn_i      ,  // ADC reset - active low
   input      [ 14-1: 0] adc_a_i         ,  // ADC data CHA
   input      [ 14-1: 0] adc_b_i         ,  // ADC data CHB
   // trigger sources
   // usb
   input                 dm_i      ,  // 
   input                 dp_i      ,  // 

   // AXI0 master
   output                axi0_clk_o      ,  // global clock
   output                axi0_rstn_o     ,  // global reset
   output     [ 32-1: 0] axi0_waddr_o    ,  // system write address
   output     [ 64-1: 0] axi0_wdata_o    ,  // system write data
   output     [  8-1: 0] axi0_wsel_o     ,  // system write byte select
   output                axi0_wvalid_o   ,  // system write data valid
   output     [  4-1: 0] axi0_wlen_o     ,  // system write burst length
   output                axi0_wfixed_o   ,  // system write burst type (fixed / incremental)
   input                 axi0_werr_i     ,  // system write error
   input                 axi0_wrdy_i     ,  // system write ready

   // AXI1 master
   output                axi1_clk_o      ,  // global clock
   output                axi1_rstn_o     ,  // global reset
   output     [ 32-1: 0] axi1_waddr_o    ,  // system write address
   output     [ 64-1: 0] axi1_wdata_o    ,  // system write data
   output     [  8-1: 0] axi1_wsel_o     ,  // system write byte select
   output                axi1_wvalid_o   ,  // system write data valid
   output     [  4-1: 0] axi1_wlen_o     ,  // system write burst length
   output                axi1_wfixed_o   ,  // system write burst type (fixed / incremental)
   input                 axi1_werr_i     ,  // system write error
   input                 axi1_wrdy_i     ,  // system write ready
  
   // System bus
   input      [ 32-1: 0] sys_addr      ,  // bus saddress
   input      [ 32-1: 0] sys_wdata     ,  // bus write data
   input                 sys_wen       ,  // bus write enable
   input                 sys_ren       ,  // bus read enable
   output reg [ 32-1: 0] sys_rdata     ,  // bus read data
   output reg            sys_err       ,  // bus error indicator
   output reg            sys_ack          // bus acknowledge signal
);

reg             adc_arm_do   ;
reg             adc_rst_do   ;

//---------------------------------------------------------------------------------
//  Input filtering

///wire [ 14-1: 0] adc_a_filt_in  ;
///wire [ 14-1: 0] adc_a_filt_out ;
///wire [ 14-1: 0] adc_b_filt_in  ;
///wire [ 14-1: 0] adc_b_filt_out ;
/*
reg  [ 18-1: 0] set_a_filt_aa  ;
reg  [ 25-1: 0] set_a_filt_bb  ;
reg  [ 25-1: 0] set_a_filt_kk  ;
reg  [ 25-1: 0] set_a_filt_pp  ;
reg  [ 18-1: 0] set_b_filt_aa  ;
reg  [ 25-1: 0] set_b_filt_bb  ;
reg  [ 25-1: 0] set_b_filt_kk  ;
reg  [ 25-1: 0] set_b_filt_pp  ;
*/
////assign adc_a_filt_out = adc_a_i ;
///assign adc_b_filt_out = adc_b_i ;
//---------------------------------------------------------------------------------
reg [31:0]cur_time;
always @(posedge clk_i) 
begin
if (rstn_i == 1'b0) 
   begin
   cur_time<=32'b0;
   end
else
   begin
   cur_time<=cur_time+1'b1;
   end
end   


reg  [ 14-1: 0] adc_a_dat     ;
reg  [ 14-1: 0] adc_b_dat     ;
always @(posedge clk_i) 
begin
if (rstn_i == 1'b0) 
   begin
   adc_b_dat<=14'b0;
   adc_a_dat<=14'b0;
   end
else
   begin
   adc_b_dat<=cur_time[21:8];
   adc_a_dat<=cur_time[19:6];
   end
end   

//---------------------------------------------------------------------------------
//  ADC buffer RAM

reg   [  14-1: 0] adc_a_buf [0:(1<<RSZ)-1] ;
reg   [  14-1: 0] adc_b_buf [0:(1<<RSZ)-1] ;
reg   [  14-1: 0] adc_a_rd      ;
reg   [  14-1: 0] adc_b_rd      ;
reg   [ RSZ-1: 0] adc_wp        ;
reg   [ RSZ-1: 0] adc_raddr     ;
reg   [ RSZ-1: 0] adc_a_raddr   ;
reg   [ RSZ-1: 0] adc_b_raddr   ;
reg   [   4-1: 0] adc_rval      ;
wire              adc_rd_dv     ;
reg               adc_we        ;
reg               adc_we_keep   ;
reg               adc_trig      ;

reg   [ RSZ-1: 0] adc_wp_trig   ;
reg   [ RSZ-1: 0] adc_wp_cur    ;
reg   [  32-1: 0] set_dly       ;
reg   [  32-1: 0] adc_we_cnt    ;
reg   [  32-1: 0] adc_dly_cnt   ;
reg               adc_dly_do    ;
////reg    [ 20-1: 0] set_deb_len   ; // debouncing length (glitch free time after a posedge)
///==================================
// Write
always @(posedge clk_i) begin
   if (rstn_i == 1'b0) begin
      adc_wp      <= {RSZ{1'b0}};
      adc_we      <=  1'b0      ;
      adc_wp_trig <= {RSZ{1'b0}};
      adc_wp_cur  <= {RSZ{1'b0}};
      adc_we_cnt  <= 32'h0      ;
      adc_dly_cnt <= 32'h0      ;
      adc_dly_do  <=  1'b0      ;
   end
   else begin
      if (adc_arm_do)
         adc_we <= 1'b1 ;
      else if (((adc_dly_do || adc_trig) && (adc_dly_cnt == 32'h0) && ~adc_we_keep) || adc_rst_do) //delayed reached or reset
         adc_we <= 1'b0 ;

      // count how much data was written into the buffer before trigger
      if (adc_rst_do | adc_arm_do)
         adc_we_cnt <= 32'h0;
      if (adc_we & ~adc_dly_do  & ~&adc_we_cnt)
         adc_we_cnt <= adc_we_cnt + 1;

      if (adc_rst_do)
         adc_wp <= {RSZ{1'b0}};
      else if (adc_we )
         adc_wp <= adc_wp + 1;

      if (adc_rst_do)
         adc_wp_trig <= {RSZ{1'b0}};
      else if (adc_trig && !adc_dly_do)
         adc_wp_trig <= adc_wp_cur ; // save write pointer at trigger arrival

      if (adc_rst_do)
         adc_wp_cur <= {RSZ{1'b0}};
      else if (adc_we )
         adc_wp_cur <= adc_wp ; // save current write pointer


      if (adc_trig)
         adc_dly_do  <= 1'b1 ;
      else if ((adc_dly_do && (adc_dly_cnt == 32'b0)) || adc_rst_do || adc_arm_do) //delayed reached or reset
         adc_dly_do  <= 1'b0 ;

      if (adc_dly_do && adc_we )
         adc_dly_cnt <= adc_dly_cnt - 1;
      else if (!adc_dly_do)
         adc_dly_cnt <= set_dly ;

   end
end

always @(posedge clk_i) begin
   if (adc_we ) begin
      adc_a_buf[adc_wp] <= adc_a_dat ;
      adc_b_buf[adc_wp] <= adc_b_dat ;
   end
end

// Read
always @(posedge clk_i) begin
   if (rstn_i == 1'b0)
      adc_rval <= 4'h0 ;
   else
      adc_rval <= {adc_rval[2:0], (sys_ren || sys_wen)};
end
assign adc_rd_dv = adc_rval[3];

always @(posedge clk_i) begin
   adc_raddr   <= sys_addr[RSZ+1:2] ; // address synchronous to clock
   adc_a_raddr <= adc_raddr     ; // double register 
   adc_b_raddr <= adc_raddr     ; // otherwise memory corruption at reading
   adc_a_rd    <= adc_a_buf[adc_a_raddr] ;
   adc_b_rd    <= adc_b_buf[adc_b_raddr] ;
end




//---------------------------------------------------------------------------------
//
//  AXI CHA connection

reg  [ 32-1: 0] set_a_axi_start    ;
reg  [ 32-1: 0] set_a_axi_stop     ;
reg  [ 32-1: 0] set_a_axi_dly      ;
reg             set_a_axi_en       ;
reg  [ 32-1: 0] set_a_axi_trig     ;
reg  [ 32-1: 0] set_a_axi_cur      ;
reg             axi_a_we           ;
reg  [ 64-1: 0] axi_a_dat          ;
reg  [  2-1: 0] axi_a_dat_sel      ;
reg  [  1-1: 0] axi_a_dat_dv       ;
reg  [ 32-1: 0] axi_a_dly_cnt      ;
reg             axi_a_dly_do       ;
wire            axi_a_clr          ;
wire [ 32-1: 0] axi_a_cur_addr     ;

assign axi_a_clr = adc_rst_do ;


always @(posedge axi0_clk_o) begin
   if (axi0_rstn_o == 1'b0) begin
      axi_a_dat_sel <=  2'h0 ;
      axi_a_dat_dv  <=  1'b0 ;
      axi_a_dly_cnt <= 32'h0 ;
      axi_a_dly_do  <=  1'b0 ;
   end
   else begin
      if (adc_arm_do && set_a_axi_en)
         axi_a_we <= 1'b1 ;
      else if (((axi_a_dly_do || adc_trig) && (axi_a_dly_cnt == 32'h0)) || adc_rst_do) //delayed reached or reset
         axi_a_we <= 1'b0 ;

      if (adc_trig && axi_a_we)
         axi_a_dly_do  <= 1'b1 ;
      else if ((axi_a_dly_do && (axi_a_dly_cnt == 32'b0)) || axi_a_clr || adc_arm_do) //delayed reached or reset
         axi_a_dly_do  <= 1'b0 ;

      if (axi_a_dly_do && axi_a_we )
         axi_a_dly_cnt <= axi_a_dly_cnt - 1;
      else if (!axi_a_dly_do)
         axi_a_dly_cnt <= set_a_axi_dly ;

      if (axi_a_clr)
         axi_a_dat_sel <= 2'h0 ;
      else if (axi_a_we )
         axi_a_dat_sel <= axi_a_dat_sel + 2'h1 ;

      axi_a_dat_dv <= axi_a_we && (axi_a_dat_sel == 2'b11)  ;
   end

   if (axi_a_we ) begin
      if (axi_a_dat_sel == 2'b00) axi_a_dat[ 16-1:  0] <= $signed(adc_a_dat);
      if (axi_a_dat_sel == 2'b01) axi_a_dat[ 32-1: 16] <= $signed(adc_a_dat);
      if (axi_a_dat_sel == 2'b10) axi_a_dat[ 48-1: 32] <= $signed(adc_a_dat);
      if (axi_a_dat_sel == 2'b11) axi_a_dat[ 64-1: 48] <= $signed(adc_a_dat);
   end

   if (axi_a_clr)
      set_a_axi_trig <= {RSZ{1'b0}};
   else if (adc_trig && !axi_a_dly_do && axi_a_we)
      set_a_axi_trig <= {axi_a_cur_addr[32-1:3],axi_a_dat_sel,1'b0} ; // save write pointer at trigger arrival

   if (axi_a_clr)
      set_a_axi_cur <= set_a_axi_start ;
   else if (axi0_wvalid_o)
      set_a_axi_cur <= axi_a_cur_addr ;
end

axi_wr_fifo #(
  .DW  (  64    ), // data width (8,16,...,1024)
  .AW  (  32    ), // address width
  .FW  (   8    )  // address width of FIFO pointers
) i_wr0 (
   // global signals
  .axi_clk_i          (  axi0_clk_o        ), // global clock
  .axi_rstn_i         (  axi0_rstn_o       ), // global reset

   // Connection to AXI master
  .axi_waddr_o        (  axi0_waddr_o      ), // write address
  .axi_wdata_o        (  axi0_wdata_o      ), // write data
  .axi_wsel_o         (  axi0_wsel_o       ), // write byte select
  .axi_wvalid_o       (  axi0_wvalid_o     ), // write data valid
  .axi_wlen_o         (  axi0_wlen_o       ), // write burst length
  .axi_wfixed_o       (  axi0_wfixed_o     ), // write burst type (fixed / incremental)
  .axi_werr_i         (  axi0_werr_i       ), // write error
  .axi_wrdy_i         (  axi0_wrdy_i       ), // write ready

   // data and configuration
  .wr_data_i          (  axi_a_dat         ), // write data
  .wr_val_i           (  axi_a_dat_dv      ), // write data valid
  .ctrl_start_addr_i  (  set_a_axi_start   ), // range start address
  .ctrl_stop_addr_i   (  set_a_axi_stop    ), // range stop address
  .ctrl_trig_size_i   (  4'hF              ), // trigger level
  .ctrl_wrap_i        (  1'b1              ), // start from begining when reached stop
  .ctrl_clr_i         (  axi_a_clr         ), // clear / flush
  .stat_overflow_o    (                    ), // overflow indicator
  .stat_cur_addr_o    (  axi_a_cur_addr    ), // current write address
  .stat_write_data_o  (                    )  // write data indicator
);

assign axi0_clk_o  = clk_i ;
assign axi0_rstn_o = rstn_i;

//---------------------------------------------------------------------------------
//
//  AXI CHB connection

reg  [ 32-1: 0] set_b_axi_start    ;
reg  [ 32-1: 0] set_b_axi_stop     ;
reg  [ 32-1: 0] set_b_axi_dly      ;
reg             set_b_axi_en       ;
reg  [ 32-1: 0] set_b_axi_trig     ;
reg  [ 32-1: 0] set_b_axi_cur      ;
reg             axi_b_we           ;
reg  [ 64-1: 0] axi_b_dat          ;
reg  [  2-1: 0] axi_b_dat_sel      ;
reg  [  1-1: 0] axi_b_dat_dv       ;
reg  [ 32-1: 0] axi_b_dly_cnt      ;
reg             axi_b_dly_do       ;
wire            axi_b_clr          ;
wire [ 32-1: 0] axi_b_cur_addr     ;

assign axi_b_clr = adc_rst_do ;


always @(posedge axi1_clk_o) begin
   if (axi1_rstn_o == 1'b0) begin
      axi_b_dat_sel <=  2'h0 ;
      axi_b_dat_dv  <=  1'b0 ;
      axi_b_dly_cnt <= 32'h0 ;
      axi_b_dly_do  <=  1'b0 ;
   end
   else begin
      if (adc_arm_do && set_b_axi_en)
         axi_b_we <= 1'b1 ;
      else if (((axi_b_dly_do || adc_trig) && (axi_b_dly_cnt == 32'h0)) || adc_rst_do) //delayed reached or reset
         axi_b_we <= 1'b0 ;

      if (adc_trig && axi_b_we)
         axi_b_dly_do  <= 1'b1 ;
      else if ((axi_b_dly_do && (axi_b_dly_cnt == 32'b0)) || axi_b_clr || adc_arm_do) //delayed reached or reset
         axi_b_dly_do  <= 1'b0 ;

      if (axi_b_dly_do && axi_b_we )
         axi_b_dly_cnt <= axi_b_dly_cnt - 1;
      else if (!axi_b_dly_do)
         axi_b_dly_cnt <= set_b_axi_dly ;

      if (axi_b_clr)
         axi_b_dat_sel <= 2'h0 ;
      else if (axi_b_we )
         axi_b_dat_sel <= axi_b_dat_sel + 2'h1 ;

      axi_b_dat_dv <= axi_b_we && (axi_b_dat_sel == 2'b11)  ;
   end

   if (axi_b_we ) begin
      if (axi_b_dat_sel == 2'b00) axi_b_dat[ 16-1:  0] <= $signed(adc_b_dat);
      if (axi_b_dat_sel == 2'b01) axi_b_dat[ 32-1: 16] <= $signed(adc_b_dat);
      if (axi_b_dat_sel == 2'b10) axi_b_dat[ 48-1: 32] <= $signed(adc_b_dat);
      if (axi_b_dat_sel == 2'b11) axi_b_dat[ 64-1: 48] <= $signed(adc_b_dat);
   end

   if (axi_b_clr)
      set_b_axi_trig <= {RSZ{1'b0}};
   else if (adc_trig && !axi_b_dly_do && axi_b_we)
      set_b_axi_trig <= {axi_b_cur_addr[32-1:3],axi_b_dat_sel,1'b0} ; // save write pointer at trigger arrival

   if (axi_b_clr)
      set_b_axi_cur <= set_b_axi_start ;
   else if (axi1_wvalid_o)
      set_b_axi_cur <= axi_b_cur_addr ;
end

axi_wr_fifo #(
  .DW  (  64    ), // data width (8,16,...,1024)
  .AW  (  32    ), // address width
  .FW  (   8    )  // address width of FIFO pointers
) i_wr1 (
   // global signals
  .axi_clk_i          (  axi1_clk_o        ), // global clock
  .axi_rstn_i         (  axi1_rstn_o       ), // global reset

   // Connection to AXI master
  .axi_waddr_o        (  axi1_waddr_o      ), // write address
  .axi_wdata_o        (  axi1_wdata_o      ), // write data
  .axi_wsel_o         (  axi1_wsel_o       ), // write byte select
  .axi_wvalid_o       (  axi1_wvalid_o     ), // write data valid
  .axi_wlen_o         (  axi1_wlen_o       ), // write burst length
  .axi_wfixed_o       (  axi1_wfixed_o     ), // write burst type (fixed / incremental)
  .axi_werr_i         (  axi1_werr_i       ), // write error
  .axi_wrdy_i         (  axi1_wrdy_i       ), // write ready

   // data and configuration
  .wr_data_i          (  axi_b_dat         ), // write data
  .wr_val_i           (  axi_b_dat_dv      ), // write data valid
  .ctrl_start_addr_i  (  set_b_axi_start   ), // range start address
  .ctrl_stop_addr_i   (  set_b_axi_stop    ), // range stop address
  .ctrl_trig_size_i   (  4'hF              ), // trigger level
  .ctrl_wrap_i        (  1'b1              ), // start from begining when reached stop
  .ctrl_clr_i         (  axi_b_clr         ), // clear / flush
  .stat_overflow_o    (                    ), // overflow indicator
  .stat_cur_addr_o    (  axi_b_cur_addr    ), // current write address
  .stat_write_data_o  (                    )  // write data indicator
);

assign axi1_clk_o  = clk_i ;
assign axi1_rstn_o = rstn_i;

//---------------------------------------------------------------------------------
//  Trigger source selector

reg               adc_trig_ap      ;
reg               adc_trig_an      ;
reg               adc_trig_bp      ;
reg               adc_trig_bn      ;
reg               adc_trig_sw      ;
reg   [   4-1: 0] set_trig_src     ;
wire              ext_trig_p       ;
wire              ext_trig_n       ;
wire              asg_trig_p       ;
wire              asg_trig_n       ;

always @(posedge clk_i)
if (rstn_i == 1'b0) begin
   adc_arm_do    <= 1'b0 ;
   adc_rst_do    <= 1'b0 ;
   adc_trig_sw   <= 1'b0 ;
   set_trig_src  <= 4'h0 ;
   adc_trig      <= 1'b0 ;
end else begin
   adc_arm_do  <= sys_wen && (sys_addr[19:0]==20'h0) && sys_wdata[0] ; // SW ARM
   adc_rst_do  <= sys_wen && (sys_addr[19:0]==20'h0) && sys_wdata[1] ;
   adc_trig_sw <= sys_wen && (sys_addr[19:0]==20'h4) && (sys_wdata[3:0]==4'h1); // SW trigger

      if (sys_wen && (sys_addr[19:0]==20'h4))
         set_trig_src <= sys_wdata[3:0] ;
      else if (((adc_dly_do || adc_trig) && (adc_dly_cnt == 32'h0)) || adc_rst_do) //delayed reached or reset
         set_trig_src <= 4'h0 ;

   case (set_trig_src)
       4'd1 : adc_trig <= adc_trig_sw   ; // manual
       4'd2 : adc_trig <= adc_trig_ap   ; // A ch rising edge
       4'd3 : adc_trig <= adc_trig_an   ; // A ch falling edge
       4'd4 : adc_trig <= adc_trig_bp   ; // B ch rising edge
       4'd5 : adc_trig <= adc_trig_bn   ; // B ch falling edge
       4'd6 : adc_trig <= ext_trig_p    ; // external - rising edge
       4'd7 : adc_trig <= ext_trig_n    ; // external - falling edge
       4'd8 : adc_trig <= asg_trig_p    ; // ASG - rising edge
       4'd9 : adc_trig <= asg_trig_n    ; // ASG - falling edge
    default : adc_trig <= 1'b0          ;
   endcase
end

/*
//---------------------------------------------------------------------------------
//  Trigger created from input signal

reg  [  2-1: 0] adc_scht_ap  ;
reg  [  2-1: 0] adc_scht_an  ;
reg  [  2-1: 0] adc_scht_bp  ;
reg  [  2-1: 0] adc_scht_bn  ;
reg  [ 14-1: 0] set_a_tresh  ;
reg  [ 14-1: 0] set_a_treshp ;
reg  [ 14-1: 0] set_a_treshm ;
reg  [ 14-1: 0] set_b_tresh  ;
reg  [ 14-1: 0] set_b_treshp ;
reg  [ 14-1: 0] set_b_treshm ;
reg  [ 14-1: 0] set_a_hyst   ;
reg  [ 14-1: 0] set_b_hyst   ;

always @(posedge clk_i)
if (rstn_i == 1'b0) begin
   adc_scht_ap  <=  2'h0 ;
   adc_scht_an  <=  2'h0 ;
   adc_scht_bp  <=  2'h0 ;
   adc_scht_bn  <=  2'h0 ;
   adc_trig_ap  <=  1'b0 ;
   adc_trig_an  <=  1'b0 ;
   adc_trig_bp  <=  1'b0 ;
   adc_trig_bn  <=  1'b0 ;
end else begin
   set_a_treshp <= set_a_tresh + set_a_hyst ; // calculate positive
   set_a_treshm <= set_a_tresh - set_a_hyst ; // and negative treshold
   set_b_treshp <= set_b_tresh + set_b_hyst ;
   set_b_treshm <= set_b_tresh - set_b_hyst ;
   adc_scht_ap[1] <= adc_scht_ap[0] ;
   adc_scht_an[1] <= adc_scht_an[0] ;
   adc_scht_bp[1] <= adc_scht_bp[0] ;
   adc_scht_bn[1] <= adc_scht_bn[0] ;

   adc_trig_ap <= adc_scht_ap[0] && !adc_scht_ap[1] ; // make 1 cyc pulse 
   adc_trig_an <= adc_scht_an[0] && !adc_scht_an[1] ;
   adc_trig_bp <= adc_scht_bp[0] && !adc_scht_bp[1] ;
   adc_trig_bn <= adc_scht_bn[0] && !adc_scht_bn[1] ;
end
*/
////===============================================================================
////===============================================================================
reg [7:0]rdata;
reg rdata_ready;
reg [3:0]rbyte_cnt;
//fix current USB line values
reg [7:0]dp_input;
reg [7:0]dm_input;
wire EOP;
wire usb_reset;
reg speed = 1'b0;
reg [2:0]receiver_cnt=0;
reg last_fixed_line=0;
reg receiver_enabled=0;
wire enable;
reg [15:0]rst_counter;
wire in_reset; 
wire [1:0]line_input;

assign enable=1;
always @(posedge clk_i)
begin
	dp_input <= { dp_input[6:0], dp_i };
	dm_input <= { dm_input[6:0], dm_i };
end


//enable receiver on raising edge of signal line
always @(posedge clk_i or posedge EOP )
begin
if(EOP)
	receiver_enabled <= 1'b0;
else
    if(in_reset)
		receiver_enabled <= 1'b0;
	else
	if( line_input[0] )
		receiver_enabled <= 1'b1 & enable;
end

//both dp/dm signals are in zero during reset/se0/no-attach
wire end_of_se0; 
assign end_of_se0 = ( (dp_input | dm_input) == 8'h01) ;
assign EOP  = end_of_se0;
always @(posedge clk_i)
if( end_of_se0 )
  speed <= dp_input[0];
assign in_reset = rst_counter[15];
always @(posedge clk_i)
if(dp_input | dm_input)
    rst_counter <= 0;
else
    if(~in_reset)
        rst_counter <= rst_counter+1;

reg [1:0]rst_sr;
always @(posedge clk_i)
	rst_sr <= {rst_sr[0],in_reset};

//impulse
assign usb_reset = (rst_sr==2'b01);
//we do not have possibility to work with differential inputs
//we just select line to spy on depending on speed
assign line_input = speed ? dm_input[1:0] : dp_input[1:0];

//change of dp/dm lines defines strobing
wire lcross; 
assign lcross = (line_input[0] ^ line_input[1]);
//generate clocks with this counter
reg [5:0]clk_counter;
reg  strobe;

//make strobe impulse
always @(posedge clk_i)
begin		
//every edge on line resynchronizes receiver clock
if(lcross | ~receiver_enabled)
    begin
	clk_counter <= 0;
	end
else
	if(speed)
		clk_counter[2:0] <= (clk_counter[2:0] + 1'b1);
	else
		clk_counter <= (clk_counter + 1'b1);
end
//catch counter somewhere in the middle of bit transfer
always @(posedge clk_i)
strobe <= speed ? clk_counter[2:0]==3'h2 : clk_counter==6'h20;
	
//next bit received is ONE if no signal change or ZERO if signal changed
wire next_bit; 
assign next_bit = (last_fixed_line == line_input[1]);
//calc number of sequental ONEs in received bit stream
reg [2:0]num_ones;
always @(posedge clk_i)
begin
if(strobe & receiver_enabled)
	begin
	if(next_bit)
		num_ones <= num_ones + 1'b1;
	else
		num_ones <= 0;
	end
end
//flag which mean that zero should be removed from bit stream
wire do_remove_zero; 
assign do_remove_zero = (num_ones==6);
//receiver process
always @(posedge clk_i or posedge EOP )
begin
if(EOP)
    begin
	//kind of reset
	receiver_cnt <= 0;
	rbyte_cnt <= 0;
	last_fixed_line <= 1'b0;
	rdata <= 0;
	rdata_ready <= 1'b0;
	end
else
	begin
	if(strobe & receiver_enabled & (!do_remove_zero))
		begin
			//decode NRZI
			//shift-in ONE  if older and new values are same
			//shift-in ZERO if older and new values are different
			//BUT (bitstuffling) do not shift-in one ZERO after 6 ONEs
			rdata  <= { next_bit, rdata[7:1] };
			receiver_cnt <= receiver_cnt + 1'b1;	
		end

		//set write-enable signal (write into receiver buffer)
		rdata_ready <= (receiver_cnt == 7) & (strobe & receiver_enabled & (!do_remove_zero));
		
		//remember last fixed signal,
		if(strobe & receiver_enabled)
			last_fixed_line <= line_input[1];
		//count nomber of received bytes
		if(rdata_ready)
			rbyte_cnt <= rbyte_cnt + 1'b1;
	end
end
////===============================================================================
////===============================================================================
//  System bus connection

always @(posedge clk_i)
if (rstn_i == 1'b0) begin
   adc_we_keep   <=   1'b0      ;
 ///  set_a_tresh   <=  14'd5000   ;
 ///  set_b_tresh   <= -14'd5000   ;
   set_dly       <=  32'd0      ;
///   set_dec       <=  17'd1      ;
 ///  set_a_hyst    <=  14'd20     ;
 ///  set_b_hyst    <=  14'd20     ;
////   set_avg_en    <=   1'b1      ;
 /*  
   set_a_filt_aa <=  18'h0      ;
   set_a_filt_bb <=  25'h0      ;
   set_a_filt_kk <=  25'hFFFFFF ;
   set_a_filt_pp <=  25'h0      ;
   set_b_filt_aa <=  18'h0      ;
   set_b_filt_bb <=  25'h0      ;
   set_b_filt_kk <=  25'hFFFFFF ;
   set_b_filt_pp <=  25'h0      ;
 */  
////   set_deb_len   <=  20'd62500  ;
   set_a_axi_en  <=   1'b0      ;
   set_b_axi_en  <=   1'b0      ;
end else begin
   if (sys_wen) begin
      if (sys_addr[19:0]==20'h00)   adc_we_keep   <= sys_wdata[     3] ;

 ///     if (sys_addr[19:0]==20'h08)   set_a_tresh   <= sys_wdata[14-1:0] ;
 ///     if (sys_addr[19:0]==20'h0C)   set_b_tresh   <= sys_wdata[14-1:0] ;
      if (sys_addr[19:0]==20'h10)   set_dly       <= sys_wdata[32-1:0] ;
 ///     if (sys_addr[19:0]==20'h14)   set_dec       <= sys_wdata[17-1:0] ;
 ////     if (sys_addr[19:0]==20'h20)   set_a_hyst    <= sys_wdata[14-1:0] ;
 ///     if (sys_addr[19:0]==20'h24)   set_b_hyst    <= sys_wdata[14-1:0] ;
 ////     if (sys_addr[19:0]==20'h28)   set_avg_en    <= sys_wdata[     0] ;
/*
      if (sys_addr[19:0]==20'h30)   set_a_filt_aa <= sys_wdata[18-1:0] ;
      if (sys_addr[19:0]==20'h34)   set_a_filt_bb <= sys_wdata[25-1:0] ;
      if (sys_addr[19:0]==20'h38)   set_a_filt_kk <= sys_wdata[25-1:0] ;
      if (sys_addr[19:0]==20'h3C)   set_a_filt_pp <= sys_wdata[25-1:0] ;
      if (sys_addr[19:0]==20'h40)   set_b_filt_aa <= sys_wdata[18-1:0] ;
      if (sys_addr[19:0]==20'h44)   set_b_filt_bb <= sys_wdata[25-1:0] ;
      if (sys_addr[19:0]==20'h48)   set_b_filt_kk <= sys_wdata[25-1:0] ;
      if (sys_addr[19:0]==20'h4C)   set_b_filt_pp <= sys_wdata[25-1:0] ;
*/
      if (sys_addr[19:0]==20'h50)   set_a_axi_start <= sys_wdata[32-1:0] ;
      if (sys_addr[19:0]==20'h54)   set_a_axi_stop  <= sys_wdata[32-1:0] ;
      if (sys_addr[19:0]==20'h58)   set_a_axi_dly   <= sys_wdata[32-1:0] ;
      if (sys_addr[19:0]==20'h5C)   set_a_axi_en    <= sys_wdata[     0] ;

      if (sys_addr[19:0]==20'h70)   set_b_axi_start <= sys_wdata[32-1:0] ;
      if (sys_addr[19:0]==20'h74)   set_b_axi_stop  <= sys_wdata[32-1:0] ;
      if (sys_addr[19:0]==20'h78)   set_b_axi_dly   <= sys_wdata[32-1:0] ;
      if (sys_addr[19:0]==20'h7C)   set_b_axi_en    <= sys_wdata[     0] ;

 ////     if (sys_addr[19:0]==20'h90)   set_deb_len <= sys_wdata[20-1:0] ;
   end
end

wire sys_en;
assign sys_en = sys_wen | sys_ren;

always @(posedge clk_i)
if (rstn_i == 1'b0) begin
   sys_err <= 1'b0 ;
   sys_ack <= 1'b0 ;
end else begin
   sys_err <= 1'b0 ;

   casez (sys_addr[19:0])
     20'h00000 : begin sys_ack <= sys_en;          sys_rdata <= {{32- 4{1'b0}}, adc_we_keep               // do not disarm on 
                                                                              , adc_dly_do                // trigger status
                                                                              , 1'b0                      // reset
                                                                              , adc_we}             ; end // arm

     20'h00004 : begin sys_ack <= sys_en;          sys_rdata <= {{32- 4{1'b0}}, set_trig_src}       ; end 

////     20'h00008 : begin sys_ack <= sys_en;          sys_rdata <= {{32-14{1'b0}}, set_a_tresh}        ; end
////     20'h0000C : begin sys_ack <= sys_en;          sys_rdata <= {{32-14{1'b0}}, set_b_tresh}        ; end
     20'h00010 : begin sys_ack <= sys_en;          sys_rdata <= {               set_dly}            ; end
 ////    20'h00014 : begin sys_ack <= sys_en;          sys_rdata <= {{32-17{1'b0}}, set_dec}            ; end

     20'h00018 : begin sys_ack <= sys_en;          sys_rdata <= {{32-RSZ{1'b0}}, adc_wp_cur}        ; end
     20'h0001C : begin sys_ack <= sys_en;          sys_rdata <= {{32-RSZ{1'b0}}, adc_wp_trig}       ; end

////     20'h00020 : begin sys_ack <= sys_en;          sys_rdata <= {{32-14{1'b0}}, set_a_hyst}         ; end
////     20'h00024 : begin sys_ack <= sys_en;          sys_rdata <= {{32-14{1'b0}}, set_b_hyst}         ; end

////     20'h00028 : begin sys_ack <= sys_en;          sys_rdata <= {{32- 1{1'b0}}, set_avg_en}         ; end

     20'h0002C : begin sys_ack <= sys_en;          sys_rdata <=                 adc_we_cnt          ; end
/*
     20'h00030 : begin sys_ack <= sys_en;          sys_rdata <= {{32-18{1'b0}}, set_a_filt_aa}      ; end
     20'h00034 : begin sys_ack <= sys_en;          sys_rdata <= {{32-25{1'b0}}, set_a_filt_bb}      ; end
     20'h00038 : begin sys_ack <= sys_en;          sys_rdata <= {{32-25{1'b0}}, set_a_filt_kk}      ; end
     20'h0003C : begin sys_ack <= sys_en;          sys_rdata <= {{32-25{1'b0}}, set_a_filt_pp}      ; end
     20'h00040 : begin sys_ack <= sys_en;          sys_rdata <= {{32-18{1'b0}}, set_b_filt_aa}      ; end
     20'h00044 : begin sys_ack <= sys_en;          sys_rdata <= {{32-25{1'b0}}, set_b_filt_bb}      ; end
     20'h00048 : begin sys_ack <= sys_en;          sys_rdata <= {{32-25{1'b0}}, set_b_filt_kk}      ; end
     20'h0004C : begin sys_ack <= sys_en;          sys_rdata <= {{32-25{1'b0}}, set_b_filt_pp}      ; end
*/
     20'h00050 : begin sys_ack <= sys_en;          sys_rdata <=                 set_a_axi_start     ; end
     20'h00054 : begin sys_ack <= sys_en;          sys_rdata <=                 set_a_axi_stop      ; end
     20'h00058 : begin sys_ack <= sys_en;          sys_rdata <=                 set_a_axi_dly       ; end
     20'h0005C : begin sys_ack <= sys_en;          sys_rdata <= {{32- 1{1'b0}}, set_a_axi_en}       ; end
     20'h00060 : begin sys_ack <= sys_en;          sys_rdata <=                 set_a_axi_trig      ; end
     20'h00064 : begin sys_ack <= sys_en;          sys_rdata <=                 set_a_axi_cur       ; end

     20'h00070 : begin sys_ack <= sys_en;          sys_rdata <=                 set_b_axi_start     ; end
     20'h00074 : begin sys_ack <= sys_en;          sys_rdata <=                 set_b_axi_stop      ; end
     20'h00078 : begin sys_ack <= sys_en;          sys_rdata <=                 set_b_axi_dly       ; end
     20'h0007C : begin sys_ack <= sys_en;          sys_rdata <= {{32- 1{1'b0}}, set_b_axi_en}       ; end
     20'h00080 : begin sys_ack <= sys_en;          sys_rdata <=                 set_b_axi_trig      ; end
     20'h00084 : begin sys_ack <= sys_en;          sys_rdata <=                 set_b_axi_cur       ; end

 ////    20'h00090 : begin sys_ack <= sys_en;          sys_rdata <= {{32-20{1'b0}}, set_deb_len}        ; end

     20'h1???? : begin sys_ack <= adc_rd_dv;       sys_rdata <= {16'h0, 2'h0,adc_a_rd}              ; end
     20'h2???? : begin sys_ack <= adc_rd_dv;       sys_rdata <= {16'h0, 2'h0,adc_b_rd}              ; end

       default : begin sys_ack <= sys_en;          sys_rdata <=  32'h0                              ; end
   endcase
end

endmodule
