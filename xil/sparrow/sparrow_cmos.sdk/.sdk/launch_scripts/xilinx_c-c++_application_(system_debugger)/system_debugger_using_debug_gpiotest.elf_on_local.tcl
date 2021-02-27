connect -url tcp:127.0.0.1:3121
source D:/proj/velograph/octopuz/xil/octopuz2/octopuz2.sdk/zebra_top_hw_platform_3/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-HS2 210249895020"} -index 0
loadhw -hw D:/proj/velograph/octopuz/xil/octopuz2/octopuz2.sdk/zebra_top_hw_platform_3/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-HS2 210249895020"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-HS2 210249895020"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-HS2 210249895020"} -index 0
dow D:/proj/velograph/octopuz/xil/octopuz2/octopuz2.sdk/gpiotest/Debug/gpiotest.elf
configparams force-mem-access 0
bpadd -addr &main
