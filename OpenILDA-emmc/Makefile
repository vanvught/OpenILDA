ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings  -mcpu=arm1176jzf-s -march=armv6 -mfpu=vfp

COPS = -Wall -O3 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mhard-float -I"./" -I"./include"
COPS += -I"./common/include" -I"./common/lib" -I"./j4cDAC/common" -I"./j4cDAC/common/inc" -I"./j4cDAC/firmware/inc" -I"./j4cDAC/firmware/lib/lpc17xx" -I"./j4cDAC/firmware/drivers/include" -I"./j4cDAC/firmware/lwip-1.3.2/src/include" -I"./j4cDAC/firmware/lwip-1.3.2/src/include/ipv4"
#COPS += -DENABLE_FRAMEBUFFER

LIB = -L /opt/gnuarm-hardfp/arm-none-eabi/lib/ -L/opt/gnuarm-hardfp/lib/gcc/arm-none-eabi/4.7.3
LIB += -L"../emmc/Release"
LIB += -L"../fb/Release"

all : main.hex main.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list

vectors.o : ./firmware/vectors.s
	$(ARMGNU)-as $(AOPS) ./firmware/vectors.s -o vectors.o
	
main.o : ./firmware/main.c
	$(ARMGNU)-gcc $(COPS) -c ./firmware/main.c -o main.o

# newlib 

syscalls.o : ./firmware/newlib/syscalls.c
	$(ARMGNU)-gcc $(COPS) -c ./firmware/newlib/syscalls.c -o syscalls.o	
	
# FATfs	
	
# common/lib
	
hardware.o : ./common/lib/hardware.c
	$(ARMGNU)-gcc $(COPS) -c ./common/lib/hardware.c -o hardware.o
	
serial.o : ./common/lib/serial.c
	$(ARMGNU)-gcc $(COPS) -c ./common/lib/serial.c -o serial.o	

# common/lib

ff.o : ./common/lib/ff.c
	$(ARMGNU)-gcc $(COPS) -c ./common/lib/ff.c -o ff.o
	
ccsbcs.o : ./common/lib/option/ccsbcs.c
	$(ARMGNU)-gcc $(COPS) -c ./common/lib/option/ccsbcs.c -o ccsbcs.o
	
# j4cDAC common/lib	

vsnprintf.o : ./j4cDAC/common/lib/vsnprintf.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/common/lib/vsnprintf.c -o vsnprintf.o	

# j4cDAC firmware/file
	
autoplay.o : ./j4cDAC/firmware/file/autoplay.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/file/autoplay.c -o autoplay.o

ild-player.o : ./j4cDAC/firmware/file/ild-player.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/file/ild-player.c -o ild-player.o		

playback.o : ./j4cDAC/firmware/file/playback.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/file/playback.c -o playback.o
	
# j4cDAC firmware/lib	

fatfs.o : ./j4cDAC/firmware/lib/fatfs.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/lib/fatfs.c -o fatfs.o	
	
fixpoint.o : ./j4cDAC/firmware/lib/fixpoint.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/lib/fixpoint.c -o fixpoint.o

lightengine.o : ./j4cDAC/firmware/lib/lightengine.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/lib/lightengine.c -o lightengine.o
	
panic.o : ./j4cDAC/firmware/lib/panic.c
	$(ARMGNU)-gcc $(COPS) -DPC_BUILD -c ./j4cDAC/firmware/lib/panic.c -o panic.o
	
playback_.o : ./j4cDAC/firmware/lib/playback.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/lib/playback.c -o playback_.o
	
transform.o : ./j4cDAC/firmware/lib/transform.c
	$(ARMGNU)-gcc $(COPS) -DPC_BUILD -c ./j4cDAC/firmware/lib/transform.c -o transform.o	
	
# j4cDAC firmware/net
	
osc.o : ./j4cDAC/firmware/net/osc.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/net/osc.c -o osc.o	
	
ilda-osc.o : ./j4cDAC/firmware/net/ilda-osc.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/net/ilda-osc.c -o ilda-osc.o
	
