@echo off
setlocal
set PATH=C:\Xilinx\SDK\2019.1\gnu\aarch32\nt\gcc-arm-none-eabi\bin;%PATH%
set ROOT=D:\proj\brook_w\xil\sparrow5\sparrow5.sdk
set BSP=%ROOT%\thk_fsbl_bsp\ps7_cortexa9_0
set SRC=%ROOT%\thk_sd_loader
if not exist "%SRC%\Debug" mkdir "%SRC%\Debug"
cd /d "%SRC%\Debug"
call C:\Xilinx\SDK\2019.1\bin\a9-linaro-pre-build-step.bat
if errorlevel 1 exit /b 1
set CFLAGS=-Wall -O2 -g -c -fmessage-length=0 -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I%BSP%\include
arm-none-eabi-gcc %CFLAGS% -o main.o ..\src\main.c
if errorlevel 1 exit /b 1
arm-none-eabi-gcc -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wl,-build-id=none -specs=Xilinx.spec -Wl,-T -Wl,../src/lscript.ld -L%BSP%\lib -o thk_sd_loader.elf main.o -Wl,--start-group,-lxil,-lxilffs,-lgcc,-lc,--end-group
if errorlevel 1 exit /b 1
arm-none-eabi-size thk_sd_loader.elf
