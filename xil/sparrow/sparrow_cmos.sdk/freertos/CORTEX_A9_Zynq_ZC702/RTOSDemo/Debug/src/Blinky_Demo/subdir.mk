################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Blinky_Demo/main_blinky.c 

OBJS += \
./src/Blinky_Demo/main_blinky.o 

C_DEPS += \
./src/Blinky_Demo/main_blinky.d 


# Each subdirectory must supply rules for building sources it contributes
src/Blinky_Demo/%.o: ../src/Blinky_Demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


