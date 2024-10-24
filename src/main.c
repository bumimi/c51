#include "config.h"
#include "GPIO.h"
#include "delay.h"
#include <stdio.h>
#define LED_SEGMENT P0
#define LED_POSTION P2
unsigned char LED_seg[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90}; // 数码管共阳字段
unsigned char LED_psi[8]  = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};             // 数码管位置选择
unsigned char LED_Buff[8] ={0};                  // 数码管显示缓存，初始值为0
/******************** IO配置函数**************************/
void GPIO_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;        // 结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_All;     
    GPIO_InitStructure.Mode = GPIO_PullUp;      
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure); // 初始化

    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure); // 初始化
}
/******************** LED 待显示数字处理**************************/

void Led_DisplayNumProcess(unsigned int Num)
{
    char i = 0;
    while (i < 8) {
        LED_Buff[i] = (unsigned char)Num % 10;
        Num /= 10;
        i++;
    }
}
//按键扫描函数  有按键 返回键值
char Scan_key(void)
{
    unsigned char i = 0;
	unsigned char buff,Read_temp,key_buff;
	buff=0x01;
	key_buff=0;
	for(i=0;i<4;i++)
	{
		P1=~buff;//P1=0xFE;
		Read_temp = P1&0xF0;//读取H高四位
		switch(Read_temp)
		{
			case 0xE0: key_buff=1+i*4;break;
			case 0xD0: key_buff=2+i*4;break;
			case 0xB0: key_buff=3+i*4;break;
			case 0x70: key_buff=4+i*4;break;
		}
		buff<<=1;
	}
	if(key_buff==0)return -1;
	else return key_buff;
}

void main(void)
{
    char i = 0;
    unsigned int cnt= 0;
    char KeyValue = 0;
    unsigned int DisplayNum = 0;
    GPIO_config();
    while(1) {
        KeyValue = Scan_key();
        if (KeyValue > 0) {
            if (1 == KeyValue) {
                DisplayNum++;
                if (DisplayNum > 99999) {
                    DisplayNum = 0;
                }
            }
            if (2 == KeyValue) {
                DisplayNum--;
                if (DisplayNum == 0) {
                    DisplayNum = 0;
                }
            }
            Led_DisplayNumProcess(DisplayNum);
        }
        for (i = 0; i < 8; i++) {
            LED_SEGMENT  = LED_seg[LED_Buff[i]];
            LED_POSTION = LED_psi[i];
            delay_ms(2);
        }
    }
}
