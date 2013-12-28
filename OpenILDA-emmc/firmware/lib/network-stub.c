#include "lwip/udp.h"
#include "lwip/ip_addr.h"

#include <stdlib.h>
#include <tables.h>
#include <irq.h>

extern struct udp_pcb *ipcb;
extern struct ip_addr *ipaddr;
extern u16_t iport;

static void aux_poll(void) {
	if (rxihead != rxitail) {
		struct pbuf *p = pbuf_alloc(0, aux_buffers[rxitail].data_len, PBUF_RAM);
		char *buf = p->payload;
		unsigned int i;
		for (i = 0; i < aux_buffers[rxitail].data_len; i++) {
			buf[i] = aux_buffers[rxitail].rxbuffer[i];
		}
		// void (* recv)(struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port),
		ipcb->recv(NULL, p, ipaddr, iport);
		rxitail = (rxitail + 1) & RXINDEXMASK;
	}
}

INITIALIZER(poll, aux_poll)
