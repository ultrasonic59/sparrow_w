################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwIP_Demo/lwIP_Apps/apps/BasicSocketCommandServer/BasicSocketCommandServer.c 

OBJS += \
./src/lwIP_Demo/lwIP_Apps/apps/BasicSocketCommandServer/BasicSocketCommandServer.o 

C_DEPS += \
./src/lwIP_Demo/lwIP_Apps/apps/BasicSocketCommandServer/BasicSocketCommandServer.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwIP_Demo/lwIP_Apps/apps/BasicSocketCommandServer/%.o: ../src/lwIP_Demo/lwIP_Apps/apps/BasicSocketCommandServer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src\lwIP_Demo\lwIP_port\include" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src\lwIP_Demo\lwIP_port\netif" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src\Full_Demo" -I"D:\proj\velograph\octopuz\xil\octopuz3a\FreeRTOS-Plus\Source\FreeRTOS-Plus-CLI" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


