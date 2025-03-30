onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib mem_2kx16_opt

do {wave.do}

view wave
view structure
view signals

do {mem_2kx16.udo}

run -all

quit -force
