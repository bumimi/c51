/**************************************
;WWW.STCMCU.COM
;Mobile:13922805190
;0755-82948412
温度传感器DS18B20测试程序
主芯片  : STC12C5A60S2 (1T)
工作频率: 12.000MHz
**************************************/


//typedef unsigned char BYTE;
sbit TEMP1 = P4^5;                  //DS18B20的数据口位P4.5---内部温度
unsigned char TPH;                           //存放温度值的高字节
unsigned char TPL;                           //存放温度值的低字节

void DelayXus(unsigned char n);
void DS18B20_Reset(unsigned char n);
unsigned char DS18B20_ReadByte(unsigned char n);
void DS18B20_WriteByte(unsigned char n,unsigned char dat);


unsigned int Ds18b20ReadTemp(unsigned char num)
{
	  unsigned int T_dat;
    DS18B20_Reset(num);                //设备复位
    DS18B20_WriteByte(num,0xCC);        //跳过ROM命令
    DS18B20_WriteByte(num,0x44);        //开始转换命令
    //while (!TEMP3);                    //等待转换完成
    DelayXus(200);
	  DelayXus(200);
    DS18B20_Reset(num);                //设备复位
    DS18B20_WriteByte(num,0xCC);        //跳过ROM命令
    DS18B20_WriteByte(num,0xBE);        //读暂存存储器命令
    TPL = DS18B20_ReadByte(num);       //读温度低字节
    TPH = DS18B20_ReadByte(num);       //读温度高字节
		T_dat = 0x0000;
		T_dat = TPH;
		T_dat <<= 8;
		T_dat = T_dat | TPL;         //将LSB和MSB整合成为一个16位的整数
		//首先通过温度数据的高5位判断采用结果是正温度还是负温度
			if((T_dat & 0xf800) == 0x0000)    //正温度的处理办法
			{
				T_dat >>= 4;         //取出温度结果的整数部分
				T_dat = T_dat*10;    //放大10倍，然后加上小数部分
				T_dat = T_dat + (TPL&0x0f)*0.625;
			}
			else T_dat=0;
    return(T_dat);
}

/**************************************
延时X微秒(STC12C5A60S2@12M)
不同的工作环境,需要调整此函数
此延时函数是使用1T的指令周期进行计算,与传统的12T的MCU不同
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
	   TEMP1 = sta;//送出低电平复位信号
	
}
bit Read_IO_sta(unsigned char n)
{
	bit sta_7;
	   sta_7=TEMP1;
	return sta_7;
}
/**************************************
复位DS18B20,并检测设备是否存在
**************************************/
void DS18B20_Reset(unsigned char n)
{
    CY = 1;
    //while (CY)
    {
				Set_IO_sta(0);
			  //TEMP3 = 0;  
        DelayXus(240);              //延时至少480us
        DelayXus(240);
        Set_IO_sta(1);//TEMP3 = 1;                     //释放数据线
        DelayXus(60);               //等待60us
        CY = Read_IO_sta();                    //检测存在脉冲
			
        DelayXus(240);              //等待设备释放数据线
        DelayXus(180);
    }
}

/**************************************
从DS18B20读1字节数据
**************************************/
unsigned char DS18B20_ReadByte(unsigned char n)
{
    unsigned char i;
    unsigned char dat = 0;

    for (i=0; i<8; i++)             //8位计数器
    {
        dat >>= 1;
        Set_IO_sta(0);//TEMP3 = 0;                     //开始时间片
        DelayXus(1);                //延时等待
        Set_IO_sta(1);//TEMP3 = 1;                     //准备接收
        DelayXus(1);                //接收延时
        if (Read_IO_sta()) dat |= 0x80;        //读取数据
        DelayXus(60);               //等待时间片结束
    }

    return dat;
}

/**************************************
向DS18B20写1字节数据
**************************************/
void DS18B20_WriteByte(unsigned char n,unsigned char dat)
{
    char i;

    for (i=0; i<8; i++)             //8位计数器
    {
        Set_IO_sta(0);//TEMP3 = 0;                     //开始时间片
        DelayXus(1);                //延时等待
        dat >>= 1;                  //送出数据
			  Set_IO_sta(CY);//TEMP3 = CY;
        DelayXus(60);               //等待时间片结束
        Set_IO_sta(1);//TEMP3 = 1;                     //恢复数据线
        DelayXus(1);                //恢复延时
    }
}
