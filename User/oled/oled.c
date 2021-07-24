#include "oled.h"
#include "codetab.h"
#include "bsp_i2c_gpio.h"


static void i2c_Delay(void)
{
	uint16_t i;
	uint16_t j;
	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 1000; i++)
	 for (j = 0; j < 10; j++);
}

/**************************************************
*������:void Oled_WriteCmd(u8 command)
*���������command
*�������ܣ�����IICЭ����OLED��д������
*ʱ�䣺2021-7-23
*ʹ�ã�Oled_WriteCmd();
***************************************************/
void Oled_WriteCmd(u8 command)
{
    i2c_Start();
    i2c_SendByte(0x78);//OLED��ַ
    i2c_WaitAck();
    i2c_SendByte(0x00);//�Ĵ�����ַ
    i2c_WaitAck();
    i2c_SendByte(command);
    i2c_WaitAck();
    i2c_Stop();
}

/**************************************************
*������:void Oled_WriteData(u8 data)
*���������data
*�������ܣ�����IICЭ����OLED��д������
*ʱ�䣺2021-7-23
*ʹ�ã�Oled_WriteData();
***************************************************/
void Oled_WriteDate(u8 data)
{
    i2c_Start();
    i2c_SendByte(0x78);//OLED��ַ
    i2c_WaitAck();
    i2c_SendByte(0x40);//�Ĵ�����ַ
    i2c_WaitAck();
    i2c_SendByte(data);
    i2c_WaitAck();
    i2c_Stop();
}


void OLED_Init(void)
{
    i2c_Delay(); //�������ʱ����Ҫ
	
    Oled_WriteCmd(0xAE); //display off
    Oled_WriteCmd(0x20); //Set Memory Addressing Mode    
    Oled_WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    Oled_WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
    Oled_WriteCmd(0xc8); //Set COM Output Scan Direction
    Oled_WriteCmd(0x00); //---set low column address
    Oled_WriteCmd(0x10); //---set high column address
    Oled_WriteCmd(0x40); //--set start line address
    Oled_WriteCmd(0x81); //--set contrast control register
    Oled_WriteCmd(0xff); //���ȵ��� 0x00~0xff
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
*������:void OLED_ON(void)
*�����������
*�������ܣ�����OLED
*ʱ�䣺2017-10-16
*ʹ�ã�OLED_ON();
***************************************************/
void OLED_ON(void)
{
    Oled_WriteCmd(0X8D);  //���õ�ɱ�
    Oled_WriteCmd(0X14);  //������ɱ�
    Oled_WriteCmd(0XAF);  //OLED����
}
 /**
  * @brief  OLED_OFF����OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
  * @param  ��
	* @retval ��
  */
void OLED_OFF(void)
{
	Oled_WriteCmd(0X8D);  //���õ�ɱ�
	Oled_WriteCmd(0X10);  //�رյ�ɱ�
	Oled_WriteCmd(0XAE);  //OLED����
}

/**************************************************
*������:void OLED_SetPos(unsigned char x, unsigned char y)
*���������  X:������
			Y:������
*�������ܣ�������ʼ������
*ʱ�䣺2017-10-16
*ʹ�ã�OLED_SetPos(x,y);
***************************************************/
void OLED_SetPos(unsigned char x, unsigned char y) 
{ 
    Oled_WriteCmd(0xb0+y);
    Oled_WriteCmd(((x&0xf0)>>4)|0x10);
    Oled_WriteCmd((x&0x0f)|0x01);
}
/**************************************************
*������:void OLED_Fill(unsigned char fill_Data)
*���������fill_Data
*�������ܣ�ȫ�����
*ʱ�䣺2017-10-16
*ʹ�ã�OLED_Fill(0x00);
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
*������:void OLED_CLS(void)
*�����������
*�������ܣ�����
*ʱ�䣺2017-10-16
*ʹ�ã�OLED_CLS();
***************************************************/
void OLED_CLS(void)
{
    OLED_Fill(0x00);
}


/**************************************************
*������:void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
*��������� X:������ (0~127)
		   Y�������� (0~7)
		   CH[]���ַ���
		   TextSize�������С ��1��6*8   2��16*8��
*�������ܣ�д��һ���ַ���
*ʱ�䣺2017-10-16
*ʹ�ã�OLED_ShowStr(1,1,"HFUU",1);
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
*������:void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
*���������   X:������ (0~127)
		     Y�������� (0~7)
			 N:ȡģ��������Ӧ�������е�λ��
*�������ܣ�д��������
*ʱ�䣺2017-10-16
*ʹ�ã�OLED_ShowCN(1,1,1);
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
*������:void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
*���������   X0:��ʼ������		X1�յ������:
		      Y0����ʼ������		Y1���յ�������
			  BMP��ͼƬȡģ������׵�ַ
*�������ܣ���һ����Ƭ
*ʱ�䣺2017-10-16
*ʹ�ã�OLED_DrawBMP(0,127,0,7,BMP1);
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


