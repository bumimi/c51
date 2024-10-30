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
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_5;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	//��ʼ��
	
}
//��������
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2

	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���
//	PrintString2("STC15F2K60S2 UART2 Test Prgramme!\r\n");	//SUART2����һ���ַ���
}
void EEPROM_INIT(void)
{
	unsigned char tmp[20],i;
	
			EEPROM_read_n(Addr,tmp,sizeof(tmp));
			for(i=0; i<10; i++)	
			{
						TX1_write2buff(tmp[i]+0x30);	//�����ݷ��ظ�����
						TX1_write2buff(0x0d);
						TX1_write2buff(0x0a);
			}
			
			if(tmp[0] == 0xff)
			{
				EEPROM_SectorErase(Addr);
				PrintString1("�����������\r\n");			
				for(i=0; i<10; i++)	
				{
							tmp[i]=i+1;	//�����ݷ��ظ�����
				}	
				EEPROM_write_n(Addr,tmp,sizeof(tmp));
				PrintString1("д��������\r\n");
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