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
#include "led.h"

/* Extern typedef ------------------------------------------------------------*/
u8 LED_State=0;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Init program.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
	/* GPIOD GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC, ENABLE);
	
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		/* Configure  PD2 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}	
	GPIOD->BRR = GPIO_Pin_2;//RESET
	LED_Set(LED_State);
}

void LED_Set(u8 state)
{
	GPIOC->ODR = (~state)<<8;
	GPIOD->BSRR = GPIO_Pin_2;//SET
	GPIOD->BRR = GPIO_Pin_2;//RESET	
}



/***************************************END OF FILE******************************/
