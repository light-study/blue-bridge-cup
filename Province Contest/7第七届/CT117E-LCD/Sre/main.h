/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "led.h"
#include "adc.h"
#include "key.h"
#include "at24c02.h"
#include "usart.h"

/* Exported constants --------------------------------------------------------*/
#define USART2_RX_BUFF_SIZE		255


/* Exported types ------------------------------------------------------------*/
typedef struct
{
	u8 height;//高度
	u8 level;//等级
	u8 level_old;//old等级
	float vol;//adc电压
}_Liquid_Struct;

typedef struct
{
	u8 L;//阀值1
	u8 M;//阀值2
	u8 H;//阀值3
}_Threshold_Struct;

typedef struct
{
	u8 buff[USART2_RX_BUFF_SIZE];//buffer
	u16 size;//size
	FlagStatus flag;//
}_Usart_Rx_Struct;

typedef enum{
	Display=0,	//显示界面 
	Setting=1	//设置界面
} _menu_enum;


typedef enum {
	KEY_B1=1,
	KEY_B2=2,
	KEY_B3=3,
	KEY_B4=4,
	KEY_OFF=0,
} _key_enum;


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */



#endif

/************************************************END OF FILE********************************************/
