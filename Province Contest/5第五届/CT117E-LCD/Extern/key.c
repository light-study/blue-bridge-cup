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
extern _KEY_ENUM key_short;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define KEY_READ_B1	(GPIOA->IDR&GPIO_Pin_0)
#define KEY_READ_B2	(GPIOA->IDR&GPIO_Pin_8)
#define KEY_READ_B3	(GPIOB->IDR&GPIO_Pin_1)
#define KEY_READ_B4	(GPIOB->IDR&GPIO_Pin_2)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init program.
  * @param  None
  * @retval None
  */
void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

	{
		GPIO_InitTypeDef GPIO_InitStructure;
		/* Configure PD0 and PD2 in output pushpull mode */
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
	static u8 key_clear=0;
	static _KEY_ENUM key_temp=KEY_OFF;
	static FlagStatus key_wait=RESET;

	if( (KEY_READ_B1==RESET)||(KEY_READ_B2==RESET)||(KEY_READ_B3==RESET)||(KEY_READ_B4==RESET) )
	{
		++key_clear;

		if( key_clear >= 2 )
		{
			key_clear = 0;
			if( KEY_READ_B1==RESET ) {key_temp=KEY_B1;}
			else if( KEY_READ_B2==RESET ) {key_temp=KEY_B2;}
			else if( KEY_READ_B3==RESET ) {key_temp=KEY_B3;}
			else if( KEY_READ_B4==RESET ) {key_temp=KEY_B4;} 
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



/****************************************END OF FILE**************************************/
