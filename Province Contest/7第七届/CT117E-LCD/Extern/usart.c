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

extern _Usart_Rx_Struct Us2_Rx;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define TxBufferSize1	USART2_RX_BUFF_SIZE

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init program.
  * @param  None
  * @retval None
  */
void Usart_Init(void)
{
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	{
		NVIC_InitTypeDef NVIC_InitStructure;

		/* Enable the USARTz Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		
		NVIC_Init(&NVIC_InitStructure);
	}{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* Configure USARTy Rx as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/* Configure USARTy Tx as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}{
		USART_InitTypeDef USART_InitStructure;

		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
		/* Configure USART */
		USART_Init(USART2, &USART_InitStructure);
	}{
		DMA_InitTypeDef DMA_InitStructure;
		
		/* USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config */
		DMA_DeInit(DMA1_Channel6);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(Us2_Rx.buff);
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	}
	
	/* Enable the USART2 Receive Interrupt */
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
	
	/* Enable USARTy DMA RX request */
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	
	
	
	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);
	
	/* Enable USARTy DMA RX Channel */
	DMA_Cmd(DMA1_Channel6, ENABLE);
	
}



/**
  * @brief  This function handles USART2  interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
//	static u16 size_temp=0;	

//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit( USART2, USART_IT_RXNE );
//		
//		Us2_Rx.buff[ size_temp++ ] = USART_ReceiveData(USART2);

//	}
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		USART2->DR;//ClearITPendingBit  USART_IT_IDLE 
		
//		DMA_Cmd(DMA1_Channel6, DISABLE);
		DMA1_Channel6->CCR &= (uint16_t)(~DMA_CCR1_EN);
		
		
//		Us2_Rx.size = TxBufferSize1 - DMA_GetCurrDataCounter(DMA1_Channel6);
		Us2_Rx.size = TxBufferSize1 - DMA1_Channel6->CNDTR;
		
//		Us2_Rx.size = size_temp;
		Us2_Rx.buff[ Us2_Rx.size ] = '\0';
		Us2_Rx.flag = SET;
//		size_temp = 0;	
		
		DMA1_Channel6->CNDTR = TxBufferSize1;
//		DMA_SetCurrDataCounter(DMA1_Channel6, TxBufferSize1);

//		DMA_Cmd(DMA1_Channel6, ENABLE);
		DMA1_Channel6->CCR |= DMA_CCR1_EN;
	}
}





#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/************************************************END OF FILE********************************************/


