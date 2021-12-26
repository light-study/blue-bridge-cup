

#ifndef  __EEPROM_H__
#define  __EEPROM_H__


#include "stm32f10x.h"




#define EEPROM_TOTAL_S	0x40
//#define EEPROM_U8_EN



void EEPROM_Init(void);
void EEPROM_Write(u8 add, u16 data_u16);
void EEPROM_Read(u8 add, u16 *data_u16);





#ifdef EEPROM_U8_EN
void EEPROM_Write_u8(u8 add, u8 data);
void EEPROM_Read_u8(u8 add, u8 *data);
#endif





#endif

