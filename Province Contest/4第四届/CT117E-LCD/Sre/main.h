/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    	2019.3.17
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



/* Exported types ------------------------------------------------------------*/
typedef struct
{
	u8 speed;
	u8 average_speed;
	u32 time_add;
	u8 time_h;
	u8 time_m;
	u8 time_s;
	float total;
	u16 total_s;
}_CAR_SPEED_DATA;


typedef enum
{
	KEY_OFF=0,
	KEY_B1=1,
	KEY_B2=2,
	KEY_B3=3,
	KEY_B4=4,
}_KEY_DATA;


typedef enum
{
	start=0,
	run=1,
	stop=2,//刹车
	end=3,//停着不动
}_CAR_STATE_DATA;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif 

/*****************************************END OF FILE************************************************/
