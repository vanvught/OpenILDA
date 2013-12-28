#include "lwip/opt.h"
#include "lwip/udp.h"
#include "lwip/def.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/icmp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "arch/perf.h"
#include "lwip/dhcp.h"

#include <bcm2835.h>

void udp_new(struct udp_pcb *pcb) {
	memset(pcb, 0, sizeof(struct udp_pcb));
}

struct udp_pcb *ipcb;
struct ip_addr *ipaddr;
u16_t iport;

err_t udp_bind(struct udp_pcb *pcb, struct ip_addr *paddr, u16_t port) {
	ipcb = pcb;
	ipaddr = paddr;
	iport = port;
	return 0;
}

void udp_recv(struct udp_pcb *pcb,
		void (*recv)(struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr,
				u16_t port), void *recv_arg) {
	pcb->recv = recv;
	pcb->recv_arg = recv_arg;
}

err_t udp_sendto(struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *dst_ip,
		u16_t dst_port) {
	int len = p->len;
	char *buf = p->payload;

	while (len--) {
		int c = *buf;
		bcm2835_uart_send(c);
		buf++;
	}
	bcm2835_uart_send('\r');
	bcm2835_uart_send('\n');
	//serial_send_str("\r\n");
	return 0;
}
