// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.1.3 (win64) Build 2644227 Wed Sep  4 09:45:24 MDT 2019
// Date        : Mon Jan 20 00:02:00 2025
// Host        : vovag running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               D:/proj/velograph/sparrow_w/xil/sparrow/sparrow_smos.srcs/sources_1/ip/ila_0/ila_0_stub.v
// Design      : ila_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z007sclg225-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "ila,Vivado 2019.1" *)
module ila_0(clk, probe0, probe1, probe2, probe3, probe4, probe5)
/* synthesis syn_black_box black_box_pad_pin="clk,probe0[15:0],probe1[13:0],probe2[12:0],probe3[15:0],probe4[12:0],probe5[13:0]" */;
  input clk;
  input [15:0]probe0;
  input [13:0]probe1;
  input [12:0]probe2;
  input [15:0]probe3;
  input [12:0]probe4;
  input [13:0]probe5;
endmodule
