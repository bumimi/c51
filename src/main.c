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

//全局变量
unsigned char LED_seg[11] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,0xff}; // 数码管共阳字段
unsigned char LED_psi[8]  = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};             // 数码管位置选择
unsigned char LED_Buff[8];                  // 数码管显示缓存，初始值为0
sbit bell=P5^5;
typedef struct
{
	u8	Flag;		//校验
	u16 value;
} canshu;
xdata  canshu CANSHU;
unsigned char cnt;
extern unsigned char Flag_200ms,Flag_500ms,Flag_1s;
unsigned char Flag_set,Flag_run; //Flag_set设置标志位	Flag_run启动倒计时标志位
unsigned int sec_value; //倒计时值
bit F_dis;

//函数声明部分
char Scan_key(void);
void convert(unsigned long Num);
void save_canshu();
void bell_n(unsigned char n);
void Adjust(unsigned char key);
void canshu_init();

/******************** IO配置函数**************************/
void GPIO_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;        // 结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_All;     
    GPIO_InitStructure.Mode = GPIO_PullUp;      
	//数码管
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure); // 初始化
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure); // 初始化
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
}
//定时器配置
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);		//初值,100 000
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
}
//串口配置
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2
	PrintString1("STC15F2K60S2 UART1 Test Prgramme!\r\n");	//SUART1发送一个字符串

}


/******主函数***************/
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
		if(Flag_200ms==1)//每200ms执行按键扫描
		{
			Flag_200ms=0;
			key_buff = Scan_key();
			if(key_buff!=0)	//若有按键按下，执行按键判断
			{
				Adjust(key_buff);	//判断键值
				if(Flag_run!=1){
					convert(CANSHU.value);	//把整数分配到数组中
				}
				
			}
		}
			if(Flag_set==1 && Flag_500ms==1 )	//在设置状态，1s闪烁一次，500ms黑，500ms亮
			{
				Flag_500ms = 0;
				F_dis = ~F_dis;
				EA=0;	
				if(F_dis==1) memset(LED_Buff,10,sizeof(LED_Buff));	//不显示
				else convert(CANSHU.value);		//显示
				EA=1;
			}
			if(Flag_run==1 && Flag_1s==1)	//启动倒计时状态，每隔1S显示值减1
			{
				EA=0;
				Flag_1s = 0;	//1s标志位 置0
				if(sec_value>0) sec_value--;
				else 
				{
					bell_n(2);	//倒计时为0，蜂鸣器响两声
					Flag_run = 0;//退出倒计时状态
					sec_value = CANSHU.value;	//重置倒计时时间
				}
				convert(sec_value);	//把整数分配到数组中
					EA=1;
			}
		
	}
}

//LED 待显示数字处理
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
//数码管显示函数 无返回值
void disp()
{
	if(cnt <7)cnt++;
	else cnt = 0;
	P0 = LED_seg[LED_Buff[cnt]];
	P2 = LED_psi[cnt];

}
//按键扫描函数  有按键 返回键值key_buff
char Scan_key(void)
{
    unsigned char i = 0;
	unsigned char buff,Read_temp,key_buff;
	buff=0x01;
	key_buff=0;
	for(i=0;i<4;i++)
	{
		P1=~buff;//P1=0xFE;
		Read_temp = P1&0xF0;//读取H高四位
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

//判断键值
void Adjust(unsigned char key){
	switch(key)
	{
		case KEY1:	//进入设置
			Flag_set = 1;
			Flag_run = 0;
			break;
		case KEY2:	//加
			if(Flag_set==1) CANSHU.value++;
			break;
		case KEY3:	//减
			if(Flag_set==1) CANSHU.value--;
			break;
		case KEY4:	//退出设置并保存
			Flag_set=0;
			save_canshu();
			break;
		case KEY5:	//启动倒计时
			Flag_set = 0;
			Flag_run = 1;
			sec_value=CANSHU.value; //倒计时值
			bell_n(1);	//蜂鸣器函数
			break;
	}
 }

//参数初始化
void canshu_init()
{
	EEPROM_read_n(Addr,(u8 *)&CANSHU,sizeof(&CANSHU));
	if(CANSHU.Flag==0xff)	//未初始化
	{
		EEPROM_SectorErase(Addr);
		PrintString1("扇区擦除完成\r\n");
		CANSHU.Flag = 1;
		CANSHU.value = 10;
		EEPROM_write_n(Addr,(u8 *)&CANSHU,sizeof(&CANSHU));
		PrintString1("写入完成\r\n");

	}

}

//保存参数
void save_canshu(){
	EEPROM_SectorErase(Addr);	//擦除
	EEPROM_write_n(Addr,(u8 *)&CANSHU,sizeof(&CANSHU));	//写入
}

//蜂鸣器函数，形参：响的次数 
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