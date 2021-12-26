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

/* Extern typedef ------------------------------------------------------------*/
u8 LED_State=0;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC, ENABLE);

  {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
  }

  LED_Set(LED_State);
}

void LED_Set( u8 state )
{
    GPIOC->ODR = (~state) << 8;
	/* Set PD0 and PD2 */
    GPIOD->BSRR = 0x00000005;
    /* Reset PD0 and PD2 */
    GPIOD->BRR  = 0x00000005;
}



/****************************************END OF FILE****************************************/


///**
//  ******************************************************************************
//  * @file     
//  * @author  
//  * @version 
//  * @date    
//  * @brief   
//  ******************************************************************************
//  * @attention
//  *
//  ******************************************************************************
//  */ 
//
///* Includes ------------------------------------------------------------------*/
//#include ".h"
///* Extern typedef ------------------------------------------------------------*/
///* Private typedef -----------------------------------------------------------*/
///* Private define ------------------------------------------------------------*/
///* Private macro -------------------------------------------------------------*/
///* Private variables ---------------------------------------------------------*/
//
//
///* Private function prototypes -----------------------------------------------*/
///* Private functions ---------------------------------------------------------*/
//
///**
//  * @brief  Main program.
//  * @param  None
//  * @retval None
//  */
//void _Init(void)
//{       
//  /* Periph clock enable */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO, ENABLE);
//  {
//	  GPIO_InitTypeDef GPIO_InitStructure;
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_;
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	  GPIO_Init(GPIO, &GPIO_InitStructure);
//  }
//
//}
//
//
///****************************************END OF FILE****************************************/
