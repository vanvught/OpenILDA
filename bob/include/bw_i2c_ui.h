#ifndef BW_I2C_UI_H_
#define BW_I2C_UI_H_

#include <stdint.h>

extern int bw_i2c_ui_start (char);
extern void bw_i2c_ui_end (void);

extern char bw_i2c_ui_read_button(char);
extern char bw_i2c_ui_read_button_last(void);

extern void bw_i2c_ui_reinit(void);

extern void bw_i2c_ui_text_line_1(char *, uint8_t);
extern void bw_i2c_ui_text_line_2(char *, uint8_t);
extern void bw_i2c_ui_text_line_3(char *, uint8_t);
extern void bw_i2c_ui_text_line_4(char *, uint8_t);

extern void bw_i2c_ui_cls(void);
extern void bw_i2c_ui_set_contrast(uint8_t);
extern void bw_i2c_ui_set_backlight(uint8_t);

extern void bw_i2c_ui_read_id(void);

#endif /* BW_I2C_UI_H_ */
