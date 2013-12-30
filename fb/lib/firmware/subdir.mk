################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../firmware/console_init.c 

S_UPPER_SRCS += \
../firmware/qmemcpy.S 

OBJS += \
./firmware/console_init.o \
./firmware/qmemcpy.o 

C_DEPS += \
./firmware/console_init.d 


# Each subdirectory must supply rules for building sources it contributes
firmware/%.o: ../firmware/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DBUILDING_RPIBOOT -DENABLE_DEFAULT_FONT -DENABLE_FRAMEBUFFER -I"/opt/data/development/workspace/fb" -I"/opt/data/development/workspace/fb/include" -O3 -Wall -c -fmessage-length=0 -std=c99 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/%.o: ../firmware/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


