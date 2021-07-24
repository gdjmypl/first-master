#include "oled.h"
#include "codetab.h"
#include "bsp_i2c_gpio.h"


static void i2c_Delay(void)
{
	uint16_t i;
	uint16_t j;
	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 1000; i++)
	 for (j = 0; j < 10; j++);
}

/**************************************************
*函数名:void Oled_WriteCmd(u8 command)
*输入参数：command
*函数功能：利用IIC协议向OLED中写入命令
*时间：2021-7-23
*使用：Oled_WriteCmd();
***************************************************/
void Oled_WriteCmd(u8 command)
{
    i2c_Start();
    i2c_SendByte(0x78);//OLED地址
    i2c_WaitAck();
    i2c_SendByte(0x00);//寄存器地址
    i2c_WaitAck();
    i2c_SendByte(command);
    i2c_WaitAck();
    i2c_Stop();
}

/**************************************************
*函数名:void Oled_WriteData(u8 data)
*输入参数：data
*函数功能：利用IIC协议向OLED中写入数据
*时间：2021-7-23
*使用：Oled_WriteData();
***************************************************/
void Oled_WriteDate(u8 data)
{
    i2c_Start();
    i2c_SendByte(0x78);//OLED地址
    i2c_WaitAck();
    i2c_SendByte(0x40);//寄存器地址
    i2c_WaitAck();
    i2c_SendByte(data);
    i2c_WaitAck();
    i2c_Stop();
}


void OLED_Init(void)
{
    i2c_Delay(); //这里的延时很重要
	
    Oled_WriteCmd(0xAE); //display off
    Oled_WriteCmd(0x20); //Set Memory Addressing Mode    
    Oled_WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    Oled_WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
    Oled_WriteCmd(0xc8); //Set COM Output Scan Direction
    Oled_WriteCmd(0x00); //---set low column address
    Oled_WriteCmd(0x10); //---set high column address
    Oled_WriteCmd(0x40); //--set start line address
    Oled_WriteCmd(0x81); //--set contrast control register
    Oled_WriteCmd(0xff); //亮度调节 0x00~0xff
    Oled_WriteCmd(0xa1); //--set segment re-map 0 to 127
    Oled_WriteCmd(0xa6); //--set normal display
    Oled_WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    Oled_WriteCmd(0x3F); //
    Oled_WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    Oled_WriteCmd(0xd3); //-set display offset
    Oled_WriteCmd(0x00); //-not offset
    Oled_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    Oled_WriteCmd(0xf0); //--set divide ratio
    Oled_WriteCmd(0xd9); //--set pre-charge period
    Oled_WriteCmd(0x22); //
    Oled_WriteCmd(0xda); //--set com pins hardware configuration
    Oled_WriteCmd(0x12);
    Oled_WriteCmd(0xdb); //--set vcomh
    Oled_WriteCmd(0x20); //0x20,0.77xVcc
    Oled_WriteCmd(0x8d); //--set DC-DC enable
    Oled_WriteCmd(0x14); //
    Oled_WriteCmd(0xaf); //--turn on oled panel
}

/**************************************************
*函数名:void OLED_ON(void)
*输入参数：无
*函数功能：开启OLED
*时间：2017-10-16
*使用：OLED_ON();
***************************************************/
void OLED_ON(void)
{
    Oled_WriteCmd(0X8D);  //设置电荷泵
    Oled_WriteCmd(0X14);  //开启电荷泵
    Oled_WriteCmd(0XAF);  //OLED唤醒
}
 /**
  * @brief  OLED_OFF，让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
  * @param  无
	* @retval 无
  */
void OLED_OFF(void)
{
	Oled_WriteCmd(0X8D);  //设置电荷泵
	Oled_WriteCmd(0X10);  //关闭电荷泵
	Oled_WriteCmd(0XAE);  //OLED休眠
}

