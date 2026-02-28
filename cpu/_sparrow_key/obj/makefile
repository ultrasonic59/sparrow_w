################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include brd/subdir.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
brook_kbd.hex \

SECONDARY_LIST += \
brook_kbd.lst \

SECONDARY_SIZE += \
brook_kbd.siz \


# All Target
all: brook_kbd.elf secondary-outputs

# Tool invocations
brook_kbd.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "D:\proj\velograph\brookesia_w\v6\kbd\brook_kbd\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"brook_kbd.map" --specs=nano.specs --specs=nosys.specs -o "brook_kbd.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lprintf
	@	@
brook_kbd.hex: brook_kbd.elf
	@	riscv-none-embed-objcopy -O ihex "brook_kbd.elf"  "brook_kbd.hex"
	@	@
brook_kbd.lst: brook_kbd.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "brook_kbd.elf" > "brook_kbd.lst"
	@	@
brook_kbd.siz: brook_kbd.elf
	@	riscv-none-embed-size --format=berkeley "brook_kbd.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) brook_kbd.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
