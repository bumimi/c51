/*
*******************************************************************************
* 文件名：DS1302.c
* 描  述：实时时钟芯片DS1302驱动模块
* 版本号：v1.0.0
* 备  注：针对STC12系列单片机在总线时序中附加延时
*******************************************************************************
*/

#define  _DS1302_C
#include "config.h"
#include "DS1302.h"

#define  SEC_READ_ADD   0x81
#define  SEC_WRITE_ADD  0x80
#define  MIN_REAN_ADD   0x83
#define  MIN_WRITE_ADD  0x82
#define  HOUR_READ_ADD  0x85
#define  HOUR_WRITE_ADD 0x84
#define  DATE_READ_ADD  0x87
#define  DATE_WRITE_ADD 0x86
#define  MON_READ_ADD   0x89
#define  MON_WRITE_ADD  0x88
#define  DAY_READ_ADD   0x8B
#define  DAY_WRITE_ADD  0x8A
#define  YEAR_READ_ADD  0x8D
#define  YEAR_WRITE_ADD 0x8C
u16 code READ_RTC_ADDR[7]  = {SEC_READ_ADD, MIN_REAN_ADD, HOUR_READ_ADD, 
                              DATE_READ_ADD, MON_READ_ADD, DAY_READ_ADD, YEAR_READ_ADD};  //读寄存器的秒分钟日月周年
u16 code WRITE_RTC_ADDR[7] = {SEC_WRITE_ADD, MIN_WRITE_ADD, HOUR_WRITE_ADD, DATE_WRITE_ADD,
                              MON_WRITE_ADD, DAY_WRITE_ADD, YEAR_WRITE_ADD}; //写寄存器的秒分钟日月周年
                              
/**
 * @description: DS1302的读操作
 * @param {u8} add：寄存器地址
 * @return {u8}寄存器数值 BCD码
 */
u8 read1302(u8 add)			
{
	u8 i,dat,dat1;
	DS1302_CK=0;
	DS1302_CE=0;
	DS1302_CE=1;
	for(i=0;i<8;i++)
	{
		DS1302_IO=add&0x01;				//与上同理
		add>>=1;
		DS1302_CK=1;						//读操作时，时钟线做下降沿
		DS1302_CK=0;	
	}
	for(i=0;i<8;i++)//先读取的低位
	{
		dat1 = DS1302_IO;
		dat = (dat>>1) | (dat1<<7);	//dat1左移7位变成最高位，dat1右移一位变成次高位，循环操作就能够将数据完整的读出
		DS1302_CK = 1;
		DS1302_CK = 0;
			
	}
		DS1302_CE = 0;	
		return dat;					//返回dat值
}
/**
 * @description: DS1302的写操作
 * @param {u8} add：寄存器地址
 * @param {u8} dat：待写入数据 BCD码
 * @return {*}
 */
void write1302(u8 add,u8 dat)
{
	u8 i;
	DS1302_CE=0;							//一开始CE为0
	DS1302_CK=0;							//时钟线为0
	DS1302_CE=1;							//过段时间CE为1开始工作
	for(i=0;i<8;i++)				//FOR循环八次，写八次地址
	{
		
		DS1302_IO=add&0x01;				//从最低位开始发送
		add>>=1;					//右移一位，执行8次
		DS1302_CK=1;						//时钟线升高，上升沿
		DS1302_CK=0;						//时钟线降低，为下一次for循环的上升沿做准备
	}
	for(i=0;i<8;i++)			    //与上同理
	{
		
		DS1302_IO=dat&0x01;
		dat>>=1;
		DS1302_CK=1;
		DS1302_CK=0;	
	}
	DS1302_CE=0;							//CE为0，停止工作
}
/**
 * @description: 获取实时时间，即读取DS1302当前时间并转换为时间结构体格式
 * @param {sTime} *time：传入存储结构体地址
 * @return {*}
 */
void GetRealTime(struct sTime *time)
{
    u8 buf[8],i;

		for(i=0;i<7;i++)
		{
			buf[i]=read1302(READ_RTC_ADDR[i]) ;//读七次，依次输出秒分钟日月周年
		}
    time->year = buf[6] + 0x2000;
    time->mon  = buf[4];
    time->day  = buf[3];
    time->hour = buf[2];
    time->min  = buf[1];
    time->sec  = buf[0];
    time->week = buf[5];
}
/**
 * @description: 设定实时时间，时间结构体格式的设定时间转换为数组并写入DS1302 
 * @param {sTime} *time：设定的目标时间
 * @return {*}
 */
void SetRealTime(struct sTime *time)
{
    u8 buf[8],i;
    write1302(0x8E,0x00);//wp=0 撤销写保护以允许写入数据	
    buf[7] = 0;
    buf[6] = time->year;
    buf[5] = time->week;
    buf[4] = time->mon;
    buf[3] = time->day;
    buf[2] = time->hour;
    buf[1] = time->min;
    buf[0] = time->sec;
		for(i=0;i<7;i++)
	 {
		 write1302(WRITE_RTC_ADDR[i],buf[i]);//循环七次的目的就是为了能够将秒分钟日月周年输入，括号内就是为之前的写地址和写数据	
	 }
	 write1302(0x8e,0x80);			//重新赋予写保护
}

/**
 * @description: DS1302初始化，如发生掉电则重新设置初始时间
 * @return {*}
 */
void InitDS1302()
{
    u8 dat;
    struct sTime code InitTime[] = {  //默认初始值：2024-07-30 15:01:00 星期4
        0x2024,0x07,0x30, 0x15,0x01,0x00, 0x04};
    
    DS1302_CE = 0;  //初始化DS1302通信引脚
    DS1302_CK = 0;
		dat = read1302(0x81);//读秒
   // dat = DS1302SingleRead(0);  //读取秒寄存器
    if ((dat & 0x80) != 0)      //由秒寄存器最高位CH的值判断DS1302是否已停止
    {
        //DS1302SingleWrite(7, 0x00);  //wp=0 撤销写保护以允许写入数据	
        SetRealTime(&InitTime);      //设置DS1302为默认的初始时间
    }
}