/**************************************************
*函数名:void OLED_SetPos(unsigned char x, unsigned char y)
*输入参数：  X:横坐标
			Y:纵坐标
*函数功能：设置起始点坐标
*时间：2017-10-16
*使用：OLED_SetPos(x,y);
***************************************************/
void OLED_SetPos(unsigned char x, unsigned char y) 
{ 
    Oled_WriteCmd(0xb0+y);
    Oled_WriteCmd(((x&0xf0)>>4)|0x10);
    Oled_WriteCmd((x&0x0f)|0x01);
}
/**************************************************
*函数名:void OLED_Fill(unsigned char fill_Data)
*输入参数：fill_Data
*函数功能：全屏填充
*时间：2017-10-16
*使用：OLED_Fill(0x00);
***************************************************/
void OLED_Fill(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        Oled_WriteCmd(0xb0+m);       //page0-page1
        Oled_WriteCmd(0x00);     //low column start address
        Oled_WriteCmd(0x10);     //high column start address
        for(n=0;n<128;n++)
            {
                Oled_WriteDate(fill_Data);
            }
    }
}

/**************************************************
*函数名:void OLED_CLS(void)
*输入参数：无
*函数功能：清屏
*时间：2017-10-16
*使用：OLED_CLS();
***************************************************/
void OLED_CLS(void)
{
    OLED_Fill(0x00);
}


/**************************************************
*函数名:void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
*输入参数： X:横坐标 (0~127)
		   Y：纵坐标 (0~7)
		   CH[]：字符串
		   TextSize：字体大小 （1：6*8   2：16*8）
*函数功能：写入一个字符串
*时间：2017-10-16
*使用：OLED_ShowStr(1,1,"HFUU",1);
***************************************************/
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0,i = 0,j = 0;
    switch(TextSize)
    {
        case 1:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 126)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<6;i++)
                    Oled_WriteDate(F6x8[c][i]);
                x += 6;
                j++;
            }
        }break;
        case 2:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<8;i++)
                    Oled_WriteDate(F8X16[c*16+i]);
                OLED_SetPos(x,y+1);
                for(i=0;i<8;i++)
                    Oled_WriteDate(F8X16[c*16+i+8]);
                x += 8;
                j++;
            }
        }break;
    }
}

/**************************************************
*函数名:void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
*输入参数：   X:横坐标 (0~127)
		     Y：纵坐标 (0~7)
			 N:取模数组中相应字在其中的位置
*函数功能：写入中文字
*时间：2017-10-16
*使用：OLED_ShowCN(1,1,1);
***************************************************/
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm <16;wm++)
	{
		Oled_WriteDate(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		Oled_WriteDate(F16x16[adder]);
		adder += 1;
	}
}

/**************************************************
*函数名:void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
*输入参数：   X0:起始横坐标		X1终点横坐标:
		      Y0：起始纵坐标		Y1：终点纵坐标
			  BMP：图片取模数组的首地址
*函数功能：画一张照片
*时间：2017-10-16
*使用：OLED_DrawBMP(0,127,0,7,BMP1);
***************************************************/
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			Oled_WriteDate(BMP[j++]);
		}
	}
}


void Oled_WriteOneNum(u8 x,u8 y,u8 N )
{
	unsigned char wm=0;
	unsigned int  adder=16*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm <8;wm++)
	{
		Oled_WriteDate(F8X16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 8;wm++)
	{
		Oled_WriteDate(F8X16[adder]);
		adder += 1;
	}	
}

void Oled_WriteNum(u8 x,u8 y,unsigned int  N )
{
	
	u8 a,b,c,d,e;
	a=N/10000;
	b=N/1000%10;
	c=N/100%10;
	d=N/10%10;
	e=N%10;
	Oled_WriteOneNum(x,y,a);
	Oled_WriteOneNum(x+8,y,b);
	Oled_WriteOneNum(x+16,y,c);
	Oled_WriteOneNum(x+24,y,d);
	Oled_WriteOneNum(x+32,y,e);
}

void Oled_WriteTimeNum(u8 x,u8 y,u8 N)
{
	u8 a,b;
	a=N/10;
	b=N%10;
	Oled_WriteOneNum(x*8,y,a);
	Oled_WriteOneNum((x+1 )*8,y,b);
}


