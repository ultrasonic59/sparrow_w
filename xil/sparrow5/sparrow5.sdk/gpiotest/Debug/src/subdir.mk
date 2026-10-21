################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/helloworld.c \
../src/platform.c 

OBJS += \
./src/helloworld.o \
./src/platform.o 

C_DEPS += \
./src/helloworld.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I../../standalone_bsp_0/ps7_cortexa9_0/include -I"C:\Xilinx\SDK\2019.1\gnu\aarch32\nt\gcc-arm-none-eabi\lib\gcc\arm-none-eabi\8.2.0\include-fixed" -I"C:\Xilinx\SDK\2019.1\gnu\aarch32\nt\gcc-arm-none-eabi\include" -I"C:\Xilinx\SDK\2019.1\gnu\aarch32\nt\gcc-arm-none-eabi\arm-none-eabi\libc\usr\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


