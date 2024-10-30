
#include	"config.h"
#include	"delay.h"
#include	"USART.h"
#include	<stdio.h>
#include	<string.h>
#include    <ADC.h>
#define VCC 4.95
//串口配置
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

//ADC配置
void ADC_config(void){
    ADC_InitTypeDef ADC_Handler = {0};
    u8	ADC_Px = ADC_P16;			//设置要做ADC的IO,	ADC_P10 ~ ADC_P17,ADC_P1_All
	u8	ADC_Speed = ADC_360T;		//ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	u8	ADC_Power = ENABLE;		//ADC功率允许/关闭	ENABLE,DISABLE
	u8	ADC_AdjResult = ADC_RES_H8L2;	//ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	u8	ADC_Polity = PolityHigh;		//优先级设置	PolityHigh,PolityLow
	u8	ADC_Interrupt = DISABLE;	//中断允许		ENABLE,DISABLE
    ADC_Inilize(&ADC_Handler);
}
void main(void)
{
	u8 buf[20]=0;
	u16 ADCres=0;
    float ADC_Vin1 = 0;
	Uart1_Init();
    ADC_config();
	EA=1;
	while(1)
	{
        ADCres = Get_ADC10bitResult(ADC_CH6);	
        ADC_Vin1 = (float)((ADCres/1024.00)*VCC);
		sprintf(buf,"ADCres=%d\n",ADCres);
		//sprintf(buf,"ADC_value=%.02f\n",ADC_Vin1);
		PrintString1(buf);
		memset(buf,0,sizeof(buf));
		delay_ms(2000);
	}
}