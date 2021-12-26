

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
/* Extern typedef ------------------------------------------------------------*/
uint32_t TIM2_IC2_Freq = 0;
uint32_t TIM2_IC3_Freq = 0;

uint16_t OC1_Freq=1000;
uint16_t OC2_Freq=1000;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

void TIM2_Init(void);
void TIM3_Init(void);



/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  Init program.
  * @param  None
  * @retval None
  */
void TIM_All_Init(void)
{       
	TIM2_Init();
	TIM3_Init();
}

void TIM2_Init(void)//capture 			TIM2_CH2_PA1		TIM2_CH3_PA2
{
	/* Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	}{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;//1M
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = 0xffff;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	}{
		TIM_ICInitTypeDef  TIM_ICInitStructure;
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0x0;
		TIM_ICInit(TIM2, &TIM_ICInitStructure);

		TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
		TIM_ICInit(TIM2, &TIM_ICInitStructure);
	}
	
	/* TIM enable counter */
	TIM_Cmd(TIM2, ENABLE);
	
	/* Enable the CC2 Interrupt Request */
	TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC2|TIM_IT_CC3, ENABLE);
	
			
}



void TIM3_Init(void)//TIM3_CH1_PA6 TIM3_CH2_PA7
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	{	
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 65535;
		TIM_TimeBaseStructure.TIM_Prescaler = 1-1;//72M
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	}{
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		/* Output Compare Toggle Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
		TIM_OCInitStructure.TIM_Pulse = 72000000/2/OC1_Freq;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		/* Output Compare Toggle Mode configuration: Channel2 */
		TIM_OCInitStructure.TIM_Pulse = 72000000/2/OC2_Freq;
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	}

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);

	
	/* TIM enable counter */
	TIM_Cmd(TIM3, ENABLE);
	
	/* TIM IT enable */
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);	
}

void TIM2_IRQHandler(void)
{ 
	static vu16 IC2ReadValue1 = 0, IC2ReadValue2 = 0;
	static vu16 Capture2Number = 0;
	static vu16 Capture2 = 0;
	static vu16 IC3ReadValue1 = 0, IC3ReadValue2 = 0;
	static vu16 Capture3Number = 0;
	static vu16 Capture3 = 0;
	
	static int clear_cont=100;//0-100(发生捕获) 100(零值) 100-200(未发生捕获超时)
	
	if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) 
	{
		/* Clear TIM2 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		if( clear_cont > 0 ) {clear_cont--;}

		if(Capture2Number == 0)
		{
			/* Get the Input Capture value */
			IC2ReadValue1 = TIM_GetCapture2(TIM2);
			Capture2Number = 1;
		}
		else if(Capture2Number == 1)
		{
			/* Get the Input Capture value */
			IC2ReadValue2 = TIM_GetCapture2(TIM2); 
			
			/* Capture computation */
			if (IC2ReadValue2 > IC2ReadValue1)
			{
				Capture2 = (IC2ReadValue2 - IC2ReadValue1); 
			}
			else
			{
				Capture2 = ((0xFFFF - IC2ReadValue1) + IC2ReadValue2); 
			}
			/* Frequency computation */ 
			TIM2_IC2_Freq = (uint32_t) 1000000  / Capture2;
			Capture2Number = 0;
		}
	}
	else if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET) 
	{
		/* Clear TIM2 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		if( clear_cont > 0 ) {clear_cont--;}

		if(Capture3Number == 0)
		{
			/* Get the Input Capture value */
			IC3ReadValue1 = TIM_GetCapture3(TIM2);
			Capture3Number = 1;
		}
		else if(Capture3Number == 1)
		{
			/* Get the Input Capture value */
			IC3ReadValue2 = TIM_GetCapture3(TIM2); 
			
			/* Capture computation */
			if (IC3ReadValue2 > IC3ReadValue1)
			{
				Capture3 = (IC3ReadValue2 - IC3ReadValue1); 
			}
			else
			{
				Capture3 = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2); 
			}
			/* Frequency computation */ 
			TIM2_IC3_Freq = (uint32_t) 1000000  / Capture3;
			Capture3Number = 0;
		}
	}
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
		if( clear_cont < 100 )
		{
			clear_cont = 100;		
		}

		if( ++clear_cont >= 102 )//2次
		{
			clear_cont=100;
			TIM2_IC2_Freq = 0;
			TIM2_IC3_Freq = 0;
			
			
			IC2ReadValue1 = 0;
			IC2ReadValue2 = 0;
			Capture2Number = 0;
			Capture2 = 0;
			IC3ReadValue1 = 0;
			IC3ReadValue2 = 0;
			Capture3Number = 0;
			Capture3 = 0; 	
		} 
	}
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
		TIM_SetCompare1(TIM3, capture + 72000000/2/OC1_Freq );
	}
	
	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM3);
		TIM_SetCompare2(TIM3, capture + 72000000/2/OC2_Freq );
	}
}


/****************************************END OF FILE****************************************/
