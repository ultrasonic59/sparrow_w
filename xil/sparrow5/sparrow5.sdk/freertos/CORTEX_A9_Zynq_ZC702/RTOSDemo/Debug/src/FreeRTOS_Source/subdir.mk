################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/event_groups.c \
D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/list.c \
D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/queue.c \
D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/tasks.c \
D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/timers.c 

OBJS += \
./src/FreeRTOS_Source/event_groups.o \
./src/FreeRTOS_Source/list.o \
./src/FreeRTOS_Source/queue.o \
./src/FreeRTOS_Source/tasks.o \
./src/FreeRTOS_Source/timers.o 

C_DEPS += \
./src/FreeRTOS_Source/event_groups.d \
./src/FreeRTOS_Source/list.d \
./src/FreeRTOS_Source/queue.d \
./src/FreeRTOS_Source/tasks.d \
./src/FreeRTOS_Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS_Source/event_groups.o: D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/event_groups.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\include" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FreeRTOS_Source/list.o: D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/list.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\include" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FreeRTOS_Source/queue.o: D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/queue.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\include" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FreeRTOS_Source/tasks.o: D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/tasks.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\include" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FreeRTOS_Source/timers.o: D:/proj/velograph/octopuz/xil/octopuz3a/octopuz3a.sdk/FreeRTOS1/button_with_interrupts/src/FreeRTOS_Source/timers.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\include" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


