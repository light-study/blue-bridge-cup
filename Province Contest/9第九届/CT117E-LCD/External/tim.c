/**
  ******************************************************************************
  * @file    
  * @author 
  * @version 
  * @date    
  * @brief   PA6 TIM3 CH1
  ******************************************************************************
  * @attention
  *

  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void TIM_ALL_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);/* TIM3 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);/* GPIOA  clock enable */

	{//TIM3
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
		TIM_TimeBaseStructure.TIM_Period = 100-1;
		TIM_TimeBaseStructure.TIM_Prescaler = 720-1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
		/* PWM1 Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;//0-99
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		
		TIM_ARRPreloadConfig(TIM3, ENABLE);
		
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE);

//		TIM_SetCompare1(TIM3, 80);
	}
}

