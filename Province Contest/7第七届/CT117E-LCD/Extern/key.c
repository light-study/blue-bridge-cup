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

/* Includes ------------------------------------------------------------------*/
#include "key.h"
#include "main.h"


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_B1_READ	(GPIOA->IDR&GPIO_Pin_0)
#define KEY_B2_READ	(GPIOA->IDR&GPIO_Pin_8)
#define KEY_B3_READ	(GPIOB->IDR&GPIO_Pin_1)
#define KEY_B4_READ	(GPIOB->IDR&GPIO_Pin_2)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init program.
  * @param  None
  * @retval None
  */
void KEY_Init(void)
{
	/* GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		/* Configure mode */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
}

extern _key_enum key_short;

void KEY_Scan(void)
{
	static u8 key_clear=0;
	static FlagStatus key_wait=RESET;
	static _key_enum key_temp=KEY_OFF;
	
	if( (KEY_B1_READ==RESET)||(KEY_B2_READ==RESET)||(KEY_B3_READ==RESET)||(KEY_B4_READ==RESET) )
	{
		++key_clear;
		
		if( key_clear >= 2 )
		{
			if( KEY_B1_READ==RESET ) {key_temp=KEY_B1;}
			else if( KEY_B2_READ==RESET ) {key_temp=KEY_B2;}
			else if( KEY_B3_READ==RESET ) {key_temp=KEY_B3;}
			else if( KEY_B4_READ==RESET ) {key_temp=KEY_B4;}
			key_wait = SET;	
		}		
	}
	else
	{
		key_clear = 0;

		if( key_wait != RESET )
		{
			key_wait = RESET;
			
			key_short = key_temp;
			key_temp = KEY_OFF; 			
		}
	}
}


/************************************************END OF FILE********************************************/
