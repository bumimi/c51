
#include	"config.h"
#include	"Exti.h"
#include<GPIO.h>
#include<delay.h>

sbit bell=P5^5;
unsigned char bell_num;
void GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//?á???¨??
	GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;		
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//???¨IO???????ò????·???,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);	//??????
}
/*************	本地常量声明	**************/


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/



/*************  外部函数和变量声明 *****************/
void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//结构定义

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_Fall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE?òDISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_Fall;	//中断模式 	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityLow;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE?òDISABLE
	Ext_Inilize(EXT_INT1,&EXTI_InitStructure);				//初始化INT1	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4

	Ext_Inilize(EXT_INT2,&EXTI_InitStructure);				//初始化INT2	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
	Ext_Inilize(EXT_INT3,&EXTI_InitStructure);				//初始化INT3	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
	//Ext_Inilize(EXT_INT4,&EXTI_InitStructure);				//??????INT4	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}

/**********************************************/
void main(void)
{
    unsigned char i;
    GPIO_config();
	EXTI_config();
	EA = 1;

	while (1)
	{
        for(i=0;i<bell_num;i++){
            bell = 0;//蜂鸣器响
            delay_ms(200);
            bell = 1;//蜂鸣器不响
            delay_ms(200);
        }
        bell_num=0;
	}
}
