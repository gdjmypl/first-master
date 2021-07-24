/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   AT24C02测试（软件模拟I2C）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h" 
#include "oled.h"
#include "bsp_i2c_gpio.h"


extern const unsigned char BMP1[];

static void Delay(void)  //乱写的一个延时
{
	uint16_t i;
	uint16_t j;
	for (i = 0; i < 1000; i++)
	 for (j = 0; j < 10000; j++);
}
 /**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
	
int main(void)
{
    uint8_t a;
	  uint8_t i;
    LED_GPIO_Config();
  
    LED_BLUE;

    /* 串口初始化 */
		USART_Config();

		printf("oled软件模拟i2c测试例程 \r\n");
    a = i2c_CheckDevice(0x78);
	  printf("a = %d \r\n", a );
    OLED_Init();
    printf("oled初始化完成 \r\n");	
	  OLED_CLS();
		OLED_ShowStr(0,3,(unsigned char*)"2021-07-23",1);				//测试6*8字符
		OLED_ShowStr(0,4,(unsigned char*)"Keep going!",2);				//测试8*16字符
	  Delay();
	  OLED_CLS();
   	OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示

}
/*********************************************END OF FILE**********************/
