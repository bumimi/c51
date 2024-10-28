
#include"config.h"
#include<GPIO.h>
#include<delay.h>
#include<timer.h>

sbit bell=P5^5;
// unsigned char bell_num;

unsigned char LED_seg[11]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff}; // 共阳 字模
unsigned char LED_psi[8] ={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};  //位置
unsigned char LED_buff[8]={10,10,10,10,10,10,10,10};
unsigned char displayNum = 0;
unsigned char count=0;
unsigned disCount = 0;
unsigned char i;


void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_All;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;				//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);			//初始化
	GPIO_InitStructure.Mode = GPIO_PullUp;				//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);			//初始化
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);			//初始化

}

/************************ 定时器配置 Timer0  ****************************/
void Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000);		//初值,1ms
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
}


//数码管，整型数字存数组
void convert(unsigned long n){
    //中断允许总控制位使能,低电平不允许中断
    EA = 0;
    for(i=0;i<8;i++){
         if(n==0)LED_buff[i]=10;
         else {
            LED_buff[i]=n%10;
            n/=10;
         }
    }
    //高电平允许中断
   EA = 1;
}

//中断函数，每1ms调用一次
void display(){
    //每1ms刷新1位数码管
    if(count < 7) count++;
    else count = 0;
    //显示数码管
    P0=LED_seg[LED_buff[count]];
    P2=LED_psi[count];
    //数码管显示的值 每隔1s加1，加到10时，蜂鸣器响，然后复位
    if(disCount<1000) disCount++; 
    else  {
        disCount=0;
        displayNum++; 
    }  
    if(displayNum>10) {
        displayNum = 0;
        bell = 0;//蜂鸣器响
        delay_ms(500);
        bell = 1;
    }
}
//主函数
void main(void)
{
    //配置
    GPIO_config();
    Timer_config();
	EA = 1;
    while(1){
        convert(displayNum);    
        delay_ms(500);
    }
}

