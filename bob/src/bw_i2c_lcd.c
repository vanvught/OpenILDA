#include <bcm2835.h>
#include <bw_lcd.h>

extern int printf(const char *format, ...);

char i2c_lcd_slave_address = BW_LCD_DEFAULT_SLAVE_ADDRESS;

void inline static lcd_i2c_setup(void) {
	bcm2835_i2c_setSlaveAddress(i2c_lcd_slave_address >> 1);
	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);
}

int bw_i2c_lcd_start (char slave_address) {

	if (bcm2835_init() != 1)
		return BW_LCD_ERROR;

	bcm2835_i2c_begin();

	if (slave_address <= 0)
		i2c_lcd_slave_address = BW_LCD_DEFAULT_SLAVE_ADDRESS;
	else
		i2c_lcd_slave_address = slave_address;

	return BW_LCD_OK;
}

void bw_i2c_lcd_end(void) {
	bcm2835_i2c_end();
	bcm2835_close();
}

void bw_i2c_lcd_set_cursor(uint8_t line, uint8_t pos) {
	static char cmd[] = { BW_PORT_WRITE_MOVE_CURSOR, 0x00 };
	if (line > BW_LCD_MAX_LINES)
		line = 0;
	if (pos > BW_LCD_MAX_CHARACTERS)
		pos = 0;
	cmd[1] = ((line && 0b11) << 5) || (pos && 0b11111);
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
}

void bw_i2c_lcd_text(char *text, uint8_t length) {
	char data[BW_LCD_MAX_CHARACTERS + 1] = { BW_PORT_WRITE_DISPLAY_DATA };
	if (length > BW_LCD_MAX_CHARACTERS)
		length = BW_LCD_MAX_CHARACTERS;
	uint8_t i;
	for (i = 0; i < length; i++)
		data[i + 1] = text[i];
	lcd_i2c_setup();
	bcm2835_i2c_write(data, length + 1);
}

void bw_i2c_lcd_text_line_1(char *text, uint8_t length) {
	static char cmd[] = { BW_PORT_WRITE_MOVE_CURSOR, 0b0000000 };
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
	bw_i2c_lcd_text(text, length);
}

void bw_i2c_lcd_text_line_2(char *text, uint8_t length) {
	static char cmd[] = { BW_PORT_WRITE_MOVE_CURSOR, 0b0100000 };
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
	bw_i2c_lcd_text(text, length);
}

void bw_i2c_lcd_text_line_3(char *text, uint8_t length) {
	static char cmd[] = { BW_PORT_WRITE_MOVE_CURSOR, 0b1000000 };
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
	bw_i2c_lcd_text(text, length);
}

void bw_i2c_lcd_text_line_4(char *text, uint8_t length) {
	static char cmd[] = { BW_PORT_WRITE_MOVE_CURSOR, 0b1100000 };
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
	bw_i2c_lcd_text(text, length);
}

void bw_i2c_lcd_cls(void) {
	static char cmd[] = { BW_PORT_WRITE_CLEAR_SCREEN, ' ' };
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
}

void bw_i2c_lcd_set_contrast(uint8_t value) {
	static char cmd[] = { BW_PORT_WRITE_SET_CONTRAST, 0x00 };
	cmd[1] = value;
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
}

void bw_i2c_lcd_set_backlight(uint8_t value) {
	static char cmd[] = { BW_PORT_WRITE_SET_BACKLIGHT, 0x00 };
	cmd[1] = value;
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
}

void bw_i2c_lcd_reinit(void) {
	static char cmd[] = { BW_PORT_WRITE_REINIT_LCD, ' ' };
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
}

void bw_i2c_lcd_read_id(void) {
	static char cmd[] = { BW_PORT_READ_ID_STRING };
	char buf[BW_LCD_ID_STRING_LENGTH + 1];
	lcd_i2c_setup();
	bcm2835_i2c_write(cmd, sizeof(cmd) / sizeof(char));
	bcm2835_i2c_read(buf, BW_LCD_ID_STRING_LENGTH);
	buf[BW_LCD_ID_STRING_LENGTH] = '\0';
	printf("[%s]\r\n", buf);
}
