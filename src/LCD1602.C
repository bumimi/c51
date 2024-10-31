#include	"LCD1602.h"

#define LCD1602_DB  P0
sbit LCD1602_RS = P4^4;
sbit LCD1602_RW = P4^2;
sbit LCD1602_E  = P3^7;


/* 等待液晶准备好 */
void LcdWaitReady()
{
    unsigned char sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;delay_ms(1);
    LCD1602_RW = 1;delay_ms(1);
    do {
        LCD1602_E = 1;delay_ms(1);
        sta = LCD1602_DB; //读取状态字
        LCD1602_E = 0;delay_ms(1);
    } while (sta & 0x80); //bit7等于1表示液晶正忙，重复检测直到其等于0为止
}
/* 向LCD1602液晶写入一字节命令，cmd-待写入命令值 */
void LcdWriteCmd(unsigned char cmd)
{
    LcdWaitReady();
    LCD1602_RS = 0;delay_ms(1);
    LCD1602_RW = 0;delay_ms(1);
    LCD1602_DB = cmd;delay_ms(1);
    LCD1602_E  = 1;delay_ms(1);
    LCD1602_E  = 0;delay_ms(1);
}
/* 向LCD1602液晶写入一字节数据，dat-待写入数据值 */
void LcdWriteDat(unsigned char dat)
{
    LcdWaitReady();
    LCD1602_RS = 1;delay_ms(1);
    LCD1602_RW = 0;delay_ms(1);
    LCD1602_DB = dat;delay_ms(1);
    LCD1602_E  = 1;delay_ms(1);
    LCD1602_E  = 0;delay_ms(1);
}
/* 设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 */
void LcdSetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    
    if (y == 0)  //由输入的屏幕坐标计算显示RAM的地址
        addr = 0x00 + x;  //第一行字符地址从0x00起始
    else
        addr = 0x40 + x;  //第二行字符地址从0x40起始
    LcdWriteCmd(addr | 0x80);  //设置RAM地址
}
/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，
   str-字符串指针，len-需显示的字符长度 */
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str, unsigned char len)
{
    LcdSetCursor(x, y);   //设置起始地址
    while (len--)         //连续写入len个字符数据
    {
        LcdWriteDat(*str++);  //先取str指向的数据，然后str自加1
    }
}
/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，显示一个字符 */  
void LcdShowChar(unsigned char x, unsigned char y, unsigned char dat)
{
    LcdSetCursor(x, y);   //设置起始地址
    LcdWriteDat(dat);  //先取str指向的数据，然后str自加1
}
/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，显示一个字符 */  
void LcdShowInt(unsigned char x, unsigned char y, unsigned int dat)           
{
    LcdSetCursor(x, y);   //设置起始地址
		if((dat/10000)%10 !=0)LcdWriteDat(0x30+((dat/10000)%10));
		if((dat/1000)%10  !=0)LcdWriteDat(0x30+((dat/1000)%10));
		if((dat/100)%10   !=0)LcdWriteDat(0x30+((dat/100)%10));
		if((dat/10)%10    !=0)LcdWriteDat(0x30+(dat/10)%10);
		if(dat%10         !=0)LcdWriteDat(0x30+dat%10 );
}
/* 初始化1602液晶 */
void InitLcd1602()
{
    LcdWriteCmd(0x38);delay_ms(1);  //16*2显示，5*7点阵，8位数据接口
    LcdWriteCmd(0x0C); delay_ms(1); //显示器开，光标关闭
    LcdWriteCmd(0x06); delay_ms(1); //文字不动，地址自动+1
    LcdWriteCmd(0x01); delay_ms(1); //清屏
}