################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Full_Demo/IntQueueTimer.c \
D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/freertos/Source/FreeRTOS-Plus/Demo/Common/FreeRTOS_Plus_CLI_Demos/UARTCommandConsole.c \
../src/Full_Demo/main_full.c \
../src/Full_Demo/serial.c 

S_UPPER_SRCS += \
../src/Full_Demo/reg_test.S 

OBJS += \
./src/Full_Demo/IntQueueTimer.o \
./src/Full_Demo/UARTCommandConsole.o \
./src/Full_Demo/main_full.o \
./src/Full_Demo/reg_test.o \
./src/Full_Demo/serial.o 

S_UPPER_DEPS += \
./src/Full_Demo/reg_test.d 

C_DEPS += \
./src/Full_Demo/IntQueueTimer.d \
./src/Full_Demo/UARTCommandConsole.d \
./src/Full_Demo/main_full.d \
./src/Full_Demo/serial.d 


# Each subdirectory must supply rules for building sources it contributes
src/Full_Demo/%.o: ../src/Full_Demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src\Full_Demo" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Full_Demo/UARTCommandConsole.o: D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/freertos/Source/FreeRTOS-Plus/Demo/Common/FreeRTOS_Plus_CLI_Demos/UARTCommandConsole.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src\Full_Demo" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Full_Demo/%.o: ../src/Full_Demo/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src\Full_Demo" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


