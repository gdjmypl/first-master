/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   AT24C02���ԣ����ģ��I2C��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h" 
#include "oled.h"
#include "bsp_i2c_gpio.h"


extern const unsigned char BMP1[];

static void Delay(void)  //��д��һ����ʱ
{
	uint16_t i;
	uint16_t j;
	for (i = 0; i < 1000; i++)
	 for (j = 0; j < 10000; j++);
}
 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
	
int main(void)
{
    uint8_t a;
	  uint8_t i;
    LED_GPIO_Config();
  
    LED_BLUE;

    /* ���ڳ�ʼ�� */
		USART_Config();

		printf("oled���ģ��i2c�������� \r\n");
    a = i2c_CheckDevice(0x78);
	  printf("a = %d \r\n", a );
    OLED_Init();
    printf("oled��ʼ����� \r\n");	
	  OLED_CLS();
		OLED_ShowStr(0,3,(unsigned char*)"2021-07-23",1);				//����6*8�ַ�
		OLED_ShowStr(0,4,(unsigned char*)"Keep going!",2);				//����8*16�ַ�
	  Delay();
	  OLED_CLS();
   	OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ

}
/*********************************************END OF FILE**********************/
