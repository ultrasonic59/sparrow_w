################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/uart/uart.c \
../src/uart/uart_thr.c 

OBJS += \
./src/uart/uart.o \
./src/uart/uart_thr.o 

C_DEPS += \
./src/uart/uart.d \
./src/uart/uart_thr.d 


# Each subdirectory must supply rules for building sources it contributes
src/uart/%.o: ../src/uart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I../../octopuz_test_bsp/ps7_cortexa9_0/include -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\hdlc" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\trk" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\def" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\lib" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


