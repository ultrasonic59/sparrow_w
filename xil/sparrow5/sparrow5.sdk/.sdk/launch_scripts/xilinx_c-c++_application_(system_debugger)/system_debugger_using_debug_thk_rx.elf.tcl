connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-HS2 210249894994"} -index 0
loadhw -hw D:/proj/brook_w/xil/sparrow5/sparrow5.sdk/sparrow5_top_hw_platform_1/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-HS2 210249894994"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-HS2 210249894994"} -index 0
dow D:/proj/brook_w/xil/sparrow5/sparrow5.sdk/thk_rx/Debug/thk_rx.elf
configparams force-mem-access 0
bpadd -addr &main
