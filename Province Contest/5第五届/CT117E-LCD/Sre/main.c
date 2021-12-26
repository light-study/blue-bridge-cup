/**
  ******************************************************************************
  * @file     
  * @author  
  * @version 
  * @date    2019.3.16
  * @brief   第五届
  ******************************************************************************
  *		13.00 
  *		
  *		RTC 官方历程最后两条配置的是什么	侵入检测输出时钟
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
//#define CH1_OUT_EN	TIM_CCxCmd( TIM2, TIM_Channel_2, ENABLE );	
//#define CH2_OUT_EN	TIM_CCxCmd( TIM2, TIM_Channel_3, ENABLE );
//#define CH1_OUT_DIS	TIM_CCxCmd( TIM2, TIM_Channel_2, DISABLE );GPIOA->BRR=GPIO_Pin_1;
//#define CH2_OUT_DIS	TIM_CCxCmd( TIM2, TIM_Channel_3, DISABLE );GPIOA->BRR=GPIO_Pin_2;
//#define CH1_OUT_EN	TIM_CCxCmd( TIM2, TIM_Channel_2, ENABLE );	
//#define CH2_OUT_EN	TIM_CCxCmd( TIM2, TIM_Channel_3, ENABLE );


/* Extern typedef ------------------------------------------------------------*/
_KEY_ENUM key_short=KEY_OFF;
_TIME_STRUCT time={23,59,50};
_USART_SET_STRUCT usart_set;

_USART_RX usart2={0};

u8 DutyCycle_1=30;//0-100
u8 DutyCycle_2=60;//0-100

FlagStatus ch1_out_flag=RESET;
FlagStatus ch2_out_flag=RESET;

/* Private typedef -----------------------------------------------------------*/
u32 TimingDelay = 0;
u8 LCD_txt[40]={0};

_CHANNEL_ENUM now_ch=CH_OFF;



FlagStatus key_scan_flag=RESET;
FlagStatus lcd_scan_flag=RESET;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/
void Delay_10Ms(u32 nTime);
void NVIC_Configuration(void);
void LCD_Display(void);
void KEY_Control(void);
void CMD_Check(void);


/* Private function prototypes -----------------------------------------------*/
//Main Body
int main(void)
{
	NVIC_Configuration();
	SysTick_Config(SystemCoreClock/100);//10ms
	
	LED_Init();
	KEY_Init();
	
	
	
	
	
	USART_ALL_Init();
	TIM_ALL_Init();	
	i2c_init();
	RTC_Init();
	
	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);



	AT24_Read(DutyCycle_ADDRESS, &DutyCycle_1, &DutyCycle_2);
	DutyCycle_1 = ((DutyCycle_1%10) != 0)?(DutyCycle_1-DutyCycle_1%10):DutyCycle_1; 
	DutyCycle_2 = ((DutyCycle_2%10) != 0)?(DutyCycle_2-DutyCycle_2%10):DutyCycle_2;
	
	
	sprintf((char*)usart2.buff,"               None");

	while(1)
	{
	 	if( key_scan_flag != RESET )
		{
			key_scan_flag = RESET;
			KEY_Scan();
			
			KEY_Control();
			LED_Set( now_ch );	
		}
		
		if( lcd_scan_flag != RESET )
		{
			lcd_scan_flag = RESET;
			LCD_Display();
		}

		CMD_Check();
	

		
		




	}
}

void LCD_Display(void)
{
	sprintf((char*)LCD_txt,"  PWM-PA1:%3d%%      ",DutyCycle_1);
	LCD_DisplayStringLine(Line1 , LCD_txt);
	sprintf((char*)LCD_txt,"  PWM-PA2:%3d%%      ",DutyCycle_2);
	LCD_DisplayStringLine(Line2 , LCD_txt);
	
	sprintf((char*)LCD_txt,"  Time:%02d:%02d:%02d      ", time.h,time.m,time.s);
	LCD_DisplayStringLine(Line4 , LCD_txt);
	
	switch(now_ch)
	{
		case CH_1:sprintf((char*)LCD_txt,"  Channel:PA1       "); break;
		case CH_2:sprintf((char*)LCD_txt,"  Channel:PA2       "); break;
		case CH_1_2:sprintf((char*)LCD_txt,"  Channel:PA1 PA2     "); break;
		case CH_OFF:sprintf((char*)LCD_txt,"  Channel:None     "); break;
	}
	LCD_DisplayStringLine(Line6 , LCD_txt);
	
	sprintf((char*)LCD_txt,"  Command          ");
	LCD_DisplayStringLine(Line7 , LCD_txt);
	
	sprintf((char*)LCD_txt,"%20s",usart2.buff);
	LCD_DisplayStringLine(Line8 , LCD_txt);

	sprintf((char*)LCD_txt,"%02d:%02d:%02d      ", usart_set.h,usart_set.m,usart_set.s);
	LCD_DisplayStringLine(Line9 , LCD_txt);			
}

