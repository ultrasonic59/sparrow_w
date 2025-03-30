
`timescale 1ns/100ps

module dac_data_out (
  // data interface
  input               tx_clk,
  input               tx_data_p,
  input               tx_data_m,
  output              tx_data_out_p,
  output              tx_data_out_m);
  // internal signals
  wire                tx_data_oddr_s;
    // delay controller
  // transmit data interface, oddr -> odelay -> obuf

  ODDR #(.DDR_CLK_EDGE ("SAME_EDGE")) i_tx_data_oddr (
    .CE (1'b1),
    .R (1'b0),
    .S (1'b0),
    .C (tx_clk),
    .D1 (tx_data_m),
    .D2 (tx_data_p),
    .Q (tx_data_oddr_s));
  // obuf

  OBUFDS 
   #(.IOSTANDARD("LVDS_25")
///        ,.SLEW("SLOW")
        )
  i_tx_data_obuf (
    .I (tx_data_oddr_s),
    .O (tx_data_out_p),
    .OB (tx_data_out_m));
endmodule

// ***************************************************************************
// ***************************************************************************
