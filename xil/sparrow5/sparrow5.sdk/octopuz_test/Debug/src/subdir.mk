################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/octopuz_test.c \
../src/pc_interf.c 

OBJS += \
./src/octopuz_test.o \
./src/pc_interf.o 

C_DEPS += \
./src/octopuz_test.d \
./src/pc_interf.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I../../octopuz_test_bsp/ps7_cortexa9_0/include -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\hdlc" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\trk" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\def" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\include" -I"E:\proj\velograph\umka2\xil\umka2\umka2.sdk\octopuz_test\src\lib" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


