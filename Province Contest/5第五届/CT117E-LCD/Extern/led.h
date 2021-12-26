/*
  程序说明: 
  软件环境: Keil uVision 4.10 
  硬件环境: CT117E嵌入式竞赛板
  日    期: 
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
extern u8 LED_State;

/* Exported constants --------------------------------------------------------*/
#define LED1	(1<<0)
#define LED2	(1<<1)
#define LED3	(1<<2)
#define LED4	(1<<3)
#define LED5	(1<<4)
#define LED6	(1<<5)
#define LED7	(1<<6)
#define LED8	(1<<7)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void LED_Init(void);
void LED_Set(u8 state);


#endif 

/***************************************END OF FILE**********************************/

