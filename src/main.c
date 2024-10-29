/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-10-29 10:47:39
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-10-29 11:25:56
 * @FilePath: \06USART\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include	"config.h"
#include	"delay.h"
#include	"USART.h"
#include	<stdio.h>
#include	<string.h>
void Uart1_Init(void)	//115200bps@22.1184MHz
{
	COMx_InitDefine COM1={0};
	COM1.UART_Mode			= UART_8bit_BRTx;//模式,UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COM1.UART_BRT_Use		= BRT_Timer1;	//使用波特率,   BRT_Timer1,BRT_Timer2
	COM1.UART_BaudRate		= 115200ul;		//波特率, 一般 110 ~ 115200
	COM1.UART_RxEnable		= ENABLE;		//允许接收,   ENABLE,DISABLE
	COM1.BaudRateDouble		= DISABLE;		//波特率加倍, ENABLE,DISABLE
	COM1.UART_Interrupt		= ENABLE;		//中断控制,   ENABLE,DISABLE
	COM1.UART_Polity		= PolityLow;	//优先级,     PolityLow,PolityHigh
	COM1.UART_P_SW			= UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COM1.UART_RXD_TXD_Short	= DISABLE;	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1,&COM1);
	PrintString1("USART INIT\n\r");
}
void main(void)
{
	u8 buf[20]=0;
	u16 a=9;
	Uart1_Init();
	EA=1;
	while(1)
	{
		sprintf(buf,"ADC_value=%d\n",a);
		PrintString1(buf);
		memset(buf,0,sizeof(buf));
		delay_ms(800);
	}
}