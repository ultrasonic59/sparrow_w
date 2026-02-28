################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../brd/brook_kbd_brd.c 

OBJS += \
./brd/brook_kbd_brd.o 

C_DEPS += \
./brd/brook_kbd_brd.d 


# Each subdirectory must supply rules for building sources it contributes
brd/%.o: ../brd/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\proj\velograph\brookesia_w\v6\kbd\brook_kbd\Debug" -I"D:\proj\velograph\brookesia_w\v6\kbd\brook_kbd\Core" -I"D:\proj\velograph\brookesia_w\v6\kbd\brook_kbd\User" -I"D:\proj\velograph\brookesia_w\v6\kbd\brook_kbd\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

