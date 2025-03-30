################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FreeRTOS_Source/portable/heap_4.c \
../src/FreeRTOS_Source/portable/port.c 

S_UPPER_SRCS += \
../src/FreeRTOS_Source/portable/portASM.S 

OBJS += \
./src/FreeRTOS_Source/portable/heap_4.o \
./src/FreeRTOS_Source/portable/port.o \
./src/FreeRTOS_Source/portable/portASM.o 

S_UPPER_DEPS += \
./src/FreeRTOS_Source/portable/portASM.d 

C_DEPS += \
./src/FreeRTOS_Source/portable/heap_4.d \
./src/FreeRTOS_Source/portable/port.d 


# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS_Source/portable/%.o: ../src/FreeRTOS_Source/portable/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -IC:/Users/remon/freeRTOS/freeRTOS_bsp/ps7_cortexa9_0/include -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src\FreeRTOS_Source\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\button_with_interrupts\src" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -IC:/Users/remon/AppData/Roaming/Xilinx/Vivado/freertos/test_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FreeRTOS_Source/portable/%.o: ../src/FreeRTOS_Source/portable/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -IC:/Users/remon/freeRTOS/freeRTOS_bsp/ps7_cortexa9_0/include -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src\FreeRTOS_Source\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\button_with_interrupts\src" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -IC:/Users/remon/AppData/Roaming/Xilinx/Vivado/freertos/test_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


