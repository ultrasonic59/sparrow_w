set_property PACKAGE_PIN E12 [get_ports i_adc_sdout]
set_property IOSTANDARD LVCMOS25 [get_ports i_adc_sdout]
###===================== ADC =================================================
###============================================
set_property PACKAGE_PIN P15 [get_ports {i_adc_dat[1]}]
set_property PACKAGE_PIN R15 [get_ports {i_adc_dat[0]}]

set_property PACKAGE_PIN N13 [get_ports {i_adc_dat[3]}]
set_property PACKAGE_PIN N14 [get_ports {i_adc_dat[2]}]


set_property PACKAGE_PIN H11 [get_ports {i_adc_dat[5]}]
set_property PACKAGE_PIN J11 [get_ports {i_adc_dat[4]}]
set_property PACKAGE_PIN L14 [get_ports {i_adc_dat[7]}]
set_property PACKAGE_PIN M14 [get_ports {i_adc_dat[6]}]
set_property PACKAGE_PIN L15 [get_ports {i_adc_dat[9]}]
set_property PACKAGE_PIN M15 [get_ports {i_adc_dat[8]}]

set_property PACKAGE_PIN P13 [get_ports {i_adc_dat[11]}]
set_property PACKAGE_PIN P14 [get_ports {i_adc_dat[10]}]

set_property PACKAGE_PIN R12 [get_ports {i_adc_dat[13]}]
set_property PACKAGE_PIN R13 [get_ports {i_adc_dat[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {i_adc_dat[*]}]

###set_property PACKAGE_PIN P10 [get_ports i_adc_clko_p]
###set_property PACKAGE_PIN R10 [get_ports i_adc_clko_m]
set_property PACKAGE_PIN P10 [get_ports i_adc_clko]
set_property PACKAGE_PIN R10 [get_ports i_zzz]
set_property IOSTANDARD LVCMOS25 [get_ports i_adc_clko]
set_property IOSTANDARD LVCMOS25 [get_ports i_zzz]

set_property PACKAGE_PIN K13 [get_ports o_adc_clk_p]
set_property PACKAGE_PIN L13 [get_ports o_adc_clk_n]
set_property IOSTANDARD LVDS_25 [get_ports o_adc_clk_*]

