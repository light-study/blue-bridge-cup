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
#include "rtc.h"
#include "main.h"

/* Extern Variables ----------------------------------------------------------*/
extern _time_struct	time;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void RTC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain */
	BKP_DeInit();
	
	/* Enable the LSI OSC */
	RCC_LSICmd(ENABLE);
	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(40000-1);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	RTC_SetCounter( time.s_add );
	RTC_WaitForLastTask();
	
//	/* To output second signal on Tamper pin, the tamper functionality
//	   must be disabled (by default this functionality is disabled) */
//	BKP_TamperPinCmd(DISABLE);
//	
//	/* Enable the RTC Second Output on Tamper Pin */
//	BKP_RTCOutputConfig(BKP_RTCOutputSource_Second);
}


/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear Interrupt pending bit */
		RTC_ClearITPendingBit(RTC_FLAG_SEC);

		time.s_add = RTC_GetCounter();
		time.h = time.s_add/3600%99;
		time.m = time.s_add/60%60;
		time.s = time.s_add%60;
	}
}



/**********************************************************END OF FILE***************************************/
