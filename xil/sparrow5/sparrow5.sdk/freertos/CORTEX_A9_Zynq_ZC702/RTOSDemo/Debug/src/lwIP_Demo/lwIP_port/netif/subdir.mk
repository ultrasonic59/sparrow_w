################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lwIP_Demo/lwIP_port/netif/xadapter.c \
../src/lwIP_Demo/lwIP_port/netif/xemacpsif.c \
../src/lwIP_Demo/lwIP_port/netif/xemacpsif_dma.c \
../src/lwIP_Demo/lwIP_port/netif/xemacpsif_hw.c \
../src/lwIP_Demo/lwIP_port/netif/xemacpsif_physpeed.c \
../src/lwIP_Demo/lwIP_port/netif/xpqueue.c \
../src/lwIP_Demo/lwIP_port/netif/xtopology_g.c 

OBJS += \
./src/lwIP_Demo/lwIP_port/netif/xadapter.o \
./src/lwIP_Demo/lwIP_port/netif/xemacpsif.o \
./src/lwIP_Demo/lwIP_port/netif/xemacpsif_dma.o \
./src/lwIP_Demo/lwIP_port/netif/xemacpsif_hw.o \
./src/lwIP_Demo/lwIP_port/netif/xemacpsif_physpeed.o \
./src/lwIP_Demo/lwIP_port/netif/xpqueue.o \
./src/lwIP_Demo/lwIP_port/netif/xtopology_g.o 

C_DEPS += \
./src/lwIP_Demo/lwIP_port/netif/xadapter.d \
./src/lwIP_Demo/lwIP_port/netif/xemacpsif.d \
./src/lwIP_Demo/lwIP_port/netif/xemacpsif_dma.d \
./src/lwIP_Demo/lwIP_port/netif/xemacpsif_hw.d \
./src/lwIP_Demo/lwIP_port/netif/xemacpsif_physpeed.d \
./src/lwIP_Demo/lwIP_port/netif/xpqueue.d \
./src/lwIP_Demo/lwIP_port/netif/xtopology_g.d 


# Each subdirectory must supply rules for building sources it contributes
src/lwIP_Demo/lwIP_port/netif/%.o: ../src/lwIP_Demo/lwIP_port/netif/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src\lwIP_Demo\lwIP_port\include" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src\lwIP_Demo\lwIP_port\netif" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src\Full_Demo" -I"D:\proj\velograph\octopuz\xil\octopuz3a\FreeRTOS-Plus\Source\FreeRTOS-Plus-CLI" -I"D:\proj\velograph\octopuz\xil\octopuz3a\octopuz3a.sdk\freertos\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -Wextra -ffunction-sections -fdata-sections -I../../RTOSDemo_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


