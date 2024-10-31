#include<config.h>
#include<GPIO.h>
#include<delay.h>
#include<LCD1602.h>
#include<STRING.H>
#include<DS18B20.h>

// IO≈‰÷√
void GPIO_config(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 GPIO_InitStructure.Pin = GPIO_Pin_All;
 GPIO_InitStructure.Mode == GPIO_PullUp;
 GPIO_Inilize(GPIO_P0,  &GPIO_InitStructure);

 GPIO_InitStructure.Pin = GPIO_Pin_7;
 GPIO_InitStructure.Mode == GPIO_PullUp;
 GPIO_Inilize(GPIO_P3,  &GPIO_InitStructure);

 GPIO_InitStructure.Pin  = GPIO_Pin_2|GPIO_Pin_4;	
 GPIO_InitStructure.Mode == GPIO_PullUp;
 GPIO_Inilize(GPIO_P4,  &GPIO_InitStructure);
}

void main(void)
{
	unsigned int temp=0;
	GPIO_config();
	InitLcd1602();
	while(1){
	 LcdShowStr(0, 0, "temp:", strlen("temp:"));
	  temp =  Ds18b20ReadTemp(0);
	 LcdShowInt(strlen("temp:"), 0, temp);
	 LcdShowChar(strlen("temp:")+2,0,'.');
	 LcdShowChar(strlen("temp:")+3,0,('0'+temp%10));
	delay_ms(200);
	}

}
