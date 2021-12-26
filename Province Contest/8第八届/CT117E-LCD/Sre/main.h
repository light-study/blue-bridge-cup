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

/* Includes ------------------------------------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "led.h"
#include "tim.h"
#include "rtc.h"
#include "key.h"
#include "machinery.h"


/* Exported types ------------------------------------------------------------*/
typedef enum {
STOP = 0,//stop 
ROS = 1,//正 
REV = 2//反
} _LED_enum;

typedef enum {
KEY_B1 = 1, 
KEY_B2 = 2, 
KEY_B3 = 3, 
KEY_B4 = 4, 
KEY_OFF = 0
} _KEY_data_enum;

typedef enum {
FLOOR_1 	= 1, 
 
FLOOR_2 	= 2, 
 
FLOOR_3 	= 4,

FLOOR_4		= 8/*,
FLOOR_NULL	= 5*/
} _floor_data_enum;

typedef struct{
	u8 h;//hour
	u8 m;//minute
	u8 s;//second
	u32 s_add;// 秒数之和
} _time_struct;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Delay_100Ms(u32 nTime);



#endif 

/**********************************************************END OF FILE***************************************/
