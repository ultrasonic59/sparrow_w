################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/adc.c \
../User/ch32v00x_it.c \
../User/main.c \
../User/my_misc.c \
../User/system_ch32v00x.c \
../User/timers.c \
../User/uart.c \
../User/zzz.c 

OBJS += \
./User/adc.o \
./User/ch32v00x_it.o \
./User/main.o \
./User/my_misc.o \
./User/system_ch32v00x.o \
./User/timers.o \
./User/uart.o \
./User/zzz.o 

C_DEPS += \
./User/adc.d \
./User/ch32v00x_it.d \
./User/main.d \
./User/my_misc.d \
./User/system_ch32v00x.d \
./User/timers.d \
./User/uart.d \
./User/zzz.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\proj\velograph\brookesia_w\v6\kbd\brook_kbd\Debug" -I"D:\proj\velograph\brookesia_w\v6\kbd\brook_kbd\Core" -I"D:\proj\velograph\brookesia_w\v6\kbd\brook_kbd\User" -I"D:\proj\velograph\brookesia_w\v6\kbd\brook_kbd\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

