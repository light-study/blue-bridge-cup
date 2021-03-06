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
#ifndef __MACHINERY_H
#define __MACHINERY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
#define	MACH_UP		(GPIOA->BSRR=GPIO_Pin_4);
#define	MACH_DOWN	(GPIOA->BRR =GPIO_Pin_4);
#define	DOOR_OPEN	(GPIOA->BSRR=GPIO_Pin_5);
#define	DOOR_CLOSE	(GPIOA->BRR =GPIO_Pin_5);

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Machinery_Init(void);


#endif 

/**********************************************************END OF FILE***************************************/