void KEY_Control(void)
{
	if( key_short != KEY_OFF )
	{
	 	switch(key_short)
		{
		 	case KEY_B1:
				if( ch1_out_flag == RESET )
				{
					ch1_out_flag = SET;
					CH_OUT(CH_1, ENABLE);
					if(now_ch == CH_2) {now_ch=CH_1_2;}//置位CH1
					else {now_ch=CH_1;}
				}
				else 
				{ 
					ch1_out_flag = RESET;
					CH_OUT(CH_1, DISABLE);
					if(now_ch == CH_1_2) {now_ch=CH_2;}//复位CH1
					else {now_ch=CH_OFF;}
				} 
				break;

			case KEY_B2:
				DutyCycle_1 += 10;
				if( DutyCycle_1 > 100 ){DutyCycle_1=0;}
				AT24_Write(DutyCycle_ADDRESS, DutyCycle_1, DutyCycle_2);
				if( ch1_out_flag != RESET ) {CH_OUT(CH_1, ENABLE);}
				break;

			case KEY_B3:
				if( ch2_out_flag == RESET )
				{
					ch2_out_flag = SET;
					CH_OUT(CH_2, ENABLE);
					if(now_ch == CH_1) {now_ch=CH_1_2;}//置位CH2
					else {now_ch=CH_2;}
				}
				else 
				{
					ch2_out_flag = RESET;
					CH_OUT(CH_2, DISABLE);
					if(now_ch == CH_1_2) {now_ch=CH_1;}//复位CH2
					else {now_ch=CH_OFF;}
				}
				break;

			case KEY_B4:
				DutyCycle_2 += 10;
				if( DutyCycle_2 > 100 ) {DutyCycle_2=0;}
				AT24_Write(DutyCycle_ADDRESS, DutyCycle_1, DutyCycle_2);
				if( ch2_out_flag != RESET ) {CH_OUT(CH_2, ENABLE);}
				break;

			default:break;
		}

		key_short = KEY_OFF;
	}			
}

void CMD_Check(void)
{
	if( usart2.flag != RESET )
	{
		usart2.flag = RESET;
	
		if( (usart2.buff[2]==':')&&(usart2.buff[5]==':')&&(usart2.buff[8]=='-')&&
			(usart2.buff[9]=='P')&&(usart2.buff[10]=='A')&&(usart2.buff[12]=='-')&&(usart2.buff[14]=='S') )
		{
			sscanf( (char *)usart2.buff, "%2d:%2d:%2d-PA%1d-%2dS", 
			&usart_set.h,&usart_set.m,&usart_set.s,&usart_set.ch,&usart_set.con_s	);
			
			sprintf((char*)usart2.buff,"PA%1d=%2d:%2d:%2d-%2dS",
			usart_set.ch,usart_set.h,usart_set.m,usart_set.s,usart_set.con_s);
		}
		else
		{
//			sprintf((char*)usart2.buff,"CMD_ERROR%s",usart2.buff);	
		}
	
	}			
}

void CH_OUT(_CHANNEL_ENUM ch, FunctionalState NewState)
{
	if( ch == CH_1 )
	{
		if( NewState != DISABLE )
		{
		 	TIM_SetCompare2( TIM2, DutyCycle_1*1000/100 );
		}
		else
		{
			TIM_SetCompare2( TIM2, 0 );
		}
	}
	else if( ch == CH_2 )
	{
		if( NewState != DISABLE )// ENABLE
		{
//		 	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//			USART_ITConfig(USART2, USART_IT_IDLE, DISABLE);
			USART_Cmd(USART2,DISABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
			
			TIM_SetCompare3( TIM2, DutyCycle_2*1000/100 );	
		}
		else//DISABLE
		{
			TIM_SetCompare3( TIM2, 0 );

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//			USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
			USART_Cmd(USART2,ENABLE);	
		}
	 


	}	
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	static u8 num_20ms=1,num_200ms=2;

	if(TimingDelay>0) {TimingDelay--;}

	if( ++num_20ms >= 2 )
	{
		num_20ms = 0;
		key_scan_flag = SET;
	}

	if( ++num_200ms >= 20 )
	{
		num_200ms = 0;
		lcd_scan_flag = SET;
	}
}




/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}



//
void Delay_10Ms(u32 nTime)//10ms
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif


/****************************************END OF FILE**************************************/


