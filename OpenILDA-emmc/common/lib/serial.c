#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <bcm2835.h>

void serial_init() {
	bcm2835_uart_begin();
}

void inline serial_send(const char *buf, int len) {
	while (len--) {
		bcm2835_uart_send((uint8_t)*buf);
		buf++;
	}
}

#ifdef ENABLE_FRAMEBUFFER
extern int console_putc(int c);
void inline console_send(const char *buf, int len) {
	while (len--) {
		console_putc((int)*buf);
		buf++;
	}
}
#endif

void outputf(const char *fmt, ...) {
	va_list va;
	char buffer[80];
	int n;

	va_start(va, fmt);
	n = vsnprintf(buffer, sizeof(buffer) - 2, fmt, va);

	if (n > (sizeof(buffer) - 2))
		n = sizeof(buffer) - 2;

	if (n > 1 && !buffer[n-1] && buffer[n-2] == '\n')
		n -= 2;

#ifdef ENABLE_FRAMEBUFFER
	buffer[n] = '\n';
	console_send(buffer, n + 1);
#else
	buffer[n] = '\r';
	buffer[n + 1] = '\n';

	serial_send(buffer, n + 2);
#endif
}

void debugf(const char *fmt, ...) {
	va_list va;
	char buffer[80];
	int n;

	va_start(va, fmt);
	n = vsnprintf(buffer, sizeof(buffer), fmt, va);

	if (n > (sizeof(buffer)))
		n = sizeof(buffer);

	serial_send(buffer, n);
}

static const char hexarr[] = "0123456789ABCDEF";

void hexdump(const char *data, int len) {
	int i;
	char c, buf[2];

	for (i = 0; i < len; i++) {
		c = data[i];
		buf[0] = hexarr[c >> 4];
		buf[1] = hexarr[c & 0xF];
		serial_send(buf, 2);
	}
}
