onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib pll24_80_opt

do {wave.do}

view wave
view structure
view signals

do {pll24_80.udo}

run -all

quit -force
