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

/* Extern typedef ------------------------------------------------------------*/
extern _KEY_DATA_ENUM key_short;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define KEY_B1_READ	(GPIOA->IDR&GPIO_Pin_0)
#define KEY_B2_READ	(GPIOA->IDR&GPIO_Pin_8)
#define KEY_B3_READ	(GPIOB->IDR&GPIO_Pin_1)
#define KEY_B4_READ	(GPIOB->IDR&GPIO_Pin_2)

/* Private variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Init program.
  * @param  None
  * @retval None
  */
void KEY_Init(void)
{
	/* GPIOD GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		/* Configure  PD2 in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
}

void KEY_Scan(void)
{
	static _KEY_DATA_ENUM key_temp=KEY_OFF;
	static FlagStatus key_wait=RESET;
	static u8 key_clear=0;

	if( (KEY_B1_READ==RESET)||(KEY_B2_READ==RESET)||(KEY_B3_READ==RESET)||(KEY_B4_READ==RESET) )
	{
		++key_clear;

		if( key_clear >= 2 )
		{
			key_clear = 0;
			if( KEY_B1_READ==RESET ) {key_temp = KEY_B1;}
			else if( KEY_B2_READ==RESET ) {key_temp = KEY_B2;}
			else if( KEY_B3_READ==RESET ) {key_temp = KEY_B3;}
			else if( KEY_B4_READ==RESET ) {key_temp = KEY_B4;}
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


/***************************************END OF FILE******************************/
