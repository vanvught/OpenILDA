#include <bcm2835.h>
#include <tables.h>
#include <irq.h>

volatile unsigned int rxhead;
volatile unsigned int rxtail;
volatile unsigned int rxihead;
volatile unsigned int rxitail;
volatile struct aux_buffer aux_buffers[RXINDEXMASK];

static void irq_init(void) {
	BCM2835_UART1->IER = 0x05;
	BCM2835_IRQ->IRQ_ENABLE1 = 1 << 29;
}

void __attribute__((interrupt("IRQ"))) c_irq_handler(void) {
	unsigned int rb;
	unsigned char c;

	while (1)
	{
		rb = BCM2835_UART1->IIR;
		if ((rb & 1) == 1)
			break; //no more interrupts
		if ((rb & 6) == 4) {
			c = BCM2835_UART1->IO  & 0xFF;
			if (c == '[') {
				rxhead = 0;
			} else if (c == ']') {
				aux_buffers[rxihead].data_len = rxhead;
				rxihead = (rxihead + 1) & RXINDEXMASK;
			} else {
				aux_buffers[rxihead].rxbuffer[rxhead] = c;
				rxhead = (rxhead + 1) & RXBUFMASK;
			}
		}
	}
}

INITIALIZER(hardware, irq_init);
