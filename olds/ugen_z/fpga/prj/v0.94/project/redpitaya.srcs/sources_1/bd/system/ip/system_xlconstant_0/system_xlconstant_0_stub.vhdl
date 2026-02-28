-- Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2016.4 (lin64) Build 1733598 Wed Dec 14 22:35:42 MST 2016
-- Date        : Thu Mar 16 19:17:35 2017
-- Host        : ubuntu64 running 64-bit Ubuntu 14.04.4 LTS
-- Command     : write_vhdl -force -mode synth_stub
--               /home/vovag/proj/rp_tst/RedPitaya-master/fpga/prj/v0.94/project/redpitaya.srcs/sources_1/bd/system/ip/system_xlconstant_0/system_xlconstant_0_stub.vhdl
-- Design      : system_xlconstant_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z010clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity system_xlconstant_0 is
  Port ( 
    dout : out STD_LOGIC_VECTOR ( 0 to 0 )
  );

end system_xlconstant_0;

architecture stub of system_xlconstant_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "dout[0:0]";
begin
end;
