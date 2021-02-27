################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/FreeRTOS_tick_config.c \
../src/led.c \
../src/main.c \
../src/main_application.c \
../src/platform.c 

S_UPPER_SRCS += \
../src/FreeRTOS_asm_vectors.S 

OBJS += \
./src/FreeRTOS_asm_vectors.o \
./src/FreeRTOS_tick_config.o \
./src/led.o \
./src/main.o \
./src/main_application.o \
./src/platform.o 

S_UPPER_DEPS += \
./src/FreeRTOS_asm_vectors.d 

C_DEPS += \
./src/FreeRTOS_tick_config.d \
./src/led.d \
./src/main.d \
./src/main_application.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -I../../FreeRTOS_bsp/ps7_cortexa9_0/include -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src\FreeRTOS_Source\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src\FreeRTOS_Source\portable\GCC\ARM_CA9" -c -fmessage-length=0 -MT"$@" -IC:/Users/remon/freeRTOS/freeRTOS_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -I../../FreeRTOS_bsp/ps7_cortexa9_0/include -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src\FreeRTOS_Source\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\FreeRTOS1\simple_demo\src\FreeRTOS_Source\portable\GCC\ARM_CA9" -c -fmessage-length=0 -MT"$@" -IC:/Users/remon/freeRTOS/freeRTOS_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