correction-osc.o : ./j4cDAC/firmware/net/correction-osc.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/net/correction-osc.c -o correction-osc.o		
	
# lwip-1.3.2

ip_addr.o : ./j4cDAC/firmware/lwip-1.3.2/src/core/ipv4/ip_addr.c
	$(ARMGNU)-gcc $(COPS) -c ./j4cDAC/firmware/lwip-1.3.2/src/core/ipv4/ip_addr.c -o ip_addr.o					
	
# Firmware library

bcm2835_asm.o : ./firmware/lib/bcm2835_asm.S
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ -c ./firmware/lib/bcm2835_asm.S -o bcm2835_asm.o	

mcp49x2_asm.o : ./firmware/lib/mcp49x2_asm.S
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ -c ./firmware/lib/mcp49x2_asm.S -o mcp49x2_asm.o
	
bcm2835.o : ./firmware/lib/bcm2835.c
	$(ARMGNU)-gcc $(COPS) -c ./firmware/lib/bcm2835.c -o bcm2835.o
	
mcp49x2.o : ./firmware/lib/mcp49x2.c
	$(ARMGNU)-gcc $(COPS) -c ./firmware/lib/mcp49x2.c -o mcp49x2.o	
	
dac_asm.o : ./firmware/lib/dac_asm.S
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ -c ./firmware/lib/dac_asm.S -o dac_asm.o
	
bcm2835_irq.o : ./firmware/lib/bcm2835_irq.c
	$(ARMGNU)-gcc $(COPS) -c ./firmware/lib/bcm2835_irq.c -o bcm2835_irq.o
	
dac.o : ./firmware/lib/dac.c
	$(ARMGNU)-gcc $(COPS) -c ./firmware/lib/dac.c -o dac.o
	
network-stub.o : ./firmware/lib/network-stub.c
	$(ARMGNU)-gcc $(COPS) -c ./firmware/lib/network-stub.c -o network-stub.o
	
pbuf-stub.o : ./firmware/lwp-1.3.2/src/core/pbuf-stub.c
	$(ARMGNU)-gcc $(COPS) -c ./firmware/lwp-1.3.2/src/core/pbuf-stub.c -o pbuf-stub.o	
	
udp-stub.o : ./firmware/lwp-1.3.2/src/core/udp-stub.c
	$(ARMGNU)-gcc $(COPS) -c ./firmware/lwp-1.3.2/src/core/udp-stub.c -o udp-stub.o	
	
fiq_handler.o : ./firmware/lib/fiq_handler.S
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ -c ./firmware/lib/fiq_handler.S -o fiq_handler.o	


main.elf : Makefile memmap vectors.o syscalls.o main.o bcm2835.o bcm2835_asm.o mcp49x2.o mcp49x2_asm.o ff.o ccsbcs.o vsnprintf.o ild-player.o playback.o autoplay.o fatfs.o panic.o playback_.o transform.o dac.o hardware.o serial.o bcm2835_irq.o lightengine.o fixpoint.o osc.o network-stub.o pbuf-stub.o udp-stub.o ilda-osc.o correction-osc.o ip_addr.o fiq_handler.o ../emmc/Release/libemmc.a ../fb/Release/libfb.a
	$(ARMGNU)-ld vectors.o main.o syscalls.o bcm2835.o bcm2835_asm.o mcp49x2.o mcp49x2_asm.o ff.o ccsbcs.o vsnprintf.o ild-player.o playback.o autoplay.o fatfs.o panic.o playback_.o dac.o transform.o hardware.o serial.o bcm2835_irq.o lightengine.o fixpoint.o osc.o network-stub.o pbuf-stub.o udp-stub.o ilda-osc.o correction-osc.o ip_addr.o fiq_handler.o -Map main.map -T memmap -o main.elf  $(LIB) -lemmc -lc -lgcc
	$(ARMGNU)-objdump -D main.elf > main.list

main.bin : main.elf
	$(ARMGNU)-objcopy main.elf -O binary main.bin

main.hex : main.elf
	$(ARMGNU)-objcopy main.elf -O ihex main.hex
