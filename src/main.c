#include<GPIO.h>
#include<delay.h>
#include<config.h>
#include<STDIO.H>
unsigned char LED_seg[11]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff}; // ���� ��ģ
unsigned char LED_psi[8] ={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};  //λ��
unsigned char LED_buff[8] = {0};
unsigned long displayNum=0;
//GPIO���ó�ʼ��
void GPIO_config(void){
    GPIO_InitTypeDef GPIO_InitStructure; //�ṹ����
    GPIO_InitStructure.Pin = GPIO_Pin_All; //ָ����ʼ��IO
    GPIO_InitStructure.Mode = GPIO_PullUp; //����׼˫���
    GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);
    GPIO_Inilize(GPIO_P2, &GPIO_InitStructure);
}
//���������ִ��������
void convert(unsigned long n){
    unsigned char i=0;
    // 12345 ����> LED_buff[8]={5,4,3,2,1,0,0,0}
    for(i=0;i<8;i++){
        LED_buff[i]=n%10;
        n=n/10;
    }
    //ȥ��  LED_seg[10]����ʾ   LED_buff[8]={5,4,3,2,1,10,10,10}
    if(n==0) return;
    for(i=0;i<8;i++){
        if(LED_buff[7-i]==0) LED_buff[7-i]=10;
        else break;
    }
}

//����ɨ��
unsigned char scan_key(void){
    unsigned char i=0,temp,tp,key_value;
    key_value=0;
    temp=0x01;
    while(i<4){
        P1=~temp;   //0xfe
        tp=P1&0xf0;
        if(tp!=0xf0){
            if(tp==0xe0) key_value=1+i*4;       //1110
            else if(tp==0xd0) key_value=2+i*4;   //1101
            else if(tp==0xb0) key_value=3+i*4;   //1011
            else if(tp==0x70) key_value=4+i*4;   //0111
        }
        temp<<=1;
        i++;
    }
    //S1���� S2��1 S3��1
    if(key_value==1){
        displayNum=0;
    }else if(key_value==2){
        displayNum++;
    }else if(key_value==3){
        displayNum--;
    }
    return key_value;

}


//main����
void main(void){
    unsigned char i,key_value;
    key_value=0; 
    GPIO_config();
    while(1){
         key_value =scan_key();
         if(key_value!=0) convert(displayNum);
         for(i=0;i<8;i++){
            P0=LED_seg[LED_buff[i]];
            P2=LED_psi[i];
            delay_ms(2);
         }
    }
}