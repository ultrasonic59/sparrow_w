@echo off
setlocal
cd /d "%~dp0"
if /I "%~1"=="qspi" (
    set BIF=boot_qspi.bif
    set OUT=BOOT_qspi.bin
) else if /I "%~1"=="threadx" (
    set BIF=boot_threadx.bif
    set OUT=THKTX.BIN
) else if /I "%~1"=="rtos" (
    set BIF=boot_rtos.bif
    set OUT=THKRT.BIN
) else (
    echo usage: make_boot.bat qspi^|threadx^|rtos
    exit /b 1
)
call "C:\Xilinx\SDK\2019.1\bin\bootgen.bat" -image %BIF% -arch zynq -o %OUT% -w
if errorlevel 1 exit /b 1
echo %OUT%
