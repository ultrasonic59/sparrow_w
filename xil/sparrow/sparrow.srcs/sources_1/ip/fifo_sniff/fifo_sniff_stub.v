// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.1 (win64) Build 2552052 Fri May 24 14:49:42 MDT 2019
// Date        : Thu Sep  5 17:58:21 2019
// Host        : vova-PC running 64-bit Service Pack 1  (build 7601)
// Command     : write_verilog -force -mode synth_stub
//               d:/proj/velograph/sparrow/xil/sparrow/sparrow.srcs/sources_1/ip/fifo_sniff/fifo_sniff_stub.v
// Design      : fifo_sniff
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z010clg225-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "fifo_generator_v13_2_4,Vivado 2019.1" *)
module fifo_sniff(clk, srst, din, wr_en, rd_en, dout, full, overflow, 
  empty, underflow, data_count)
/* synthesis syn_black_box black_box_pad_pin="clk,srst,din[8:0],wr_en,rd_en,dout[8:0],full,overflow,empty,underflow,data_count[12:0]" */;
  input clk;
  input srst;
  input [8:0]din;
  input wr_en;
  input rd_en;
  output [8:0]dout;
  output full;
  output overflow;
  output empty;
  output underflow;
  output [12:0]data_count;
endmodule
