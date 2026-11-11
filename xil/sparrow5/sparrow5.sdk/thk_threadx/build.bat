@echo off
setlocal
set PATH=C:\Xilinx\SDK\2019.1\gnu\aarch32\nt\gcc-arm-none-eabi\bin;%PATH%
cd /d D:\proj\emu_thick\sparrow5\sparrow5.sdk\thk_threadx
if not exist Debug mkdir Debug
cd Debug
del /q *.o thk_threadx.elf 2>nul
call C:\Xilinx\SDK\2019.1\bin\a9-linaro-pre-build-step.bat
if errorlevel 1 exit /b 1
set CFLAGS=-Wall -O0 -g3 -c -fmessage-length=0 -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../sparrow_bsp/ps7_cortexa9_0/include -I../src/threadx/common/inc -I../src/threadx/port/inc
for %%F in (..\src\*.c ..\src\threadx\common\src\*.c) do (
  arm-none-eabi-gcc %CFLAGS% -o %%~nF.o %%F
  if errorlevel 1 exit /b 1
)
for %%F in (..\src\*.S ..\src\threadx\port\src\*.S) do (
  arm-none-eabi-gcc %CFLAGS% -o %%~nF.o %%F
  if errorlevel 1 exit /b 1
)
arm-none-eabi-gcc -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wl,-build-id=none -specs=Xilinx.spec -Wl,-T -Wl,../src/lscript.ld -L../../sparrow_bsp/ps7_cortexa9_0/lib *.o -Wl,--start-group,-lxil,-lgcc,-lc,--end-group -o thk_threadx.elf
if errorlevel 1 exit /b 1
arm-none-eabi-size thk_threadx.elf
