################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../firmware/diskio.c 

S_UPPER_SRCS += \
../firmware/qmemcpy.S 

OBJS += \
./firmware/diskio.o \
./firmware/qmemcpy.o 

C_DEPS += \
./firmware/diskio.d 


# Each subdirectory must supply rules for building sources it contributes
firmware/%.o: ../firmware/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DBUILDING_RPIBOOT -DENABLE_DEFAULT_FONT -DENABLE_FRAMEBUFFER -DENABLE_SD -DENABLE_BLOCK_CACHE -I"/opt/data/development/workspace/ff9b/src" -I"/opt/data/development/workspace/emmc" -I"/opt/data/development/workspace/emmc/include" -O3 -Wall -c -fmessage-length=0 -std=c99 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/%.o: ../firmware/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


