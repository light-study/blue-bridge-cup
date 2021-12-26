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
#include "tim.h"


/* Extern typedef ------------------------------------------------------------*/
extern u8 DutyCycle_1,DutyCycle_2;



/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
void TIM2_Init(void);

/* Private function prototypes -----------------------------------------------*/
void TIM_ALL_Init(void)
{
	TIM2_Init();	
	

}

void TIM2_Init(void)
{
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* GPIOA clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA/*|RCC_APB2Periph_AFIO*/, ENABLE);

	{
		GPIO_InitTypeDef GPIO_InitStructure;//-------TIM2  CH2  PA1  -------- TIM2 CH3 PA2    
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / 1000000) - 1;//1M
		TIM_TimeBaseStructure.TIM_Period = 1000;//									OUT	1KHz
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
	}{
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		/* PWM1 Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//   Disable  TIM_OutputState_Disable
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);//------TIM2  CH2  PA1

		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC3Init(TIM2, &TIM_OCInitStructure);//------TIM2 CH3 PA2		
	}
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
//	TIM_ARRPreloadConfig(TIM2, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}



/****************************************END OF FILE**************************************/
