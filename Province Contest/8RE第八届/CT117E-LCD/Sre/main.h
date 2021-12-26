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



/* Exported types ------------------------------------------------------------*/
typedef struct
{
	u8	h;
	u8	m;
	u8	s;
	u32	s_add;
}_TIME_DATA_STRUCT;


typedef enum
{
	KEY_OFF=0,
	KEY_B1,
	KEY_B2,
	KEY_B3,
	KEY_B4
}_KEY_DATA_ENUM;


typedef enum
{
	m_stop=0,
	m_up,
	m_down
}_MACH_STATE_ENUM;

typedef enum
{
	d_open=0,
	d_close
}_DOOR_STATE_ENUM;


/* Exported constants --------------------------------------------------------*/
#define TIM3_Clock		1000000		
#define TIM3_Prescaler	((uint16_t) (SystemCoreClock / TIM3_Clock) - 1)

#define FLOOR1	0x01
#define FLOOR2	0x02
#define FLOOR3	0x04
#define FLOOR4	0x08


#define DOOR_OPEN_FLAG	(GPIOA->BSRR=GPIO_Pin_5)
#define DOOR_CLOSE_FLAG	(GPIOA->BRR=GPIO_Pin_5)

#define MACH_UP_FLAG	(GPIOA->BSRR=GPIO_Pin_4)
#define MACH_DOWN_FLAG	(GPIOA->BRR=GPIO_Pin_4)

#define MACH_UP			MACH_UP_FLAG;OC1_FREQ=80;TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Enable);mach_state = m_up;
#define MACH_DOWN		MACH_DOWN_FLAG;OC1_FREQ=60;TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Enable);mach_state = m_down;
#define MACH_DISABLE	TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Disable);mach_state = m_stop;

#define DOOR_OPEN		DOOR_OPEN_FLAG;OC2_FREQ=60;TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Enable);
#define DOOR_CLOSE		DOOR_CLOSE_FLAG;OC2_FREQ=50;TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Enable);
#define DOOR_DISABLE	TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Disable);
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */



#endif 

/***************************************END OF FILE******************************/

