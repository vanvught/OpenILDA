################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../block.c \
../block_cache.c \
../emmc.c \
../mbox.c 

OBJS += \
./block.o \
./block_cache.o \
./emmc.o \
./mbox.o 

C_DEPS += \
./block.d \
./block_cache.d \
./emmc.d \
./mbox.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DBUILDING_RPIBOOT -DENABLE_DEFAULT_FONT -DENABLE_FRAMEBUFFER -DENABLE_SD -DENABLE_BLOCK_CACHE -I"/opt/data/development/workspace/ff9b/src" -I"/opt/data/development/workspace/emmc" -I"/opt/data/development/workspace/emmc/include" -O3 -Wall -c -fmessage-length=0 -std=c99 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mhard-float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


