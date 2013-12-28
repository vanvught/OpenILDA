#include <bcm2835.h>
#include <spi_dac.h>

/* hw_dac_init()
 *
 * Initialize the DAC hardware and sets it to output 0.
 */
void inline hw_dac_init(void) {
	// SPI
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(12);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	// LDAC
	bcm2835_gpio_fsel(LDAC_GPIO_PIN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_clr(LDAC_GPIO_PIN);
	// HC139
	bcm2835_gpio_fsel(HC139_A_GPIO_PIN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_clr(HC139_A_GPIO_PIN);
	bcm2835_gpio_fsel(HC139_B_GPIO_PIN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_clr(HC139_B_GPIO_PIN);
}

void inline hw_dac_zero_all_channels(void) {
	dac_write_all(0x07FF, 0x07FF, 0x00, 0x00, 0x00, 0x00);
}

/* led_set_frontled()
 *
 * Set the state of the front (top, on proto) LED.
 */
void led_set_frontled(int state) {
	bcm2835_gpio_write(16, state);
}

/* led_set_backled()
 *
 * Set the state of the back (bottom, on proto) LED.
 */
void led_set_backled(int state) {

}

/* led_init()
 *
 * Initialize the LEDs.
 */
void led_init(void) {
	bcm2835_gpio_fsel(16, BCM2835_GPIO_FSEL_OUTP);
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// this is derived from:

/*
 *  Broadcom BCM2708 watchdog driver.
 *
 *  (c) Copyright 2010 Broadcom Europe Ltd
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 *      BCM2708 watchdog driver. Loosely based on wdt driver.
 */

//------------------------------------------------------------------------

#define BCM2708_PERI_BASE           0x20000000
#define PM_BASE                     (BCM2708_PERI_BASE + 0x100000) /* Power Management, Reset controller and Watchdog registers */
#define PM_RSTC                     (PM_BASE+0x1c)
#define PM_WDOG                     (PM_BASE+0x24)
#define PM_WDOG_RESET               0000000000
#define PM_PASSWORD                 0x5a000000
#define PM_WDOG_TIME_SET            0x000fffff
#define PM_RSTC_WRCFG_CLR           0xffffffcf
#define PM_RSTC_WRCFG_SET           0x00000030
#define PM_RSTC_WRCFG_FULL_RESET    0x00000020
#define PM_RSTC_RESET               0x00000102

//------------------------------------------------------------------------
static void inline wdog_start ( unsigned int timeout )
{
    unsigned int pm_rstc,pm_wdog;
    /* Setup watchdog for reset */
    pm_rstc = GET32(PM_RSTC);
    pm_wdog = PM_PASSWORD | (timeout & PM_WDOG_TIME_SET); // watchdog timer = timer clock / 16; need password (31:16) + value (11:0)
    pm_rstc = PM_PASSWORD | (pm_rstc & PM_RSTC_WRCFG_CLR) | PM_RSTC_WRCFG_FULL_RESET;
    PUT32(PM_WDOG,pm_wdog);
    PUT32(PM_RSTC,pm_rstc);
}
//------------------------------------------------------------------------
void wdog_stop ( void )
{
    PUT32(PM_RSTC,PM_PASSWORD | PM_RSTC_RESET);
}

#define WDOG_TIMEOUT 0x0FFFF

void watchdog_init(void) {
	wdog_start(WDOG_TIMEOUT);
}

void watchdog_feed(void) {
	wdog_start(WDOG_TIMEOUT);
}
