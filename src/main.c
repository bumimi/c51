
#include	"config.h"
#include	"Exti.h"
#include<GPIO.h>
#include<delay.h>

// sbit bell=P5^5;
// unsigned char bell_num;
unsigned char LED_seg[11]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff}; // ���� ��ģ
unsigned char LED_psi[8] ={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};  //λ��
unsigned char LED_buff[8]={0};
unsigned char displayNum = 0;
void GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		
	GPIO_InitStructure.Pin  = GPIO_Pin_All;		
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	

    GPIO_InitStructure.Mode = GPIO_PullUp;		
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	
}


/*************  �ⲿ�����ͱ������� *****************/
void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//�ṹ����

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_Fall;	//�ж�ģʽ,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//�ж����ȼ�,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE?��DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//��ʼ��INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_Fall;	//�ж�ģʽ 	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityLow;			//�ж����ȼ�,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE?��DISABLE
	Ext_Inilize(EXT_INT1,&EXTI_InitStructure);				//��ʼ��INT1	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4

	Ext_Inilize(EXT_INT2,&EXTI_InitStructure);				//��ʼ��INT2	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
	Ext_Inilize(EXT_INT3,&EXTI_InitStructure);				//��ʼ��INT3	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
	//Ext_Inilize(EXT_INT4,&EXTI_InitStructure);				//??????INT4	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}

/**********************************************/

//����ܣ��������ִ�����
void convert(unsigned long n){
    unsigned char i;
    for(i=0;i<8;i++){
        LED_buff[i]=n%10;
        n/=10;
    }
     if(displayNum==0) {
            for(i=0;i<7;i++){
                LED_buff[7-i]=10;
            }
            return ;
            }
    //ȥ��
    for(i=0;i<8;i++){
        if(LED_buff[7-i]==0)LED_buff[7-i]=10;
        else break;
    }
}
//�жϿ����������ʾ INT0 ��λ  INT1 ��  INT2 ��


void main(void)
{
    unsigned char i;
    GPIO_config();
	EXTI_config();
	EA = 1;

	// while (1)
	// {
    //     for(i=0;i<bell_num;i++){
    //         bell = 0;//��������
    //         delay_ms(200);
    //         bell = 1;//����������
    //         delay_ms(200);
    //     }
    //     bell_num=0;
	// }
    while(1){
        convert(displayNum);
        for(i=0;i<8;i++){
        P0=LED_seg[LED_buff[i]];
        P2=LED_psi[i];
        delay_ms(2);
        }
    }
}
