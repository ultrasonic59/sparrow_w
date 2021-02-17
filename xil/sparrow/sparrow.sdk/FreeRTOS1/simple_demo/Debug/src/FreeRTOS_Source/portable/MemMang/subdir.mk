################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FreeRTOS_Source/portable/MemMang/heap_4.c 

OBJS += \
./src/FreeRTOS_Source/portable/MemMang/heap_4.o 

C_DEPS += \
./src/FreeRTOS_Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS_Source/portable/MemMang/%.o: ../src/FreeRTOS_Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -I../../FreeRTOS_bsp/ps7_cortexa9_0/include -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src\FreeRTOS_Source\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src\FreeRTOS_Source\portable\GCC\ARM_CA9" -c -fmessage-length=0 -MT"$@" -IC:/Users/remon/freeRTOS/freeRTOS_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


