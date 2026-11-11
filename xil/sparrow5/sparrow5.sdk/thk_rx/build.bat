@echo off
setlocal
set GCCBIN=C:\Xilinx\SDK\2019.1\gnu\aarch32\nt\gcc-arm-none-eabi\bin
set GCC=%GCCBIN%\arm-none-eabi-gcc.exe
set BSP=D:\proj\emu_thick\sparrow5\sparrow5.sdk\thk_fsbl_bsp\ps7_cortexa9_0
set SRC=D:\proj\emu_thick\sparrow5\sparrow5.sdk\thk_rx
set OBJ=%SRC%\obj
set CFLAGS=-Wall -O2 -g -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I"%BSP%\include" -I"D:\proj\emu_thick\protocol"
if not exist "%OBJ%" mkdir "%OBJ%"
"%GCC%" %CFLAGS% -c "%SRC%\src\main.c" -o "%OBJ%\main.o" || exit /b 1
"%GCC%" %CFLAGS% -c "%SRC%\src\usb_ehci.c" -o "%OBJ%\usb_ehci.o" || exit /b 1
"%GCC%" %CFLAGS% -c "%SRC%\src\rtl_udp.c" -o "%OBJ%\rtl_udp.o" || exit /b 1
"%GCC%" %CFLAGS% -c "%SRC%\src\usb_com.c" -o "%OBJ%\usb_com.o" || exit /b 1
"%GCC%" %CFLAGS% -c "%SRC%\src\mcp4725.c" -o "%OBJ%\mcp4725.o" || exit /b 1
"%GCC%" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wl,-build-id=none -specs="%SRC%\..\thk_rtos\Debug\Xilinx.spec" -T "%SRC%\src\lscript.ld" -L"%BSP%\lib" -o "%SRC%\thk_rx.elf" "%OBJ%\main.o" "%OBJ%\usb_ehci.o" "%OBJ%\rtl_udp.o" "%OBJ%\usb_com.o" "%OBJ%\mcp4725.o" -Wl,--start-group,-lxil,-lgcc,-lc,--end-group
exit /b %errorlevel%
