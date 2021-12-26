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
#include "rtc.h"
#include "main.h"

/* Extern typedef ------------------------------------------------------------*/
extern _TIME_STRUCT time;
extern _USART_SET_STRUCT usart_set;
extern FlagStatus ch1_out_flag;
extern FlagStatus ch2_out_flag;
extern u8 DutyCycle_1;//0-100
extern u8 DutyCycle_2;//0-100
extern _CHANNEL_ENUM now_ch;
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init program.
  * @param  None
  * @retval None
  */
void RTC_Init(void)
{	
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

	
	RTC_SetCounter( time.h*60*60 + time.m*60 + time.s );
	RTC_WaitForLastTask();


	
//	/* To output second signal on Tamper pin, the tamper functionality
//	must be disabled (by default this functionality is disabled) */
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
	static u8 set_flag=0;
	u32 s_add_temp;

	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear Interrupt pending bit */
		RTC_ClearITPendingBit(RTC_FLAG_SEC);

		time.s_add = RTC_GetCounter();
		time.h = time.s_add/3600%24;
		time.m = time.s_add/60%60;
		time.s = time.s_add%60;
		
		if( (usart_set.h==time.h)&&(usart_set.m==time.m)&&(usart_set.s==time.s) )
		{
			if( set_flag == 0 )
			{
				if( usart_set.ch == 1 )
				{
					CH_OUT(CH_1, ENABLE);
					if(now_ch == CH_2) {now_ch=CH_1_2;}//置位CH1
					else {now_ch=CH_1;}
				}
				else if( usart_set.ch == 2 )
				{
					CH_OUT(CH_2, ENABLE);
					if(now_ch == CH_1) {now_ch=CH_1_2;}//置位CH2
					else {now_ch=CH_2;}	
				}
	
				s_add_temp = usart_set.h*60*60+usart_set.m*60+usart_set.s + usart_set.con_s;
				usart_set.h = s_add_temp/3600%24;
				usart_set.m = s_add_temp/60%60;
				usart_set.s = s_add_temp%60;
				set_flag = 1;
			}
			else if( set_flag == 1 )
			{
				if( usart_set.ch == 1 )
				{
					CH_OUT(CH_1, DISABLE);
					if(now_ch == CH_1_2) {now_ch=CH_2;}//复位CH1
					else {now_ch=CH_OFF;}
				}
				else if( usart_set.ch == 2 )
				{
					CH_OUT(CH_2, DISABLE);
					if(now_ch == CH_1_2) {now_ch=CH_1;}//复位CH2
					else {now_ch=CH_OFF;}	
				}
				set_flag = 0;
			}
		}	 

	}
}



/****************************************END OF FILE**************************************/
