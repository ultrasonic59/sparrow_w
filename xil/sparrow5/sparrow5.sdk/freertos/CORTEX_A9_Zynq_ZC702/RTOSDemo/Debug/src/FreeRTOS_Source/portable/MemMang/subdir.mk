################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/freertos/Source/portable/MemMang/heap_4.c 

OBJS += \
./src/FreeRTOS_Source/portable/MemMang/heap_4.o 

C_DEPS += \
./src/FreeRTOS_Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS_Source/portable/MemMang/heap_4.o: D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/freertos/Source/portable/MemMang/heap_4.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\Source\include" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\Source\portable\GCC\ARM_CA9" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


