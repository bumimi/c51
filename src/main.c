#include "config.h"
#include "GPIO.h"
#include "delay.h"
#include"EEPROM.h"
#include"STRING.H"
#include"timer.h"
#include"USART.h"

#define KEY1 1
#define KEY2 2
#define KEY3 6
#define KEY4 4
#define KEY5 5
#define Addr 0x0000

//ȫ�ֱ���
unsigned char LED_seg[11] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,0xff}; // ����ܹ����ֶ�
unsigned char LED_psi[8]  = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};             // �����λ��ѡ��
unsigned char LED_Buff[8];                  // �������ʾ���棬��ʼֵΪ0
sbit bell=P5^5;
typedef struct
{
	u8	Flag;		//У��
	u16 value;
} canshu;
xdata  canshu CANSHU;
unsigned char cnt;
extern unsigned char Flag_200ms,Flag_500ms,Flag_1s;
unsigned char Flag_set,Flag_run; //Flag_set���ñ�־λ	Flag_run��������ʱ��־λ
unsigned int sec_value; //����ʱֵ
bit F_dis;

//������������
char Scan_key(void);
void convert(unsigned long Num);
void save_canshu();
void bell_n(unsigned char n);
void Adjust(unsigned char key);
void canshu_init();

/******************** IO���ú���**************************/
void GPIO_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;        // �ṹ����
    GPIO_InitStructure.Pin  = GPIO_Pin_All;     
    GPIO_InitStructure.Mode = GPIO_PullUp;      
	//�����
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure); // ��ʼ��
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure); // ��ʼ��
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
}
//��ʱ������
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);		//��ֵ,100 000
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
}
//��������
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2
	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1����һ���ַ���

}


/******������***************/
void main(void)
{
	unsigned char key_buff;
    GPIO_config();
	Timer_config();
	UART_config();
	canshu_init();
	EA=1;
	convert(CANSHU.value);
    while(1) {
		if(Flag_200ms==1)//ÿ200msִ�а���ɨ��
		{
			Flag_200ms=0;
			key_buff = Scan_key();
			if(key_buff!=0)	//���а������£�ִ�а����ж�
			{
				Adjust(key_buff);	//�жϼ�ֵ
				if(Flag_run!=1){
					convert(CANSHU.value);	//���������䵽������
				}
				
			}
		}
			if(Flag_set==1 && Flag_500ms==1 )	//������״̬��1s��˸һ�Σ�500ms�ڣ�500ms��
			{
				Flag_500ms = 0;
				F_dis = ~F_dis;
				EA=0;	
				if(F_dis==1) memset(LED_Buff,10,sizeof(LED_Buff));	//����ʾ
				else convert(CANSHU.value);		//��ʾ
				EA=1;
			}
			if(Flag_run==1 && Flag_1s==1)	//��������ʱ״̬��ÿ��1S��ʾֵ��1
			{
				EA=0;
				Flag_1s = 0;	//1s��־λ ��0
				if(sec_value>0) sec_value--;
				else 
				{
					bell_n(2);	//����ʱΪ0��������������
					Flag_run = 0;//�˳�����ʱ״̬
					sec_value = CANSHU.value;	//���õ���ʱʱ��
				}
				convert(sec_value);	//���������䵽������
					EA=1;
			}
		
	}
}

//LED ����ʾ���ִ���
void convert(unsigned long n)
{
	unsigned char i;
	i=0;
	while(i<8)
	{
		if(n==0 && i>0)LED_Buff[i] =10;
		else 
		LED_Buff[i]=n%10;
		n= n /10;
		i++;
	}
}
//�������ʾ���� �޷���ֵ
void disp()
{
	if(cnt <7)cnt++;
	else cnt = 0;
	P0 = LED_seg[LED_Buff[cnt]];
	P2 = LED_psi[cnt];

}
//����ɨ�躯��  �а��� ���ؼ�ֵkey_buff
char Scan_key(void)
{
    unsigned char i = 0;
	unsigned char buff,Read_temp,key_buff;
	buff=0x01;
	key_buff=0;
	for(i=0;i<4;i++)
	{
		P1=~buff;//P1=0xFE;
		Read_temp = P1&0xF0;//��ȡH����λ
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

//�жϼ�ֵ
void Adjust(unsigned char key){
	switch(key)
	{
		case KEY1:	//��������
			Flag_set = 1;
			Flag_run = 0;
			break;
		case KEY2:	//��
			if(Flag_set==1) CANSHU.value++;
			break;
		case KEY3:	//��
			if(Flag_set==1) CANSHU.value--;
			break;
		case KEY4:	//�˳����ò�����
			Flag_set=0;
			save_canshu();
			break;
		case KEY5:	//��������ʱ
			Flag_set = 0;
			Flag_run = 1;
			sec_value=CANSHU.value; //����ʱֵ
			bell_n(1);	//����������
			break;
	}
 }

//������ʼ��
void canshu_init()
{
	EEPROM_read_n(Addr,(u8 *)&CANSHU,sizeof(&CANSHU));
	if(CANSHU.Flag==0xff)	//δ��ʼ��
	{
		EEPROM_SectorErase(Addr);
		PrintString1("�����������\r\n");
		CANSHU.Flag = 1;
		CANSHU.value = 10;
		EEPROM_write_n(Addr,(u8 *)&CANSHU,sizeof(&CANSHU));
		PrintString1("д�����\r\n");

	}

}

//�������
void save_canshu(){
	EEPROM_SectorErase(Addr);	//����
	EEPROM_write_n(Addr,(u8 *)&CANSHU,sizeof(&CANSHU));	//д��
}

//�������������βΣ���Ĵ��� 
void bell_n(unsigned char n)
{
	unsigned char i;
	for(i=0;i<n;i++){
		bell = 0;
		delay_ms(100);
		bell = 1;
		delay_ms(200);
	}
}