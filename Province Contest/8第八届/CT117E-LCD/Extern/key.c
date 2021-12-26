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
#include "key.h"
#include "main.h"

/* Variables -----------------------------------------------------------------*/
_KEY_data_enum Key_Short=KEY_OFF;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_B1_READ	(GPIOA->IDR & GPIO_Pin_0)			
#define KEY_B2_READ	(GPIOA->IDR & GPIO_Pin_8)
#define KEY_B3_READ	(GPIOB->IDR & GPIO_Pin_1)
#define KEY_B4_READ	(GPIOB->IDR & GPIO_Pin_2)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void KEY_Scan(void)
{
	static u8 key_clear=0;
	static _KEY_data_enum key_temp=KEY_OFF;
	static FlagStatus key_wait=RESET;

	if( (KEY_B1_READ == RESET)||(KEY_B2_READ == RESET)||(KEY_B3_READ == RESET)||(KEY_B4_READ == RESET) )
	{
		++key_clear;
		
		if( key_clear >= 2 )
		{
			if( KEY_B1_READ == RESET ) { key_temp = KEY_B1;}
			else if( KEY_B2_READ == RESET ) { key_temp = KEY_B2;}
			else if( KEY_B3_READ == RESET ) { key_temp = KEY_B3;}
			else if( KEY_B4_READ == RESET ) { key_temp = KEY_B4;}
			key_wait = SET;	
		}	
	}
	else
	{
		key_clear = 0;

		if( key_wait != RESET )
		{
			key_wait = RESET;
			
			Key_Short = key_temp;
			key_temp=KEY_OFF;
		}
	}

} 





/**********************************************************END OF FILE***************************************/
