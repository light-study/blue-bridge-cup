/*
  程序说明: CT117E嵌入式竞赛板GPIO模拟I2C总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT117E嵌入式竞赛板
  日    期: 2011-8-9
*/

#include "eeprom.h"

/** I2C 总线接口 */
#define I2C_PORT GPIOB
#define SDA_Pin	GPIO_Pin_7
#define SCL_Pin GPIO_Pin_6

#define FAILURE 0
#define SUCCESS 1


#define EEPROM_IIC_ADDRESS_WRITE	0xa0
#define EEPROM_IIC_ADDRESS_READ		(EEPROM_IIC_ADDRESS_WRITE+1)



void i2c_init(void);
void delay1(unsigned int n);

void I2CStart(void);
void I2CStop(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
unsigned char I2CWaitAck(void);

void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);





void EEPROM_Init(void)
{
	i2c_init();	
}

void EEPROM_Write(u8 add, u16 data_u16)
{
	I2CStart();
	I2CSendByte(EEPROM_IIC_ADDRESS_WRITE);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	I2CSendByte( (u8)(data_u16>>8) );
	I2CWaitAck();
	I2CSendByte( (u8)(data_u16&0x00ff) );
	I2CWaitAck();
	I2CStop();				
}


void EEPROM_Read(u8 add, u16 *data_u16)
{
	I2CStart();
	I2CSendByte(EEPROM_IIC_ADDRESS_WRITE);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(EEPROM_IIC_ADDRESS_READ);
	I2CWaitAck();
	*data_u16 = ( I2CReceiveByte() <<8 );
	I2CSendAck();
	*data_u16 |= I2CReceiveByte();
	I2CSendNotAck();
	I2CStop();		
}

#ifdef EEPROM_U8_EN
void EEPROM_Write_u8(u8 add, u8 data)
{
	I2CStart();
	I2CSendByte(EEPROM_IIC_ADDRESS_WRITE);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	I2CSendByte( data );
	I2CWaitAck();
	I2CStop();				
}

void EEPROM_Read_u8(u8 add, u8 *data)
{
	I2CStart();
	I2CSendByte(EEPROM_IIC_ADDRESS_WRITE);
	I2CWaitAck();
	I2CSendByte(add);
	I2CWaitAck();
	
	I2CStart();
	I2CSendByte(EEPROM_IIC_ADDRESS_READ);
	I2CWaitAck();
	*data = I2CReceiveByte();
	I2CSendNotAck();
	I2CStop();		
}
#endif







//配置SDA信号线为输入模式
void SDA_Input_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

//配置SDA信号线为输出模式
void SDA_Output_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

//
void SDA_Output( uint16_t val )
{
	if ( val ) {
		GPIO_SetBits(I2C_PORT,SDA_Pin);
	} else {
		GPIO_ResetBits(I2C_PORT,SDA_Pin);
	}
}

//
void SCL_Output( uint16_t val )
{
	if ( val ) {
		GPIO_SetBits(I2C_PORT,SCL_Pin);
	} else {
		GPIO_ResetBits(I2C_PORT,SCL_Pin);
	}
}

//
uint8_t SDA_Input()
{
	return GPIO_ReadInputDataBit( I2C_PORT, SDA_Pin);
}

//延时程序
void delay1(unsigned int n)
{
	unsigned int i;
	for ( i=0;i<n;++i);
}

//I2C总线启动
void I2CStart(void)
{
	SDA_Output(1);delay1(500);
	SCL_Output(1);delay1(500);
	SDA_Output(0);delay1(500);
	SCL_Output(0);delay1(500);
}

//I2C总线停止
void I2CStop(void)
{
	SCL_Output(0); delay1(500);
	SDA_Output(0); delay1(500);
	SCL_Output(1); delay1(500);
	SDA_Output(1); delay1(500);

}

//等待应答
unsigned char I2CWaitAck(void)
{
	unsigned short cErrTime = 5;
	SDA_Input_Mode(); 
	delay1(500);
	SCL_Output(1);delay1(500);
	while(SDA_Input())
	{
		cErrTime--;
		delay1(500);
		if (0 == cErrTime)
		{
			SDA_Output_Mode();
			I2CStop();
			return FAILURE;
		}
	}
	SDA_Output_Mode();
	SCL_Output(0);delay1(500); 
	return SUCCESS;
}

//发送应答位
void I2CSendAck(void)
{
	SDA_Output(0);delay1(500);
	delay1(500);
	SCL_Output(1); delay1(500);
	SCL_Output(0); delay1(500);

}

//
void I2CSendNotAck(void)
{
	SDA_Output(1);
	delay1(500);
	SCL_Output(1); delay1(500);
	SCL_Output(0); delay1(500);

}

//通过I2C总线发送一个字节数据
void I2CSendByte(unsigned char cSendByte)
{
	unsigned char  i = 8;
	while (i--)
	{
		SCL_Output(0);delay1(500); 
		SDA_Output(cSendByte & 0x80); delay1(500);
		cSendByte += cSendByte;
		delay1(500); 
		SCL_Output(1);delay1(500); 
	}
	SCL_Output(0);delay1(500); 
}

//从I2C总线接收一个字节数据
unsigned char I2CReceiveByte(void)
{
	unsigned char i = 8;
	unsigned char cR_Byte = 0;
	SDA_Input_Mode(); 
	while (i--)
	{
		cR_Byte += cR_Byte;
		SCL_Output(0);delay1(500); 
		delay1(500); 
		SCL_Output(1);delay1(500); 
		cR_Byte |=  SDA_Input(); 
	}
	SCL_Output(0);delay1(500); 
	SDA_Output_Mode();
	return cR_Byte;
}

//I2C总线初始化
void i2c_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SDA_Pin | SCL_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 // **

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);

}
