-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.1 (win64) Build 2552052 Fri May 24 14:49:42 MDT 2019
-- Date        : Tue Jan 21 19:39:58 2020
-- Host        : vova-PC running 64-bit Service Pack 1  (build 7601)
-- Command     : write_vhdl -force -mode synth_stub -rename_top ila_0 -prefix
--               ila_0_ ila_0_stub.vhdl
-- Design      : ila_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z010clg225-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity ila_0 is
  Port ( 
    clk : in STD_LOGIC;
    probe0 : in STD_LOGIC_VECTOR ( 15 downto 0 );
    probe1 : in STD_LOGIC_VECTOR ( 13 downto 0 );
    probe2 : in STD_LOGIC_VECTOR ( 12 downto 0 );
    probe3 : in STD_LOGIC_VECTOR ( 15 downto 0 );
    probe4 : in STD_LOGIC_VECTOR ( 12 downto 0 );
    probe5 : in STD_LOGIC_VECTOR ( 13 downto 0 )
  );

end ila_0;

architecture stub of ila_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk,probe0[15:0],probe1[13:0],probe2[12:0],probe3[15:0],probe4[12:0],probe5[13:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "ila,Vivado 2019.1";
begin
end;