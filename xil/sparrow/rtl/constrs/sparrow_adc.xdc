set_property PACKAGE_PIN E12 [get_ports i_adc_sdout]
set_property IOSTANDARD LVCMOS25 [get_ports i_adc_sdout]
###===================== ADC =================================================
set_property PACKAGE_PIN P10 [get_ports i_adc_clko_p]
set_property PACKAGE_PIN R10 [get_ports i_adc_clko_m]
set_property IOSTANDARD LVDS_25 [get_ports i_adc_clko_m]
set_property IOSTANDARD LVDS_25 [get_ports i_adc_clko_p]
###============================================
set_property PACKAGE_PIN P15 [get_ports {i_adc_dat_p[0]}]
set_property PACKAGE_PIN R15 [get_ports {i_adc_dat_m[0]}]

set_property PACKAGE_PIN N13 [get_ports {i_adc_dat_p[1]}]
set_property PACKAGE_PIN N14 [get_ports {i_adc_dat_m[1]}]


set_property PACKAGE_PIN H11 [get_ports {i_adc_dat_p[2]}]
set_property PACKAGE_PIN J11 [get_ports {i_adc_dat_m[2]}]
set_property PACKAGE_PIN L14 [get_ports {i_adc_dat_p[3]}]
set_property PACKAGE_PIN M14 [get_ports {i_adc_dat_m[3]}]
set_property PACKAGE_PIN L15 [get_ports {i_adc_dat_p[4]}]
set_property PACKAGE_PIN M15 [get_ports {i_adc_dat_m[4]}]

set_property PACKAGE_PIN P13 [get_ports {i_adc_dat_p[5]}]
set_property PACKAGE_PIN PJ14 [get_ports {i_adc_dat_m[5]}]

set_property PACKAGE_PIN R12 [get_ports {i_adc_dat_p[6]}]
set_property PACKAGE_PIN R13 [get_ports {i_adc_dat_m[6]}]
set_property IOSTANDARD LVDS_25 [get_ports {i_adc_dat_p[*]}]
set_property IOSTANDARD LVDS_25 [get_ports {i_adc_dat_m[*]}]

set_property PACKAGE_PIN K13 [get_ports o_adc_clk_p]
set_property PACKAGE_PIN L13 [get_ports o_adc_clk_n]
set_property IOSTANDARD LVDS_25 [get_ports o_adc_clk_*]

