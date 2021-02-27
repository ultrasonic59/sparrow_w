#============================================
###===================== IO =================================================
###set_property -dict {PACKAGE_PIN F12 IOSTANDARD LVCMOS25} [get_ports {i_ext[0]}]
set_property PACKAGE_PIN G14 [get_ports {io_ext[0]}]
set_property PACKAGE_PIN H14 [get_ports {io_ext[1]}]
set_property PACKAGE_PIN G12 [get_ports {io_ext[2]}]
set_property PACKAGE_PIN H13 [get_ports {io_ext[3]}]
set_property PACKAGE_PIN F13 [get_ports {io_ext[4]}]
set_property PACKAGE_PIN F14 [get_ports {io_ext[5]}]
set_property PACKAGE_PIN F15 [get_ports {io_ext[6]}]
set_property PACKAGE_PIN G15 [get_ports {io_ext[7]}]
set_property PACKAGE_PIN F12 [get_ports {io_ext[8]}]
set_property PACKAGE_PIN E13 [get_ports {io_ext[9]}]
set_property PACKAGE_PIN E11 [get_ports {io_ext[10]}]
###set_property PACKAGE_PIN E12 [get_ports {io_ext[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {io_ext[*]}]
###===================== DAC =================================================
set_property PACKAGE_PIN K11 [get_ports o_dac_clk_p]
set_property PACKAGE_PIN K12 [get_ports o_dac_clk_m]
set_property IOSTANDARD LVDS_25 [get_ports o_dac_clk_k]
set_property IOSTANDARD LVDS_25 [get_ports o_dac_clk_m]

set_property PACKAGE_PIN P8 [get_ports o_dac_dclk_p]
set_property PACKAGE_PIN P9 [get_ports o_dac_dclk_m]
set_property IOSTANDARD LVDS_25 [get_ports o_dac_dclk_*]

set_property PACKAGE_PIN R7 [get_ports {o_dac_dat_p[6]}]
set_property PACKAGE_PIN R8 [get_ports {o_dac_dat_m[6]}]

set_property PACKAGE_PIN N11 [get_ports {o_dac_dat_p[5]}]
set_property PACKAGE_PIN N12 [get_ports {o_dac_dat_m[5]}]


set_property PACKAGE_PIN L12 [get_ports {o_dac_dat_p[4]}]
set_property PACKAGE_PIN M12 [get_ports {o_dac_dat_m[4]}]

set_property PACKAGE_PIN M10 [get_ports {o_dac_dat_p[3]}]
set_property PACKAGE_PIN M11 [get_ports {o_dac_dat_m[3]}]

set_property PACKAGE_PIN M9 [get_ports {o_dac_dat_p[2]}]
set_property PACKAGE_PIN N9 [get_ports {o_dac_dat_m[2]}]

set_property PACKAGE_PIN N7 [get_ports {o_dac_dat_p[1]}]
set_property PACKAGE_PIN N8 [get_ports {o_dac_dat_m[1]}]

set_property PACKAGE_PIN P11 [get_ports {o_dac_dat_p[0]}]
set_property PACKAGE_PIN R11 [get_ports {o_dac_dat_m[0]}]
set_property IOSTANDARD LVDS_25 [get_ports {o_dac_dat_p[*]}]
set_property IOSTANDARD LVDS_25 [get_ports {o_dac_dat_m[*]}]

###================= serial dac adc= =====================================================
set_property PACKAGE_PIN J13 [get_ports o_adc_dac_sck]
set_property IOSTANDARD LVCMOS25 [get_ports o_adc_dac_sck]
set_property PACKAGE_PIN J14 [get_ports io_adc_dac_sdio]
set_property IOSTANDARD LVCMOS25 [get_ports io_adc_dac_sdio]
set_property PACKAGE_PIN J15 [get_ports o_dac_en]
set_property IOSTANDARD LVCMOS25 [get_ports o_dac_en]
set_property PACKAGE_PIN K15 [get_ports o_adc_en]
set_property IOSTANDARD LVCMOS25 [get_ports o_adc_en]
###======================================================================
set_property PACKAGE_PIN G11 [get_ports i_sync]
set_property IOSTANDARD LVCMOS25 [get_ports i_sync]
###======================================================================

##==================================
set_property PACKAGE_PIN H12 [get_ports i_clk24]
set_property IOSTANDARD LVCMOS25 [get_ports i_clk24]
set_property PIO_DIRECTION INPUT [get_ports i_clk24]
###set_property CLOCK_DEDICATED_ROUTE FALSE [get_ports i_clk]
###set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets i_clk24_IBUF]
set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets i_clk24]
###set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets pll1/inst/clk_in1_pll24_80]
##==================================
set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets ps_sys_clk]
