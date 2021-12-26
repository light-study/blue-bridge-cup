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
#include "led.h"

/* Variables -----------------------------------------------------------------*/
u8 LED_State=0x00;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	LED_Control(0x00);
}

void LED_Control( u8 state )// 0Ãð 1ÁÁ
{
	GPIOC->ODR = ~(state) << 8; 	
	GPIOD->BSRR = GPIO_Pin_2;/* Set  PD2 */
    GPIOD->BRR  = GPIO_Pin_2;/* Reset  PD2 */
} 





/**********************************************************END OF FILE***************************************/
