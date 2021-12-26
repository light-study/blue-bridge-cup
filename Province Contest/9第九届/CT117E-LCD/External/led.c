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

/* Includes ------------------------------------------------------------------*/
#include "led.h"

/* External----------------------------------------------------------*/
u8 LED_State=0;//stm32f10x_it.c     main.c


/* ------------------------------------------------------------------*/

/**
  * @brief  
  * @param  
  * @retval 
  */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//LED_LE
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	LED_Set(0x00);
}


void LED_Set( u8 state )
{
    GPIOC->ODR = ~(state) << 8;

    GPIOD->BSRR = GPIO_Pin_2;/* Set */
    GPIOD->BRR  = GPIO_Pin_2;/* Reset */	
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
