onbreak {quit -force}
onerror {quit -force}

asim -t 1ps +access +r +m+sparrow_ps -L xil_defaultlib -L xpm -L unisims_ver -L unimacro_ver -L secureip -O5 xil_defaultlib.sparrow_ps xil_defaultlib.glbl

do {wave.do}

view wave
view structure

do {sparrow_ps.udo}

run -all

endsim

quit -force
