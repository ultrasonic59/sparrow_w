################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/adc_test.c \
../src/i2c_test.c \
../src/mcp4725.c \
../src/sparrow_test.c \
../src/xiicps_polled_example.c 

OBJS += \
./src/adc_test.o \
./src/i2c_test.o \
./src/mcp4725.o \
./src/sparrow_test.o \
./src/xiicps_polled_example.o 

C_DEPS += \
./src/adc_test.d \
./src/i2c_test.d \
./src/mcp4725.d \
./src/sparrow_test.d \
./src/xiicps_polled_example.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../sparrow_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


