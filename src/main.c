
#include"config.h"
#include<GPIO.h>
#include<delay.h>
#include<timer.h>

sbit bell=P5^5;
// unsigned char bell_num;

unsigned char LED_seg[11]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff}; // ���� ��ģ
unsigned char LED_psi[8] ={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};  //λ��
unsigned char LED_buff[8]={10,10,10,10,10,10,10,10};
unsigned char displayNum = 0;
unsigned char count=0;
unsigned disCount = 0;
unsigned char i;


void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_All;	//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);			//��ʼ��
	GPIO_InitStructure.Mode = GPIO_PullUp;				//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);			//��ʼ��
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);			//��ʼ��

}

/************************ ��ʱ������ Timer0  ****************************/
void Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000);		//��ֵ,1ms
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
}


//����ܣ��������ִ�����
void convert(unsigned long n){
    //�ж������ܿ���λʹ��,�͵�ƽ�������ж�
    EA = 0;
    for(i=0;i<8;i++){
         if(n==0)LED_buff[i]=10;
         else {
            LED_buff[i]=n%10;
            n/=10;
         }
    }
    //�ߵ�ƽ�����ж�
   EA = 1;
}

//�жϺ�����ÿ1ms����һ��
void display(){
    //ÿ1msˢ��1λ�����
    if(count < 7) count++;
    else count = 0;
    //��ʾ�����
    P0=LED_seg[LED_buff[count]];
    P2=LED_psi[count];
    //�������ʾ��ֵ ÿ��1s��1���ӵ�10ʱ���������죬Ȼ��λ
    if(disCount<1000) disCount++; 
    else  {
        disCount=0;
        displayNum++; 
    }  
    if(displayNum>10) {
        displayNum = 0;
        bell = 0;//��������
        delay_ms(500);
        bell = 1;
    }
}
//������
void main(void)
{
    //����
    GPIO_config();
    Timer_config();
	EA = 1;
    while(1){
        convert(displayNum);    
        delay_ms(500);
    }
}

