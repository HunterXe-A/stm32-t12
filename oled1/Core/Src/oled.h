#ifndef __OLED_H_
#define __OLED_H_

#include "main.h"

extern I2C_HandleTypeDef hi2c1;

void oled_init(void);
void oled_display_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void oled_show_string(uint8_t x, uint8_t y, char ch[], uint8_t TextSize);
void oled_showCN(unsigned char x, unsigned char y, unsigned char CN[],unsigned char N);
void oled_drawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void oled_showNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1);
void oled_full(uint8_t data);
void oled_cls(void);  //«Â∆¡
void oled_on(void);
void oled_off(void);

void show_title(void);
#endif
