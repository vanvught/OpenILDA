#include <stdlib.h>
#include "lwip/opt.h"
#include "lwip/udp.h"
#include "lwip/def.h"

struct pbuf *pbuf_alloc(pbuf_layer layer, u16_t length, pbuf_type type) {
	struct pbuf *p = NULL;

	p = (struct pbuf*) malloc(sizeof(struct pbuf));
	if (p == NULL ) {
		return NULL ;
	}

	p->payload = (u8_t *) malloc(length * sizeof(u8_t));
	p->len = p->tot_len = length;
	p->next = NULL;
	p->type = type;

	return p;
}

u8_t inline pbuf_free(struct pbuf *p) {
	free(p->payload);
	free(p);
	return 0;
}
