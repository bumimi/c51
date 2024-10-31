/**************************************
;WWW.STCMCU.COM
;Mobile:13922805190
;0755-82948412
�¶ȴ�����DS18B20���Գ���
��оƬ  : STC12C5A60S2 (1T)
����Ƶ��: 12.000MHz
**************************************/


//typedef unsigned char BYTE;
sbit TEMP1 = P4^5;                  //DS18B20�����ݿ�λP4.5---�ڲ��¶�
unsigned char TPH;                           //����¶�ֵ�ĸ��ֽ�
unsigned char TPL;                           //����¶�ֵ�ĵ��ֽ�

void DelayXus(unsigned char n);
void DS18B20_Reset(unsigned char n);
unsigned char DS18B20_ReadByte(unsigned char n);
void DS18B20_WriteByte(unsigned char n,unsigned char dat);


unsigned int Ds18b20ReadTemp(unsigned char num)
{
	  unsigned int T_dat;
    DS18B20_Reset(num);                //�豸��λ
    DS18B20_WriteByte(num,0xCC);        //����ROM����
    DS18B20_WriteByte(num,0x44);        //��ʼת������
    //while (!TEMP3);                    //�ȴ�ת�����
    DelayXus(200);
	  DelayXus(200);
    DS18B20_Reset(num);                //�豸��λ
    DS18B20_WriteByte(num,0xCC);        //����ROM����
    DS18B20_WriteByte(num,0xBE);        //���ݴ�洢������
    TPL = DS18B20_ReadByte(num);       //���¶ȵ��ֽ�
    TPH = DS18B20_ReadByte(num);       //���¶ȸ��ֽ�
		T_dat = 0x0000;
		T_dat = TPH;
		T_dat <<= 8;
		T_dat = T_dat | TPL;         //��LSB��MSB���ϳ�Ϊһ��16λ������
		//����ͨ���¶����ݵĸ�5λ�жϲ��ý�������¶Ȼ��Ǹ��¶�
			if((T_dat & 0xf800) == 0x0000)    //���¶ȵĴ���취
			{
				T_dat >>= 4;         //ȡ���¶Ƚ������������
				T_dat = T_dat*10;    //�Ŵ�10����Ȼ�����С������
				T_dat = T_dat + (TPL&0x0f)*0.625;
			}
			else T_dat=0;
    return(T_dat);
}

/**************************************
��ʱX΢��(STC12C5A60S2@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
����ʱ������ʹ��1T��ָ�����ڽ��м���,�봫ͳ��12T��MCU��ͬ
**************************************/
void DelayXus(unsigned char n)
{
    while (n--)
    {
        _nop_();
        _nop_();
    }
}
void Set_IO_sta(bit sta)
{
	   TEMP1 = sta;//�ͳ��͵�ƽ��λ�ź�
	
}
bit Read_IO_sta(unsigned char n)
{
	bit sta_7;
	   sta_7=TEMP1;
	return sta_7;
}
/**************************************
��λDS18B20,������豸�Ƿ����
**************************************/
void DS18B20_Reset(unsigned char n)
{
    CY = 1;
    //while (CY)
    {
				Set_IO_sta(0);
			  //TEMP3 = 0;  
        DelayXus(240);              //��ʱ����480us
        DelayXus(240);
        Set_IO_sta(1);//TEMP3 = 1;                     //�ͷ�������
        DelayXus(60);               //�ȴ�60us
        CY = Read_IO_sta();                    //����������
			
        DelayXus(240);              //�ȴ��豸�ͷ�������
        DelayXus(180);
    }
}

/**************************************
��DS18B20��1�ֽ�����
**************************************/
unsigned char DS18B20_ReadByte(unsigned char n)
{
    unsigned char i;
    unsigned char dat = 0;

    for (i=0; i<8; i++)             //8λ������
    {
        dat >>= 1;
        Set_IO_sta(0);//TEMP3 = 0;                     //��ʼʱ��Ƭ
        DelayXus(1);                //��ʱ�ȴ�
        Set_IO_sta(1);//TEMP3 = 1;                     //׼������
        DelayXus(1);                //������ʱ
        if (Read_IO_sta()) dat |= 0x80;        //��ȡ����
        DelayXus(60);               //�ȴ�ʱ��Ƭ����
    }

    return dat;
}

/**************************************
��DS18B20д1�ֽ�����
**************************************/
void DS18B20_WriteByte(unsigned char n,unsigned char dat)
{
    char i;

    for (i=0; i<8; i++)             //8λ������
    {
        Set_IO_sta(0);//TEMP3 = 0;                     //��ʼʱ��Ƭ
        DelayXus(1);                //��ʱ�ȴ�
        dat >>= 1;                  //�ͳ�����
			  Set_IO_sta(CY);//TEMP3 = CY;
        DelayXus(60);               //�ȴ�ʱ��Ƭ����
        Set_IO_sta(1);//TEMP3 = 1;                     //�ָ�������
        DelayXus(1);                //�ָ���ʱ
    }
}
