#include <bcm2835.h>
#include <spi_dac.h>

.macro FUNC name
.text
.code 32
.global \name
\name:
.endm

.cpu arm1176jzf-s
.fpu vfp

FUNC spi_select_mcp4922
@ void spi_select_mcp4922(void)
ldr r0, =BCM2835_GPIO_BASE
mov r1, #(1 << HC139_A_GPIO_PIN)
str r1, [r0, #BCM2835_GPCLR0]
mov r1, #(1 << HC139_B_GPIO_PIN)
str r1, [r0, #BCM2835_GPSET0]
bx lr

FUNC spi_select_mcp4902_1
@ void spi_select_mcp4902_1(void)
ldr r0, =BCM2835_GPIO_BASE
mov r1, #(1 << HC139_A_GPIO_PIN)
str r1, [r0, #BCM2835_GPSET0]
mov r1, #(1 << HC139_B_GPIO_PIN)
str r1, [r0, #BCM2835_GPCLR0]
bx lr

FUNC spi_select_mcp4902_2
@ void spi_select_mcp4902_2(void)
ldr r0, =BCM2835_GPIO_BASE
mov r1, #(1 << HC139_A_GPIO_PIN)
str r1, [r0, #BCM2835_GPCLR0]
mov r1, #(1 << HC139_B_GPIO_PIN)
str r1, [r0, #BCM2835_GPCLR0]
bx lr
