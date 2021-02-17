`timescale 1ns/100ps

module ad_data_in_cmos #(
/// parameters
/// parameter   IODELAY_ENABLE = 1,
/// parameter   IODELAY_CTRL = 0,
parameter   IODELAY_GROUP = "dev_if_delay_group",
parameter   REFCLK_FREQUENCY = 200) (

  // data interface

  input               i_clk,
  input               i_data,
  output              o_data,
  
  // delay-data interface

  input               up_clk,
  input               up_dld,
  input       [ 4:0]  up_dwdata,
  output      [ 4:0]  up_drdata,

  // delay-cntrl interface

  input               delay_clk,
  input               delay_rst,
  output              delay_locked);


 wire         t_data_ibuf_s;
 wire         t_data_idelay_s;
 

IBUF i_data_ (
    .I (i_data),
    .O (t_data_ibuf_s));

(* IODELAY_GROUP = IODELAY_GROUP *)
  IDELAYE2 #(
    .CINVCTRL_SEL ("FALSE"),
    .DELAY_SRC ("IDATAIN"),
    .HIGH_PERFORMANCE_MODE ("FALSE"),
    .IDELAY_TYPE ("VAR_LOAD"),
    .IDELAY_VALUE (0),
    .REFCLK_FREQUENCY (REFCLK_FREQUENCY),
    .PIPE_SEL ("FALSE"),
    .SIGNAL_PATTERN ("DATA"))
  i_rx_data_idelay (
    .CE (1'b0),
    .INC (1'b0),
    .DATAIN (1'b0),
    .LDPIPEEN (1'b0),
    .CINVCTRL (1'b0),
    .REGRST (1'b0),
    .C (up_clk),
    .IDATAIN (t_data_ibuf_s),
    .DATAOUT (o_data),
    .LD (up_dld),
    .CNTVALUEIN (up_dwdata),
    .CNTVALUEOUT (up_drdata));
    
///====================================================
endmodule