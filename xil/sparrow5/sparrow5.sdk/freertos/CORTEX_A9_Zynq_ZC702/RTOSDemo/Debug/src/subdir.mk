################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/FreeRTOS_tick_config.c \
../src/ParTest.c \
../src/main.c \
../src/platform.c \
../src/printf-stdarg.c 

S_UPPER_SRCS += \
../src/FreeRTOS_asm_vectors.S 

OBJS += \
./src/FreeRTOS_asm_vectors.o \
./src/FreeRTOS_tick_config.o \
./src/ParTest.o \
./src/main.o \
./src/platform.o \
./src/printf-stdarg.o 

S_UPPER_DEPS += \
./src/FreeRTOS_asm_vectors.d 

C_DEPS += \
./src/FreeRTOS_tick_config.d \
./src/ParTest.d \
./src/main.d \
./src/platform.d \
./src/printf-stdarg.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\FreeRTOS1\button_with_interrupts\src\FreeRTOS_Source\portable" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


