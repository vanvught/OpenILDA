#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>

void c_irq_handler(void) {}
void c_fiq_handler(void) {}

#ifdef ENABLE_FRAMEBUFFER
extern void bcm2835_console_begin(void);
#endif

extern void cpu_info(void);
extern void mem_info(void);

int notmain ( unsigned int earlypc )
{
	bcm2835_uart_begin();

#ifdef ENABLE_FRAMEBUFFER
	bcm2835_console_begin();
#endif

    printf("Hello World!\n");

    uint64_t ts = bcm2835_st_read();

    mem_info();

    printf("\n%ld usec elapsed\n\n", (long int)(bcm2835_st_read() - ts));

	cpu_info();

    printf("Program end\n");

    return 0;
}
