#include <bcm2835.h>
#include <spi_dac.h>
#include <dac.h>
#include <transform.h>

/* ip : scratch register, synonymous with r12 */

.macro SELECT_MCP4922 reg_gpio_base, reg_scratch
mov \reg_scratch, #(1 << HC139_A_GPIO_PIN)
str \reg_scratch, [\reg_gpio_base, #BCM2835_GPCLR0]
mov \reg_scratch, #(1 << HC139_B_GPIO_PIN)
str \reg_scratch, [\reg_gpio_base, #BCM2835_GPSET0]
.endm

.macro SELECT_MCP4902_1 reg_gpio_base, reg_scratch
mov \reg_scratch, #(1 << HC139_A_GPIO_PIN)
str \reg_scratch, [\reg_gpio_base, #BCM2835_GPSET0]
mov \reg_scratch, #(1 << HC139_B_GPIO_PIN)
str \reg_scratch, [\reg_gpio_base, #BCM2835_GPCLR0]
.endm

.macro SELECT_MCP4902_2 reg_gpio_base, reg_scratch
mov \reg_scratch, #(1 << HC139_A_GPIO_PIN)
str \reg_scratch, [\reg_gpio_base, #BCM2835_GPCLR0]
mov \reg_scratch, #(1 << HC139_B_GPIO_PIN)
str \reg_scratch, [\reg_gpio_base, #BCM2835_GPCLR0]
.endm

.macro SPI_WRITE reg_data, reg_spio_base, reg_scratch
ldr	\reg_scratch, [\reg_spio_base]
orr	\reg_scratch, \reg_scratch, #BCM2835_SPI0_CS_CLEAR
str	\reg_scratch, [\reg_spio_base]
ldr	\reg_scratch, [\reg_spio_base]
orr	\reg_scratch, \reg_scratch, #BCM2835_SPI0_CS_TA
str	\reg_scratch, [\reg_spio_base]
1:
    ldr	\reg_scratch, [\reg_spio_base]
    tst	\reg_scratch, #BCM2835_SPI0_CS_TXD
    beq	1b
lsr	\reg_scratch, \reg_data, #8
uxtb \reg_data, \reg_data
str	\reg_scratch, [\reg_spio_base, #BCM2835_SPI0_FIFO]
str	\reg_data, [\reg_spio_base, #BCM2835_SPI0_FIFO]
2:
 	ldr	\reg_scratch, [\reg_spio_base]
 	tst	\reg_scratch, #BCM2835_SPI0_CS_DONE
 	beq	2b
ldr	\reg_scratch, [\reg_spio_base]
bic	\reg_scratch, \reg_scratch, #BCM2835_SPI0_CS_TA
str	\reg_scratch, [\reg_spio_base]
.endm

.section .text
.align	2
.global asm_fiq_handler
asm_fiq_handler:
@ Enter
sub	lr, lr, #4
push	{r0, r1, r2, r3, r4, r5, r6, r7, lr}
								@ r0		r1		r2		r3		r4		r5		r6		r7		r9		r10		r11		ip/r12
#if DAC_INSTRUMENT_TIME
@ Get SystemTimer CLO
ldr r2,=BCM2835_ST_BASE			@ 					&ST_BASE
ldr r11, [r2, #BCM2835_ST_CLO]	@																					time0
#endif
@ BCM2835_ST->CS = 2
@ldr r2, =BCM2835_ST_BASE
mov r1, #2						@			2		&ST_BASE														time0
str r1, [r2]					@ Write r1 to ST_CS (= ST_BASE)														time0
@ BCM2835_ST->C1 = BCM2835_ST->CLO + ticks_per_second
ldr r1, =ticks_per_point		@			&ticks_per_point														time0
ldr	r1, [r1]					@			ticks_per_point															time0
ldr	r0, [r2, #BCM2835_ST_CLO]	@	ST_CLO																			time0
add	r1, r0, r1					@			ST_CLO+ticks_per_point													time0
str	r1, [r2, #16]				@ Write r1 to ST_C1 (= ST_BASE + 0x10)												time0

@ Get dac_control
ldr r0, =(dac_control+20)		@ &c																				time0

@ Load irq_do flag
ldrb r2, [r0, #-12]				@ &c				irq_do															time0

@ Check what we're supposed to do
cmp r2, #14						@ IRQ_DO_BUFFER = 14
bne exit

@ Load buf produce/consume pointers
ldr r1, [r0, #-16]				@ &c		p/c		14																time0
uxth ip, r1						@ &c		p/c		14																time0	prod
uxth r1, r1, ror #16			@ &c		cons	14																time0	prod

@ Underflow ?
cmp r1, ip						@ &c		cons	14																time0	prod
beq do_dac_stop_underflow
								@ r0		r1		r2		r3		r4		r5		r6		r7		r9		r10		r11		ip/r12
@ Find the addres of our point
ldr r4, =dac_buffer				@ &c		cons	14				&dac_b											time0	prod
mla r5, r2, r1, r4				@ &c		cons					&dac_b 	&point									time0	prod

@ Increment counter
ldr r2, [r0, #-20]				@ &c		cons	count			&dac_b 	&point									time0	prod
add r2, #1						@ &c		cons	cnt+1			&dac_b 	&point									time0	prod
str r2, [r0, #-20]				@ write back count

@ Increment consume
add r1, #1						@ &c		cons+1	cnt+1			&dac_b 	&point									time0	prod
cmp r1, #DAC_BUFFER_POINTS		@ &c		cons+1	cnt+1			&dac_b 	&point									time0	prod
moveq r1, #0					@ &c		cons	count			&dac_b 	&point									time0	prod
strh r1, [r0, #-14]				@ writeback consume

@ Time to handle blue+flags
ldrh r2, [r5, #12]				@ &c		cons	bf				&dac_b 	&point									time0	prod

@ ----TODO
@tst r2, #(1<<15)				@ &c		cons	bf				&dac_b 	&point									time0	prod
@bne dac_pop_rate_change

@ Load/increment produce and consume pointers
ldr r7, [r0, #32]				@ &c		cons	bf				&dac_b 	&point			colorc					time0	prod
@ we don't have Thumb-2, so create the magic constants
mov  r8, #0x01
orr  r8, r8, r8, lsl #8
orr  r8, r8, r8, lsl #16
add r7, r8						@ r8 = 0x01010101
mov  r8, #0x0f
orr  r8, r8, r8, lsl #8
orr  r8, r8, r8, lsl #16
and r7, r8						@ r8 = #0x0f0f0f0f
str r7, [r0, #32]
								@ r0		r1		r2		r3		r4		r5		r6		r7		r9		r10		r11		ip/r12
@ We do nothing with U1 and U2

ldr r10, =BCM2835_GPIO_BASE		@ &c										&point							&GPIO	time0	prod
@ .macro SELECT_MCP4902_1 reg_gpio_base, reg_scratch
SELECT_MCP4902_1 r10, r4		@ &c								scrtch	&point							&GPIO	time0	prod

@ Output intensity
ldrh r2, [r5, #6]				@ &c						i				&point							&GPIO	time0	prod
lsr	r3, r2, #8
lsl	r3, r3, #4
orr	r3, r3, #(0x3000 | 0<<15)	@ &c						i | 0x3000 | 0<<15								&GPIO	time0	prod
uxth r3, r3						@ &c						i (16-bits)		&point							&GPIO	time0	prod

ldr r9, =BCM2835_SPI0_BASE		@ &c						i (16-bits)		&point					&SPIO	&GPIO	time0	prod
@ .macro SPI_WRITE reg_data, reg_spio_base, reg_scratch
SPI_WRITE r3, r9, r4			@ &c						i		scrtch	&point					&SPIO	&GPIO	time0	prod

@ Output red
ldrb    r3, [r5, #6]			@ &c						red				&point					&SPIO	&GPIO	time0	prod
lsl	r3, r3, #4
orr	r3, r3, #(0x3000 | 1<<15)
@ .macro SPI_WRITE reg_data, reg_spio_base, reg_scratch
SPI_WRITE r3, r9, r4			@ &c						red		scrtch	&point					&SPIO	&GPIO	time0	prod

@ .macro SELECT_MCP4902_1 reg_gpio_base, reg_scratch
SELECT_MCP4902_2 r10, r4		@ &c								scrtch	&point					&SPIO	&GPIO	time0	prod

@ Output green
ldrh	r3, [r5, #4]			@ &c						green			&point					&SPIO	&GPIO	time0	prod
and	r3, r3, #0xff0
orr	r3, r3, #(0x3000 | 0<<15)
@ .macro SPI_WRITE reg_data, reg_spio_base, reg_scratch
SPI_WRITE r3, r9, r4			@ &c						green	scrtch	&point					&SPIO	&GPIO	time0	prod

@ Output blue
ldrh	r3, [r5, #12]			@ &c						blue			&point					&SPIO	&GPIO	time0	prod
and	r3, r3, #0xff0
orr	r3, r3, #(0x3000 | 1<<15)
@ .macro SPI_WRITE reg_data, reg_spio_base, reg_scratch
SPI_WRITE r3, r9, r4			@ &c						blue	scrtch	&point					&SPIO	&GPIO	time0	prod
								@ r0		r1		r2		r3		r4		r5		r6		r7		r9		r10		r11		ip/r12
@ Get ready to load the transform
ldr ip, =transform_matrix		@ &c				irg						&point					&SPIO	&GPIO	time0	&transform_matrix

@ Separate X and Y
ldrsh r6, [r5, #2]				@ &c				irg						&point	y				&SPIO	&GPIO	time0	&transform_matrix
ldrsh r5, [r5]					@ &c				irg						x						&SPIO	&GPIO	time0	&transform_matrix


/* Do the transform */
mul r7, r5, r6					@ &c				irg						x		y		x*y		&SPIO	&GPIO	time0	&transform_matrix
asrs r7, r7, #COORD_MAX_EXP		@ &c				irg						x		y		x*ys	&SPIO	&GPIO	time0	&transform_matrix

									@ r5 = x, r6 = y
									@ r7 = x * y >> 15
ldmia ip!, { r0, r1, r2, r3}		@ r0 = c[0], r1 = c[1], r2 = c[2], r3 = c[3]
mul r0, r0, r5						@ r0 = c[0] * x
mla r0, r1, r6, r0					@ r0 = c[0] * x + c[1] * y
mla r0, r2, r7, r0					@ r0 = c[0] * x + c[1] * y + c[2] * (x * y >> 15)
add r0, r3, r0, asr #COORD_MAX_EXP 	@ r0 = c[3] + (c[0] * x + c[1] * y + c[2] * x * y >> 15)) >> 15
ldmia ip, { r1, r2, r3, r4}			@ r1 = c[4], r2 = c[4+1], r3 = c[4+2], r4 = c[4+3]
mul r1, r1, r5						@ r1 = c[4] * x
mla r1, r2, r6, r1					@ r1 = c[4] * x + c[4+1] * y
mla r1, r3, r7, r1					@ r1 = c[4] * x + c[4+1] * y + c[4+2] * (x * y >> 15)
add r1, r4, r1, asr #COORD_MAX_EXP	@ r1 = c[4+3] + (c[4] * x + c[4+1] * y + c[4+2] * (x * y >> 15)) >> 15

								@ r0		r1		r2		r3		r4		r5		r6		r7		r9		r10		r11		ip/r12
@ .macro SELECT_MCP4922 reg_gpio_base, reg_scratch
SELECT_MCP4922 r10, r4			@ x			y						scrtch							&SPIO	&GPIO	time0

asr	r0, r0, #4
add	r0, r0, #0x800
orr	r0, #(0x3000 | 0<<15)
@ .macro SPI_WRITE reg_data, reg_spio_base, reg_scratch
ldr r9, =BCM2835_SPI0_BASE
SPI_WRITE r0, r9, r4			@ x(dac)	y						scrtch							&SPIO	&GPIO	time0

asr	r0, r1, #4
add	r0, r0, #0x800
orr	r0, #(0x3000 | 1<<15)
@ .macro SPI_WRITE reg_data, reg_spio_base, reg_scratch
SPI_WRITE r0, r9, r4			@ y(dac)							scrtch							&SPIO	&GPIO	time0

exit:
#if DAC_INSTRUMENT_TIME
ldr r0,=BCM2835_ST_BASE			@ &ST_BASE 																			time0
ldr r0, [r0, #BCM2835_ST_CLO]	@ time																				time0
ldr r1,=dac_cycle_count			@			&dac_cycle_count														time0
sub r0, r11						@ time-time0
str r0, [r1]					@ Write r0 to dac_cycle_count
#endif
								@ r0		r1		r2		r3		r4		r5		r6		r7		r9		r10		r11		ip/r12
@ Exit
ldm	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, pc}^

do_dac_stop_underflow:
beq dac_stop_underflow
b exit
