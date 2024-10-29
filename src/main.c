/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-10-29 10:47:39
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-10-29 11:25:56
 * @FilePath: \06USART\main.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include	"config.h"
#include	"delay.h"
#include	"USART.h"
#include	<stdio.h>
#include	<string.h>
void Uart1_Init(void)	//115200bps@22.1184MHz
{
	COMx_InitDefine COM1={0};
	COM1.UART_Mode			= UART_8bit_BRTx;//ģʽ,UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COM1.UART_BRT_Use		= BRT_Timer1;	//ʹ�ò�����,   BRT_Timer1,BRT_Timer2
	COM1.UART_BaudRate		= 115200ul;		//������, һ�� 110 ~ 115200
	COM1.UART_RxEnable		= ENABLE;		//�������,   ENABLE,DISABLE
	COM1.BaudRateDouble		= DISABLE;		//�����ʼӱ�, ENABLE,DISABLE
	COM1.UART_Interrupt		= ENABLE;		//�жϿ���,   ENABLE,DISABLE
	COM1.UART_Polity		= PolityLow;	//���ȼ�,     PolityLow,PolityHigh
	COM1.UART_P_SW			= UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COM1.UART_RXD_TXD_Short	= DISABLE;	//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
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