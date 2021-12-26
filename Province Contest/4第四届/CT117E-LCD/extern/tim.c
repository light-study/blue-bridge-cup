/**
  ******************************************************************************
  * @file     
  * @author  
  * @version 
  * @date    2019.3.16
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
uint32_t TIM2Freq = 0;
u16 TIM3_CCR1_Val=200;//为总值的一半 50%占空比
FlagStatus catch_flag=RESET;
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
void TIM_ALL_Init(void)
{
	TIM2_Init();//IN 
	TIM3_Init();// TIM3 PA6 CH1    OUT
}




void TIM2_Init(void)// TIM2 PA1 CH2
{
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* GPIOA and GPIOB clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	
	{
		GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 20000-1;//10S  0.1Hz
		TIM_TimeBaseStructure.TIM_Prescaler = ((uint16_t) (SystemCoreClock / TIM2_frequency) - 1);//2Khz
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	}{
		TIM_ICInitTypeDef  TIM_ICInitStructure;

		TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0x0;
		TIM_ICInit(TIM2, &TIM_ICInitStructure);
	}
	/* TIM enable counter */
	TIM_Cmd(TIM2, ENABLE);
	/* Enable the CC2 Interrupt Request */
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void TIM3_Init(void)// TIM3 PA6 CH1	 		OUT
{
	/* GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 0xffff;
		TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (SystemCoreClock / TIM3_frequency) - 1;//2Khz
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	}{
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		/* Output Compare Toggle Mode configuration */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = TIM3_CCR1_Val;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);	
	}
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	
	TIM_ARRPreloadConfig(TIM3, DISABLE);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);

	/* TIM IT enable */
  	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);


//	TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter);
}





/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{ 
	static __IO uint16_t TIM2ReadValue1 = 0, TIM2ReadValue2 = 0;
	static __IO uint16_t CaptureNumber = 0;
	static __IO uint32_t Capture = 0;
	
//	static u8 time_out=100;//0-100 Update       100-200CC2	


	if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) 
	{
		/* Clear Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

		catch_flag = SET;
		
		if(CaptureNumber == 0)
		{
			/* Get the Input Capture value */
			TIM2ReadValue1 = TIM_GetCapture2(TIM2);
			CaptureNumber = 1;
		}
		else if(CaptureNumber == 1)
		{
			/* Get the Input Capture value */
			TIM2ReadValue2 = TIM_GetCapture2(TIM2); 
			
			/* Capture computation */
			if (TIM2ReadValue2 > TIM2ReadValue1)
			{
				Capture = (TIM2ReadValue2 - TIM2ReadValue1); 
			}
			else
			{
				Capture = ((0xFFFF - TIM2ReadValue1) + TIM2ReadValue2); 
			}
			/* Frequency computation */ 
			TIM2Freq = (uint32_t) TIM2_frequency / Capture;
			CaptureNumber = 0;
		}
	}
	
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) 
//	{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//
////		if( time_out>100 ) {time_out=100;}
////
////		if( --time_out <= 98 )//两次
////		{
////			time_out=100;
////			TIM2Freq = 0;
////		}
//
////		TIM2Freq = 0;
//	}
}


/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	static uint16_t capture = 0;

	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );

		capture = TIM_GetCapture1(TIM3);
		TIM_SetCompare1(TIM3, capture + TIM3_CCR1_Val );
	}
}






/*****************************************END OF FILE****************************************/
