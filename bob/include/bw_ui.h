#ifndef BW_UI_H_
#define BW_UI_H_

#include <stdint.h>

#define BW_UI_DEFAULT_SLAVE_ADDRESS		0x94
#define BW_UI_I2C_BYTE_WAIT_US			20

#define BW_UI_OK 						0
#define BW_UI_ERROR						1

#define BW_UI_MAX_CHARACTERS			16
#define BW_UI_MAX_LINES					2
#define BW_UI_ID_STRING_LENGTH			20

typedef enum
{
	BW_UI_BUTTON1	= 0,	///<
	BW_UI_BUTTON2	= 1,	///<
	BW_UI_BUTTON3	= 2,	///<
	BW_UI_BUTTON4	= 3,	///<
	BW_UI_BUTTON5	= 4,	///<
	BW_UI_BUTTON6	= 5		///<
} bwUiButtons;

/// read ports
#define BW_PORT_READ_BUTTON_SINCE_LAST		0x31

#define BW_PORT_READ_BUTTON_1				0x40
#define BW_PORT_READ_BUTTON_2				0x41
#define BW_PORT_READ_BUTTON_3				0x42
#define BW_PORT_READ_BUTTON_4				0x43
#define BW_PORT_READ_BUTTON_5				0x44
#define BW_PORT_READ_BUTTON_6				0x45

#ifdef BW_I2C_UI
#include <bw_i2c_ui.h>
#define ui_start 			bw_i2c_ui_start
#define ui_end				bw_i2c_ui_end
#define ui_reinit			bw_i2c_ui_reinit
#define ui_set_cursor		bw_i2c_ui_set_cursor
#define ui_text				bw_i2c_ui_text
#define ui_text_line_1		bw_i2c_ui_text_line_1
#define ui_text_line_2		bw_i2c_ui_text_line_2
#define ui_text_line_3		bw_i2c_ui_text_line_3
#define ui_text_line_4		bw_i2c_ui_text_line_4
#define	 ui_cls				bw_i2c_ui_cls
#define ui_set_contrast		bw_i2c_ui_set_contrast
#define ui_set_backlight	bw_i2c_ui_set_backlight
#define ui_read_id			bw_i2c_ui_read_id
#endif

#ifdef BW_SPI_UI
#include <bw_spi_ui.h>
#endif

extern int ui_start(char);
extern void ui_end(void);
extern void ui_reinit(void);
extern void ui_set_cursor(uint8_t, uint8_t );
extern void ui_text(char *, uint8_t );
extern void ui_text_line_1(char *, uint8_t);
extern void ui_text_line_2(char *, uint8_t);
extern void ui_text_line_3(char *, uint8_t);
extern void ui_text_line_4(char *, uint8_t);
extern void ui_cls(void);
extern void ui_read_id(void);

#endif /* BW_UI_H_ */
