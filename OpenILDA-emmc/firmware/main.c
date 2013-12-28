#include <LPC17xx.h>
#include <serial.h>
#include "ether.h"
#include <lwip/init.h>
#include <lwip/tcp.h>
#include <lwip/dhcp.h>
#include <ipv4/lwip/autoip.h>
#include <string.h>
#include <broadcast.h>
#include <assert.h>
#include <attrib.h>
#include <lightengine.h>
#include <skub.h>
#include <tables.h>
#include <playback.h>
#include <dac.h>
#include <hardware.h>
#include <file_player.h>
#include <dac_settings.h>

#include <bcm2835.h>

struct {
	volatile uint32_t time;
	volatile uint8_t mtime;
} clock;
//extern int f0ad_flag;
//extern const char build[];

dac_settings_t settings;
#if 0
void SysTick_Handler(void) {
	uint8_t mtime_buf = clock.mtime + 1;

	if (hw_board_rev == HW_REV_MP2 && le_state != LIGHTENGINE_ESTOP) {
		if (mtime_buf % 2)
			LPC_GPIO2->FIOSET = (1 << 8);
		else
			LPC_GPIO2->FIOCLR = (1 << 8);
	}

	if (mtime_buf < 10) {
		clock.mtime = mtime_buf;
		return;
	}

	clock.mtime = 0;
	uint32_t time = clock.time;
	clock.time = time + 1;

	/* If we're in an estop condition, slow-blink the LED */
	if (time % 512 == 0) {
		led_set_frontled(1);
	} else if (time % 512 == 450 && le_state == LIGHTENGINE_ESTOP) {
		led_set_frontled(0);
	}
}
#endif
extern void print_dac_cycle_count(void);

//extern void dmx_in_poll(void);

struct periodic_event {
	void (*f)(void);
	int period;
	const char *msg;
	int start;
} const events[] = {
//	{ tcp_tmr, 250, "tcp", 100 },
//	{ ip_reass_tmr, 1000, "ip_reass", 17 },
//	{ etharp_tmr, 5000, "ip_reass", 177 },
//	{ dhcp_coarse_tmr, 60000, "dhcp c", 35 },
//	{ dmx_in_poll, 25, "dmx", 4 },
//	{ dhcp_fine_tmr, 500, "dhcp f", 25 },
//	{ autoip_tmr, AUTOIP_TMR_INTERVAL, "autoip", 10 },
//	{ broadcast_send, 1000, "broadcast", 10 },
#if DAC_INSTRUMENT_TIME
	{ print_dac_cycle_count, 1000000, "dac_cycle_count", 4 },
#endif
//	{ eth_check_link, 100, "eth_check_link", 2 },
};

int events_last[sizeof(events) / sizeof(events[0])];

TABLE(initializer_t, protocol);
TABLE(initializer_t, hardware);
TABLE(initializer_t, poll);

static void COLD NOINLINE FPA_init() {
	int i;

	debugf("### Initializing Hardware ###\r\n");

	for (i = 0; i < TABLE_LENGTH(hardware); i++) {
		outputf("- %s()", hardware_table[i].name);
		hardware_table[i].f();
	}

	debugf("### Initializing Protocols ###\r\n");

	for (i = 0; i < TABLE_LENGTH(protocol); i++) {
		outputf("- %s()", protocol_table[i].name);
		protocol_table[i].f();
	}
}

void check_periodic_timers() {
	int i;
	clock.time = bcm2835_st_read();
	for (i = 0; i < (sizeof(events) / sizeof(events[0])); i++) {
		if (clock.time > events_last[i] + events[i].period) {
			events[i].f();
			events_last[i] += events[i].period;
			watchdog_feed();
		}
	}
}

#ifdef ENABLE_FRAMEBUFFER
extern void bcm2835_console_begin(void);
#endif

int notmain (void)
{
	__disable_irq();
	__disable_fiq();

	bcm2835_uart_begin();

#ifdef ENABLE_FRAMEBUFFER
	bcm2835_console_begin();
#endif

	debugf("Starting up: led\r\n");
	led_init();
	led_set_frontled(1);

	/* Initialize hardware */
	FPA_init();

	outputf("Entering main loop...");
	watchdog_init();

	/* Startup might have taken some time, so reset the periodic event
	 * timers. */
	int i;
	clock.time = bcm2835_st_read();
	for (i = 0; i < (sizeof(events) / sizeof(events[0])); i++) {
		events_last[i] = events[i].start + clock.time;
	}

	__enable_irq();
	//__enable_fiq();

	playback_set_src(SRC_ILDAPLAYER);

	/* Default values */
	dac_set_rate(1000);
	ilda_set_fps_limit(5);

	while(1) {
		watchdog_feed();

		/* Check the stuff we check on each loop iteration. */
		for (i = 0; i < TABLE_LENGTH(poll); i++) {
			poll_table[i].f();
		}

		/* Check for periodic events */
		check_periodic_timers();
	}

	return 0;
}
