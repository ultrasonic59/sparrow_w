// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.1.3 (win64) Build 2644227 Wed Sep  4 09:45:24 MDT 2019
// Date        : Mon Jan 20 00:01:26 2025
// Host        : vovag running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               D:/proj/velograph/sparrow_w/xil/sparrow/sparrow_smos.srcs/sources_1/ip/mem_2kx16/mem_2kx16_stub.v
// Design      : mem_2kx16
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z007sclg225-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "blk_mem_gen_v8_4_3,Vivado 2019.1" *)
module mem_2kx16(clka, ena, wea, addra, dina, douta, clkb, enb, web, addrb, 
  dinb, doutb)
/* synthesis syn_black_box black_box_pad_pin="clka,ena,wea[0:0],addra[12:0],dina[15:0],douta[15:0],clkb,enb,web[0:0],addrb[12:0],dinb[15:0],doutb[15:0]" */;
  input clka;
  input ena;
  input [0:0]wea;
  input [12:0]addra;
  input [15:0]dina;
  output [15:0]douta;
  input clkb;
  input enb;
  input [0:0]web;
  input [12:0]addrb;
  input [15:0]dinb;
  output [15:0]doutb;
endmodule
