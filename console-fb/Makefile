ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings  -mcpu=arm1176jzf-s -march=armv6 -mfpu=vfp

COPS = -Wall -O3 -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mhard-float -I"./include"
COPS += -DENABLE_FRAMEBUFFER

LIB = -L /opt/gnuarm-hardfp/arm-none-eabi/lib/ -L/opt/gnuarm-hardfp/lib/gcc/arm-none-eabi/4.7.3
#LIB += -L"../emmc/Release" 
LIB += -L"../fb/Release"

BUILD = build/
SOURCE = ./
ASM_OBJECTS := $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s)) $(patsubst $(SOURCE)%.S,$(BUILD)%.o,$(wildcard $(SOURCE)*.S))
C_OBJECTS := $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c)) 
OBJECTS := $(ASM_OBJECTS) $(C_OBJECTS)

TARGET = kernel.img
LINKER = memmap
MAP = kernel.map
LIST = kernel.list

all : $(TARGET)

clean :
	rm -f $(BUILD)*.o
	rm -f $(TARGET)
	rm -f $(BUILD)*.elf
	rm -f $(MAP)
	rm -f $(LIST)

$(BUILD)vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o $(BUILD)vectors.o
	
$(BUILD)%.o: $(SOURCE)%.S
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ $< -c -o $@		

$(BUILD)%.o: $(SOURCE)%.c
	$(ARMGNU)-gcc $(COPS) $< -c -o $@	

$(BUILD)main.elf : Makefile $(LINKER) $(OBJECTS) ../fb/Release/libfb.a
	$(ARMGNU)-ld $(OBJECTS) -Map $(MAP) -T $(LINKER) -o $(BUILD)main.elf $(LIB) -lfb -lc -lgcc
	$(ARMGNU)-objdump -D $(BUILD)main.elf > $(LIST)

$(TARGET) : $(BUILD)main.elf
	$(ARMGNU)-objcopy $(BUILD)main.elf -O binary $(TARGET)