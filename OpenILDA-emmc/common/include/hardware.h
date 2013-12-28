#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>
#include <panic.h>

void hw_get_board_rev();
void hw_dac_init(void);
void hw_dac_zero_all_channels(void);

int clock_init(void);

void led_set_frontled(int state);
void led_set_backled(int state);
void led_init(void);

void watchdog_init(void);
void watchdog_feed(void);

inline void hw_dac_write32(uint32_t word);

void __disable_fiq(void);
void __enable_fiq(void);

#endif /* HARDWARE_H_ */
