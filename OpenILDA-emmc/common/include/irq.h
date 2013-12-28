#ifndef IRQ_H_
#define IRQ_H_

#define RXBUFMASK 0xFF
#define RXINDEXMASK 0xFF

extern volatile unsigned int rxihead;
extern volatile unsigned int rxitail;
extern volatile unsigned char rxbuffer[RXINDEXMASK][RXBUFMASK];

struct aux_buffer {
	unsigned char rxbuffer[RXBUFMASK];
	unsigned char data_len;
};

extern volatile struct aux_buffer aux_buffers[RXINDEXMASK];

#endif /* IRQ_H_ */
