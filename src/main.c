#include <config.h>
#include <GPIO.h>
#include <delay.h>
//本地常量声明
unsigned char LED_seg[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char LED_psi[8] ={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F}; 
unsigned char LED_buff[8]={0};

void GPIO_config(void){
    GPIO_InitTypeDef GPIO_InitStructure;    //结构定义
    GPIO_InitStructure.Pin = GPIO_Pin_All;
    GPIO_InitStructure.Mode = GPIO_PullUp;
    GPIO_Inilize(GPIO_P0,&GPIO_InitStructure); //初始化
    //  GPIO_InitStructure.Pin = GPIO_Pin_All;
    // GPIO_InitStructure.Mode = GPIO_OUT_PP;
    GPIO_Inilize(GPIO_P2,&GPIO_InitStructure); //初始化

}
void convert(unsigned int n){
    unsigned char i = 0;
    while(i<8){
        LED_buff[i] = (unsigned char)(n%10);
        n=n/10;
        i++;
    }
}
    void main(void){
        int i;
        unsigned int PlusCountFlag = 0; //计数标志
        GPIO_config();
        while(1){
            PlusCountFlag++;
            convert(PlusCountFlag);
            for(i=0;i<8;i++){
                P0 = LED_seg[LED_buff[i]];
                P2 = LED_psi[i];
                delay_ms(2);
            }
            if(PlusCountFlag>1024) PlusCountFlag=0;
        }
    }
