/*
  ����˵��: CT117EǶ��ʽ������GPIOģ��I2C������������
  �������: Keil uVision 4.10 
  Ӳ������: CT117EǶ��ʽ������
  ��    ��: 2011-8-9
*/

#include "at24c02.h"

/** I2C ���߽ӿ� */
#define I2C_PORT GPIOB
#define SDA_Pin	GPIO_Pin_7
#define SCL_Pin GPIO_Pin_6

#define FAILURE 0
#define SUCCESS 1



void delay1(unsigned int n);

void I2CStart(void);
void I2CStop(void);
void I2CSendAck(void);
void I2CSendNotAck(void);
unsigned char I2CWaitAck(void);

void I2CSendByte(unsigned char cSendByte);
unsigned char I2CReceiveByte(void);


void AT24C02_Write(u8 index, time_struct data)
{
	I2CStart();
	I2CSendByte(AT24C02_Write_Address);
	I2CWaitAck();
	I2CSendByte( BASE_ADDRESS + index*SIZE_ADDRESS);
	I2CWaitAck();

	I2CSendByte(data.h);
	I2CWaitAck();
	I2CSendByte(data.m);
	I2CWaitAck();
	I2CSendByte(data.s);
	I2CWaitAck();
	I2CStop();
}

void AT24C02_Read(u8 index, time_struct* data)
{
	I2CStart();
	I2CSendByte(AT24C02_Write_Address);
	I2CWaitAck();
	I2CSendByte( BASE_ADDRESS + index*SIZE_ADDRESS );
	I2CWaitAck();

	I2CStart();
	I2CSendByte(AT24C02_Read_Address);
	I2CWaitAck();
	data->h = I2CReceiveByte();
	I2CSendAck();
	data->m = I2CReceiveByte();
	I2CSendAck();
	data->s = I2CReceiveByte();
	I2CSendNotAck();
	I2CStop();
}




//����SDA�ź���Ϊ����ģʽ
void SDA_Input_Mode()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = SDA_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);
}

//����SDA�ź���Ϊ���ģʽ
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

//��ʱ����
void delay1(unsigned int n)
{
	unsigned int i;
	for ( i=0;i<n;++i);
}

//I2C��������
void I2CStart(void)
{
	SDA_Output(1);delay1(500);
	SCL_Output(1);delay1(500);
	SDA_Output(0);delay1(500);
	SCL_Output(0);delay1(500);
}

//I2C����ֹͣ
void I2CStop(void)
{
	SCL_Output(0); delay1(500);
	SDA_Output(0); delay1(500);
	SCL_Output(1); delay1(500);
	SDA_Output(1); delay1(500);

}

//�ȴ�Ӧ��
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

//����Ӧ��λ
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

//ͨ��I2C���߷���һ���ֽ�����
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

//��I2C���߽���һ���ֽ�����
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

//I2C���߳�ʼ��
void i2c_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SDA_Pin | SCL_Pin;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 // **

  	GPIO_Init(I2C_PORT, &GPIO_InitStructure);

}
