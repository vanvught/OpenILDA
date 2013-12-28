#ifndef BCM2835_SPI_H_
#define BCM2835_SPI_H_

extern int bcm2835_spi_dac_begin(void);
extern void bcm2835_spi_dac_end(void);
extern void bcm2835_spi_dac_transfer(char *data, char *rgbi);

extern void inline bcm2835_spi_mcp4922_a(uint16_t data);
extern void inline bcm2835_spi_mcp4922_b(uint16_t data);

extern void inline bcm2835_spi_mcp4902_1a(uint16_t data);
extern void inline bcm2835_spi_mcp4902_1b(uint16_t data);

extern void inline bcm2835_spi_mcp4902_2a(uint16_t data);
extern void inline bcm2835_spi_mcp4902_2b(uint16_t data);

#endif
