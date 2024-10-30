#include <config.h>
#include	"GPIO.h"
#include  "USART.h"
#include	"delay.h"
#include	"string.h"
#include "eeprom.h"
#define Addr  0x0000
unsigned char EEprom_buff[10];

void EEPROM_INIT(void);
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	//初始化
	
}
//串口配置
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2

	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串
//	PrintString2("STC15F2K60S2 UART2 Test Prgramme!\r\n");	//SUART2发送一个字符串
}
void EEPROM_INIT(void)
{
	unsigned char tmp[20],i;
	
			EEPROM_read_n(Addr,tmp,sizeof(tmp));
			for(i=0; i<10; i++)	
			{
						TX1_write2buff(tmp[i]+0x30);	//将数据返回给串口
						TX1_write2buff(0x0d);
						TX1_write2buff(0x0a);
			}
			
			if(tmp[0] == 0xff)
			{
				EEPROM_SectorErase(Addr);
				PrintString1("扇区擦除完成\r\n");			
				for(i=0; i<10; i++)	
				{
							tmp[i]=i+1;	//将数据返回给串口
				}	
				EEPROM_write_n(Addr,tmp,sizeof(tmp));
				PrintString1("写入操作完成\r\n");
		 }
}

void main(void)
{
	GPIO_config();
	UART_config();
	EEPROM_INIT();

	while(1)
	{
       delay_ms(100);
	}
}