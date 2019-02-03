################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include user_buttons/subdir.mk
-include ui_template/subdir.mk
-include uart/subdir.mk
-include spi/subdir.mk
-include rgb_led/subdir.mk
-include pwm/subdir.mk
-include interval/subdir.mk
-include i2c/subdir.mk
-include gpio/subdir.mk
-include core/subdir.mk
-include cloud/subdir.mk
-include buzzer/subdir.mk
-include board/subdir.mk
-include block/subdir.mk
-include ble/subdir.mk
-include app_src/subdir.mk
-include adc/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: Atmosphere_Project.axf

# Tool invocations
Atmosphere_Project.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -L"lib" -L"RapidIot_Base/Debug" -Xlinker -Map="Atmosphere_Project.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T MK64FN1M0xxx12_connectivity.ld -L ../ -o "Atmosphere_Project.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Atmosphere_Project.axf
	-@echo ' '

pre-build:
	@echo 'Pre-Build'
	-arm-none-eabi-gcc -E -x c -P -Iinclude "MK64FN1M0xxx12_connectivity.lld" -o "MK64FN1M0xxx12_connectivity.ld" -DgUseBootloaderLink_d=1 -DgUseInternalStorageLink_d=0 -DgUseNVMLink_d=0 -D__ram_vector_table__=1 -DgNVMSectorCountLink_d=32 -DgEraseNVMLink_d=0
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Atmosphere_Project.axf" ; arm-none-eabi-objcopy -v -O binary "Atmosphere_Project.axf" "Atmosphere_Project.bin"
	-arm-none-eabi-gcc --version
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
