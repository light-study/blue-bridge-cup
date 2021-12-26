

#ifndef  __I2C_H__
#define  __I2C_H__



#include "stm32f10x.h"


#define MULTIPLE_ADDRESS	0x10



void i2c_init(void);
void AT24C02_Write(u8 address, u8 data_1, u8 data_2);
void AT24C02_Read(u8 address, u8 *p_data_1, u8 *p_data_2);




#endif

