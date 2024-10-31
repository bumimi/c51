
#ifndef	__DS1302_H
#define	__DS1302_H

#include	"config.h"
#include	"delay.h"

sbit DS1302_CK = P3^4;		
sbit DS1302_IO = P3^5;		
sbit DS1302_CE = P3^6;   // DS1302复位


struct sTime {  //日期时间结构
    u16 year; //年
    u8 mon;   //月
    u8 day;   //日
    u8 hour;  //时
    u8 min;   //分
    u8 sec;   //秒
    u8 week;  //星期
};

#ifndef _DS1302_C

#endif

void InitDS1302();
void GetRealTime(struct sTime *time);
void GetRealTime2(struct sTime *time);
void SetRealTime(struct sTime *time);


u8 read1302(u8 add)	;
void write1302(u8 add,u8 dat);
#endif