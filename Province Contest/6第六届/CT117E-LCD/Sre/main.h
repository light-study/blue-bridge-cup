
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
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
//#include <string.h>
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "tim.h"
#include "i2c.h"
#include "usart.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	u8 buff[128];
	u8 size;
	FlagStatus flag;
} _USART_STRUCT;


typedef enum{ 
KEY_B1 = 1,
KEY_B2 = 2,
KEY_B3 = 3,
KEY_B4 = 4,
KEY_OFF = 0
} _KEY_enum;


/* Exported constants --------------------------------------------------------*/



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif

/************************************END OF FILE*******************************************/
