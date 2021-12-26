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
#include "tim.h"

/* Private variables ---------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PrescalerValue	(uint16_t)(SystemCoreClock / 1000000) - 1; //1MHz
#define CCR1_H_Val		800		
#define CCR1_L_Val		200

#define CCR2_H_Val		300
#define CCR2_L_Val		200
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void TIM3_Init(void)	//PA6  TIM3_CH1    PA7   TIM3_CH2
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* Output Compare Toggle Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR1_H_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//≥ı º÷µ
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	/* Output Compare Toggle Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_Pulse = CCR2_H_Val;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);

	/* TIM enable counter */
	TIM_Cmd(TIM3, ENABLE);
	/* TIM IT enable */
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 , DISABLE);
}


/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	static uint16_t capture = 0;
	static u8 oc1_flag=1,oc2_flag=1;

	/* TIM3_CH1 toggling  */
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );

		capture = TIM_GetCapture1(TIM3);
		if( oc1_flag == 0 )
		{
			oc1_flag=1;
			TIM_SetCompare1(TIM3, capture + CCR1_H_Val );
		}
		else
		{
			oc1_flag=0;
			TIM_SetCompare1(TIM3, capture + CCR1_L_Val );	
		}
	}
	
	/* TIM3_CH2 toggling  */
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

		capture = TIM_GetCapture2(TIM3);
		if( oc2_flag == 0 )
		{
			oc2_flag=1;
			TIM_SetCompare2(TIM3, capture + CCR2_H_Val);
		}
		else
		{
			oc2_flag=0;
			TIM_SetCompare2(TIM3, capture + CCR2_L_Val );	
		}
		
	}
}


/**********************************************************END OF FILE***************************************/
