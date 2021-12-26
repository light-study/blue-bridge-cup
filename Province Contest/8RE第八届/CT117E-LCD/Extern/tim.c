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
#include "main.h"

/* Extern variables ------------------------------------------------------------*/
u8 OC1_FREQ=50;//0-100
u8 OC2_FREQ=50;//0-100

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define TIM3_OC1_FREQ	1000
#define TIM3_OC2_FREQ	2000
#define OC1_CCR_ALL			(TIM3_Clock/TIM3_OC1_FREQ)//1000
#define OC2_CCR_ALL			(TIM3_Clock/TIM3_OC2_FREQ)//500



/* Private variables ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
//void TIM2_Init(void);
void TIM3_Init(void);

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Init program.
  * @param  None
  * @retval None
  */
void TIM_ALL_Init(void)
{
//	TIM2_Init();
	TIM3_Init();//OUT TIM3_PA6_CH1  TIM3_PA7_CH2
}


void TIM3_Init(void)//OUT TIM3_PA6_CH1  TIM3_PA7_CH2
{
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 65535;
		TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Prescaler;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	}{
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		/* Output Compare Toggle Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;//TIM_OutputState_Enable
		TIM_OCInitStructure.TIM_Pulse = (OC1_CCR_ALL/2);
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		
		/* Output Compare Toggle Mode configuration: Channel2 */
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;//TIM_OutputState_Enable
		TIM_OCInitStructure.TIM_Pulse = (OC2_CCR_ALL/2);
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	}
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
	
	/* TIM enable counter */
	TIM_Cmd(TIM3, ENABLE);
	/* TIM IT enable */
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);
}



/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	static u16 capture = 0;
	static FlagStatus OC1_flag=SET,OC2_flag=SET;
	static u16 OC1_CCR_L,OC1_CCR_H,OC2_CCR_L,OC2_CCR_H;	
	
	/* TIM3_CH1 toggling with frequency = 183.1 Hz */
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
		capture = TIM_GetCapture1(TIM3);
		
		if( OC1_flag != RESET )
		{
			OC1_CCR_L = (100-OC1_FREQ)*OC1_CCR_ALL/100.0;
			TIM_SetCompare1(TIM3, capture + OC1_CCR_L );//==SET
			OC1_flag = RESET;
		}
		else
		{
			OC1_CCR_H = OC1_FREQ*OC1_CCR_ALL/100.0;
			TIM_SetCompare1(TIM3, capture + OC1_CCR_H );//==RESET
			OC1_flag = SET;
		}
	}
	
	/* TIM3_CH2 toggling with frequency = 366.2 Hz */
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM3);
		
		if( OC2_flag != RESET )
		{
			OC2_CCR_L = (100-OC2_FREQ)*OC2_CCR_ALL/100.0;
			TIM_SetCompare2(TIM3, capture + OC2_CCR_L );//==SET
			OC2_flag = RESET;
		}
		else
		{
			OC2_CCR_H = OC2_FREQ*OC2_CCR_ALL/100.0;
			TIM_SetCompare2(TIM3, capture + OC2_CCR_H );//==RESET
			OC2_flag = SET;
		}
	}


}


/***************************************END OF FILE******************************/
