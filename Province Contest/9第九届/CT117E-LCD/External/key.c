/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "key.h"

/* External----------------------------------------------------------*/
vu8 key_short_data;
vu8 key_long_data;

/* ------------------------------------------------------------------*/



/* ------------------------------------------------------------------*/

/**
  * @brief  
  * @param  
  * @retval 
  */
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
    static u8 key_temp=KEY_OFF,key_clear,key_wait=RESET;
	
	if(  (KEY_SCAN_B1==RESET)||(KEY_SCAN_B2==RESET)||(KEY_SCAN_B3==RESET)||(KEY_SCAN_B4==RESET) )
	{
		++key_clear;

		if( key_clear >= 40)// 20ms*40 = 800ms 长按
		{
			key_long_data = key_temp;
			key_clear = 2+1;
		}
		else if( key_clear == 2 )//短按
		{
			if( KEY_SCAN_B1==RESET ) { key_temp = KEY_B1; }
			else if( KEY_SCAN_B2==RESET ) { key_temp = KEY_B2; }
			else if( KEY_SCAN_B3==RESET ) { key_temp = KEY_B3; }
			else if( KEY_SCAN_B4==RESET ) { key_temp = KEY_B4; }
			key_wait = SET;
		}


	}
	else
	{
		if( key_wait != RESET )//检测到松手
		{
		 	key_wait = RESET;

			if( key_long_data != KEY_OFF ) {key_long_data = KEY_OFF;}
			else {key_short_data = key_temp;}		
		}
		key_clear = 0;
	}	
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
