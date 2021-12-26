






#ifndef  __AT24C02_H
#define  __AT24C02_H



#include "stm32f10x.h"



#define DutyCycle_ADDRESS	0x50





void i2c_init(void);
void AT24_Write(u8 address, u8 data_1, u8 data_2);
void AT24_Read(u8 address, u8 *p_data_1, u8 *p_data_2);






#endif

