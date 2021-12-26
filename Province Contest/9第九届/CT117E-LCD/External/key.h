/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_H
#define __KEY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Exported constants --------------------------------------------------------*/
#define KEY_B1	0x01
#define KEY_B2	0x02
#define KEY_B3	0x03
#define KEY_B4	0x04
#define KEY_OFF	0x00

#define KEY_SCAN_B1		(GPIOA->IDR & GPIO_Pin_0)
#define KEY_SCAN_B2		(GPIOA->IDR & GPIO_Pin_8)
#define KEY_SCAN_B3		(GPIOB->IDR & GPIO_Pin_1)
#define KEY_SCAN_B4		(GPIOB->IDR & GPIO_Pin_2)


/* Exported functions ------------------------------------------------------- */
void KEY_Init(void);
void KEY_Scan(void);






#endif


