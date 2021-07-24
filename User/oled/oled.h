#define __OLED_H__
#define __OLED_H__
#include "stm32f10x.h"

					
void WriteCmd(u8 command);							
void WriteDat(u8 data);
void OLED_Init(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_SetPos(unsigned char x, unsigned char y);							
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void LCD_PutHZ(unsigned int x,unsigned int y, unsigned char g[2]);
void LCD_PutString(unsigned int x,unsigned int y, unsigned char *s) ;
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void Oled_WriteOneNum(u8 x,u8 y,u8 N );
void Oled_WriteNum(u8 x,u8 y,unsigned int N );
void Oled_WriteTimeNum(u8 x,u8 y,u8 N);

