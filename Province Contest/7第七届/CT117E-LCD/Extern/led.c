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



u8 LED_State=0;//extern

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
	/* GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		/* Configure mode */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}

	LED_Control(LED_State);
}


void LED_Control(u8 state)// 1ÁÁ   0Ãð
{
	GPIOC->ODR = (~state) << 8;
	GPIOD->BSRR = GPIO_Pin_2;//SET
	GPIOD->BRR = GPIO_Pin_2;//RESET
}


/************************************************END OF FILE********************************************/

