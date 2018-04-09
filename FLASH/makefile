################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 7
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include .external_binary/Users/b46911/workspace/Kinetis_Auto/TRK-KEA128_Lab4/FLASH/subdir.mk
-include .external_binary/Users/b46911/Documents/Kinetis/KinetisEA/KEA128/EVB/Software/TRK-KEA128_Lab4/FLASH/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
TRK-KEA128_Lab4.elf \

EXECUTABLES_QUOTED += \
"TRK-KEA128_Lab4.elf" \

EXECUTABLES_OS_FORMAT += \
TRK-KEA128_Lab4.elf \

SECONDARY_FLASH += \
TRK-KEA128_Lab4.hex \

SECONDARY_FLASH_QUOTED += \
"TRK-KEA128_Lab4.hex" \

SECONDARY_FLASH_OS_FORMAT += \
TRK-KEA128_Lab4.hex \


# All Target
all: warning-messages explicit-dependencies TRK-KEA128_Lab4.elf secondary-outputs

# Tool invocations
TRK-KEA128_Lab4.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/SKEAZ128_flash.ld
	@echo 'Building target: $@'
	@echo 'Executing target #8 $@'
	@echo 'Invoking: ARM Ltd Windows GCC C Linker'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED) @"TRK-KEA128_Lab4.args" -o"TRK-KEA128_Lab4.elf"
	@echo 'Finished building target: $@'
	@echo ' '

TRK-KEA128_Lab4.hex: TRK-KEA128_Lab4.elf
	@echo 'Executing target #9 $@'
	@echo 'Invoking: ARM Ltd Windows GNU Create Flash Image'
	"$(ARMSourceryDirEnv)/arm-none-eabi-objcopy"  -O srec TRK-KEA128_Lab4.elf "TRK-KEA128_Lab4.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) $(SECONDARY_FLASH_QUOTED) "./*/*/*.o" "./*/*.o"  $(S_UPPER_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(SX_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*/*.d" "./*/*.d"  $(ProjDirPath)/FLASH/TRK-KEA128_Lab4.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: ../Project_Settings/Linker_Files/SKEAZ128_flash.ld TRK-KEA128_Lab4.elf 

warning-messages: 

