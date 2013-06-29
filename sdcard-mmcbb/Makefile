ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings  -mcpu=arm1176jzf-s -march=armv6 -mfpu=vfp

COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mhard-float -I"./include" -I"./firmware/include" -DBARE_METAL -DBW_I2C_LCD

LIB = -L /opt/gnuarm-hardfp/arm-none-eabi/lib/ -L/opt/gnuarm-hardfp/lib/gcc/arm-none-eabi/4.7.3

gcc : main.hex sdcard.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o
	
bcm2835_asm.o : bcm2835_asm.s
	$(ARMGNU)-as $(AOPS) bcm2835_asm.s -o bcm2835_asm.o		
	
main.o : main.c
	$(ARMGNU)-gcc $(COPS) -c main.c -o main.o

syscalls.o : syscalls.c
	$(ARMGNU)-gcc $(COPS) -c $(COPS) syscalls.c -o syscalls.o	
	
bcm2835.o : bcm2835.c
	$(ARMGNU)-gcc $(COPS) -c bcm2835.c -o bcm2835.o
	
ff.o : ff.c
	$(ARMGNU)-gcc $(COPS) -c ff.c -o ff.o
	
ccsbcs.o : ./option/ccsbcs.c
	$(ARMGNU)-gcc $(COPS) -c ./option/ccsbcs.c -o ccsbcs.o		

mmcbb.o : mmcbb.c
	$(ARMGNU)-gcc $(COPS) -c mmcbb.c -o mmcbb.o	
	
main.elf : memmap vectors.o main.o syscalls.o bcm2835.o bcm2835_asm.o ff.o ccsbcs.o mmcbb.o
	$(ARMGNU)-ld vectors.o main.o syscalls.o bcm2835.o bcm2835_asm.o ff.o ccsbcs.o mmcbb.o -T memmap -o main.elf  $(LIB) -lc -lgcc
	$(ARMGNU)-objdump -D main.elf > main.list

sdcard.bin : main.elf
	$(ARMGNU)-objcopy main.elf -O binary sdcard.bin

main.hex : main.elf
	$(ARMGNU)-objcopy main.elf -O ihex main.hex


