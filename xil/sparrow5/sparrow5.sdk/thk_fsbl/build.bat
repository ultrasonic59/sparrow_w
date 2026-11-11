@echo off
setlocal
set PATH=C:\Xilinx\SDK\2019.1\gnu\aarch32\nt\gcc-arm-none-eabi\bin;%PATH%
cd /d D:\proj\emu_thick\sparrow5\sparrow5.sdk\thk_fsbl
if not exist Debug mkdir Debug
cd Debug
call C:\Xilinx\SDK\2019.1\bin\a9-linaro-pre-build-step.bat
if errorlevel 1 exit /b 1
set CFLAGS=-Wall -O0 -g3 -c -fmessage-length=0 -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../sparrow5_top_hw_platform_0 -I../../thk_fsbl_bsp/ps7_cortexa9_0/include
for %%F in (..\src\*.c) do (
  arm-none-eabi-gcc %CFLAGS% -o %%~nF.o %%F
  if errorlevel 1 exit /b 1
)
arm-none-eabi-gcc %CFLAGS% -o ps7_init.o ..\..\sparrow5_top_hw_platform_0\ps7_init.c
if errorlevel 1 exit /b 1
arm-none-eabi-gcc %CFLAGS% -o fsbl_handoff.o ..\src\fsbl_handoff.S
if errorlevel 1 exit /b 1
arm-none-eabi-gcc -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wl,-build-id=none -specs=Xilinx.spec -Wl,-T -Wl,../src/lscript.ld -L../../thk_fsbl_bsp/ps7_cortexa9_0/lib *.o -Wl,--start-group,-lxil,-lgcc,-lc,--end-group -Wl,--start-group,-lxilffs,-lxil,-lgcc,-lc,--end-group -Wl,--start-group,-lrsa,-lxil,-lgcc,-lc,--end-group -o thk_fsbl.elf
if errorlevel 1 exit /b 1
arm-none-eabi-size thk_fsbl.elf
