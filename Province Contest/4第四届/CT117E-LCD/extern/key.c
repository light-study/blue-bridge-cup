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
#include "key.h"
#include "main.h"



/* Extern typedef ------------------------------------------------------------*/
_KEY_DATA key_short=KEY_OFF;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define KEY_READ_B1	(GPIOA->IDR&GPIO_Pin_0)
#define KEY_READ_B2	(GPIOA->IDR&GPIO_Pin_8)
#define KEY_READ_B3	(GPIOB->IDR&GPIO_Pin_1)
#define KEY_READ_B4	(GPIOB->IDR&GPIO_Pin_2)

#define KEY_CLEAR_VAL	2	//Ïû¶¶Ê±¼ä

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
	/* GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

	{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
}

void KEY_Scan(void)
{
	static u8 key_clear=0;
	static _KEY_DATA key_temp=KEY_OFF;
	static FlagStatus key_wait=RESET;

	if( (KEY_READ_B1==RESET)||(KEY_READ_B2==RESET)||(KEY_READ_B3==RESET)||(KEY_READ_B4==RESET) )
	{
		++key_clear;

		if( key_clear >= KEY_CLEAR_VAL )
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






/*****************************************END OF FILE****************************************/
