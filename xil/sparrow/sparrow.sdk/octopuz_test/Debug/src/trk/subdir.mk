################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/trk/drv_utrk.c 

OBJS += \
./src/trk/drv_utrk.o 

C_DEPS += \
./src/trk/drv_utrk.d 


# Each subdirectory must supply rules for building sources it contributes
src/trk/%.o: ../src/trk/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I../../octopuz_test_bsp/ps7_cortexa9_0/include -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\hdlc" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\trk" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\def" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\lib" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


