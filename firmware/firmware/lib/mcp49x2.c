#include <bcm2835.h>
#include <spi_dac.h>

extern void spi_select_mcp4922(void);

void inline bcm2835_spi_mcp4922_a(uint16_t data) {
	spi_select_mcp4922();
	bcm2835_spi_write(data  | 0x3000 | (0<<15));
}

void inline bcm2835_spi_mcp4922_b(uint16_t data) {
	spi_select_mcp4922();
	bcm2835_spi_write(data | 0x3000 | (1<<15));
}

void inline bcm2835_spi_mcp4902_1a(uint16_t data) {
	spi_select_mcp4902_1();
	bcm2835_spi_write((data << 4) | 0x3000 | (0<<15));
}

void inline bcm2835_spi_mcp4902_1b(uint16_t data) {
	spi_select_mcp4902_1();
	bcm2835_spi_write((data << 4) | 0x3000 | (1<<15));
}

void inline bcm2835_spi_mcp4902_2a(uint16_t data) {
	spi_select_mcp4902_2();
	bcm2835_spi_write((data << 4) | 0x3000 | (0<<15));
}

void inline bcm2835_spi_mcp4902_2b(uint16_t data) {
	spi_select_mcp4902_2();
	bcm2835_spi_write((data << 4) | 0x3000 | (1<<15));
}

void dac_write_all(uint16_t x, uint16_t y, uint8_t intensity, uint8_t red, uint8_t green, uint8_t blue) {
	spi_select_mcp4922();
	bcm2835_spi_write(x  | 0x3000 | (0<<15));
	bcm2835_spi_write(y  | 0x3000 | (1<<15));
	bcm2835_spi_mcp4902_1a(intensity);
	bcm2835_spi_mcp4902_1b(red);
	bcm2835_spi_mcp4902_2a(green);
	bcm2835_spi_mcp4902_2b(blue);
}

