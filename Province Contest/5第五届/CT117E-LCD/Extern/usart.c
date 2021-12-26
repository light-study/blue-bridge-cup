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
#include "usart.h"
#include "main.h"

/* Extern typedef ------------------------------------------------------------*/
extern _USART_RX usart2;
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PRINTF_COM	USART2
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void USART2_Init(void);

/**
  * @brief  Init program.
  * @param  None
  * @retval None
  */
void USART_ALL_Init(void)
{
	USART2_Init();	
	
}

void USART2_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		/* Configure USARTy Rx as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//RX PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	 	/* Configure USARTy Tx as alternate function push-pull */
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}{
		USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx/* | USART_Mode_Tx*/;
		/* Configure USART2 */
		USART_Init(USART2, &USART_InitStructure);
	}
	
	/* Enable the USART Receive Interrupt */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);
}



/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	static u8 size_temp=0;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		
		usart2.buff[size_temp++] = USART_ReceiveData(USART2);	
	}
	else if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		USART2->DR;//ClearITPendingBit
		
		usart2.size = size_temp;
		usart2.buff[size_temp]='\0';
		size_temp = 0;
		usart2.flag = SET;
	}
}



//
//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */
//
///**
//  * @brief  Retargets the C library printf function to the USART.
//  * @param  None
//  * @retval None
//  */
//PUTCHAR_PROTOTYPE
//{
//	/* Place your implementation of fputc here */
//	/* e.g. write a character to the USART */
//	USART_SendData(PRINTF_COM, (uint8_t) ch);
//	
//	/* Loop until the end of transmission */
//	while (USART_GetFlagStatus(PRINTF_COM, USART_FLAG_TC) == RESET)
//	{}
//	
//	return ch;
//}

/****************************************END OF FILE**************************************/
