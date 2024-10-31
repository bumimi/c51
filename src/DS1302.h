
#ifndef	__DS1302_H
#define	__DS1302_H

#include	"config.h"
#include	"delay.h"

sbit DS1302_CK = P3^4;		
sbit DS1302_IO = P3^5;		
sbit DS1302_CE = P3^6;   // DS1302��λ


struct sTime {  //����ʱ��ṹ
    u16 year; //��
    u8 mon;   //��
    u8 day;   //��
    u8 hour;  //ʱ
    u8 min;   //��
    u8 sec;   //��
    u8 week;  //����
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