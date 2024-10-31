
#ifndef	__LCD1602_H
#define	__LCD1602_H

#include	"config.h"
#include	"delay.h"

void LcdWaitReady();
void LcdWriteCmd(unsigned char cmd);
void LcdWriteDat(unsigned char dat);
void LcdSetCursor(unsigned char x, unsigned char y);
void LcdShowStr(unsigned char x, unsigned char y,unsigned char *str, unsigned char len); 
                
void LcdShowChar(unsigned char x, unsigned char y,unsigned char dat); 
void LcdShowInt(unsigned char x, unsigned char y, unsigned int dat);

void InitLcd1602();



#endif