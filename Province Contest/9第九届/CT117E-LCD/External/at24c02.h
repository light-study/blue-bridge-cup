




#ifndef  __I2C_H__
#define  __I2C_H__

#include "stm32f10x.h"


#define AT24C02_Write_Address	0xa0
#define AT24C02_Read_Address	0xa1




#define BASE_ADDRESS	0x00
#define SIZE_ADDRESS	0x10



typedef struct
{
	u8 h;//hour
	u8 m;//minute
	u8 s;//second
	u32 s_add;//second add
}time_struct;


void i2c_init(void);
void AT24C02_Write(u8 index, time_struct data);
void AT24C02_Read(u8 index, time_struct* data);


#endif

