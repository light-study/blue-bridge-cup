
/**
  ******************************************************************************
  * @file     
  * @author  
  * @version 
  * @date		2019.3.13    
  * @brief		第六届模拟题   
  ******************************************************************************
  * @attention
  *				19.00	20.15	20.40 22.10	    165min 	待解决  TIM输出低电平(关闭输出即可输出低电平，输出高电平参考我的CSDN) 
  *				10.50	12.30					100min	35min
  ******************************************************************************
  */ 
/*
	什么东西不能用 |  	(看assert定义)
	频率测量范围		72M 2K-72M	1M	30-1M 
	可用TIM资源 		TIM 1\2\3\4
	TIM难道不用basic	可以不用

	待解决 捕获会跳0



	tip:
	TIM_ICInit 只能一次初始化一个CH
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Extern typedef ------------------------------------------------------------*/
_KEY_enum key_short;


extern uint32_t TIM2_IC2_Freq;
extern uint32_t TIM2_IC3_Freq;
extern uint16_t OC1_Freq;
extern uint16_t OC2_Freq;

extern _USART_STRUCT USART2_R;

extern u8 LCD_txt[40];
/* Private typedef -----------------------------------------------------------*/
u32 TimingDelay = 0;
u8 CH1_Multiple=0;
u8 CH2_Multiple=0;
u8 ch_cont=0;

FlagStatus ch1_in_flag=RESET;
FlagStatus ch2_in_flag=RESET;

FlagStatus usart_flag=RESET;

FlagStatus key_scan_flag=RESET;
FlagStatus lcd_display_flag = RESET;


/* Private define ------------------------------------------------------------*/
#define In_CH1_Freq		TIM2_IC2_Freq	
#define In_CH2_Freq		TIM2_IC3_Freq
#define Out_CH1_Freq	OC1_Freq
#define Out_CH2_Freq	OC2_Freq

#define OPEN_CH1_OUT	TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Enable);//CH1  OUT_PA6
#define CLOSE_CH1_OUT	TIM_CCxCmd(TIM3, TIM_Channel_1, TIM_CCx_Disable);//CH1  OUT_PA6
#define OPEN_CH2_OUT	TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Enable);//CH2  OUT_PA7
#define CLOSE_CH2_OUT	TIM_CCxCmd(TIM3, TIM_Channel_2, TIM_CCx_Disable);//CH2  OUT_PA7


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Delay_10Ms(u32 nTime);
void LCD_Display(void);
void KEY_Control(void);
void LED_Control(void);
void TIM_Control(void);
void USART_Check(void);


//Main Body
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Config(SystemCoreClock/100);//10ms

 	LED_Init();
	KEY_Init();
	i2c_init();
	USART2_Init();
	TIM_All_Init();


	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	
	AT24C02_Read(MULTIPLE_ADDRESS, &CH1_Multiple, &CH2_Multiple);
	

	

	while(1)
	{
	 	if( key_scan_flag!=RESET )
		{
			key_scan_flag = RESET;
			KEY_Scan();
			
			KEY_Control();
			LED_Control();
			TIM_Control();
		}

		if( lcd_display_flag!=RESET )
		{
			lcd_display_flag = RESET;
			LCD_Display();
		}
		
		USART_Check();
		
		 

	}
}

void LCD_Display(void)
{
	
	sprintf((char*)LCD_txt, "  Channel(1):%dHz        ",In_CH1_Freq);
	LCD_DisplayStringLine(Line3 , LCD_txt);
	sprintf((char*)LCD_txt, "  N(1):%1d            ",CH1_Multiple);
	LCD_DisplayStringLine(Line4 , LCD_txt);
	
	sprintf((char*)LCD_txt, "  Channel(2):%dHz        ",In_CH2_Freq);
	LCD_DisplayStringLine(Line6 , LCD_txt);
	sprintf((char*)LCD_txt, "  N(2):%1d            ",CH2_Multiple);
	LCD_DisplayStringLine(Line7 , LCD_txt);	

}

void KEY_Control(void)
{
	if( key_short != KEY_OFF )
	{
		if( usart_flag != SET )
		{
			switch(key_short)
			{
				case KEY_B1: 
					usart_flag=SET;
					key_short = KEY_OFF; 
					break;
				case KEY_B2: 
					if( ++ch_cont >= 2 ) {ch_cont = 0;}
					key_short = KEY_OFF;
					break;
				case KEY_B3:
					if( ch_cont == 0 ) { if(CH1_Multiple > 1) {--CH1_Multiple;} }
					else{ if(CH2_Multiple > 1) {--CH2_Multiple;} }
					AT24C02_Write(MULTIPLE_ADDRESS, CH1_Multiple, CH2_Multiple); 
					key_short = KEY_OFF;
					break;
				case KEY_B4: 
					if( ch_cont == 0 ) { if(CH1_Multiple < 10) {++CH1_Multiple;} }
					else{ if(CH2_Multiple < 10) {++CH2_Multiple;} }
					AT24C02_Write(MULTIPLE_ADDRESS, CH1_Multiple, CH2_Multiple);
					key_short = KEY_OFF;
					break;
				
				default: break;
			}
		}
		else //usart mode
		{
			if( key_short == KEY_B1 )
			{
				usart_flag=RESET;
				key_short = KEY_OFF;	
			}
			else
			{
				key_short = KEY_OFF;	
			}
		}
	}
}

void LED_Control(void)
{
	if( ch1_in_flag != RESET ) { LED_Set( LED_State |= LED1 ); }
	else { LED_Set( LED_State &= ~LED1 ); }

	if( ch2_in_flag != RESET ) { LED_Set( LED_State |= LED2 ); }
	else { LED_Set( LED_State &= ~LED2 ); }

	if( usart_flag != RESET ) { LED_Set( LED_State |= LED3 ); }
	else { LED_Set( LED_State &= ~LED3 ); }	
}


void TIM_Control(void)
{
	if( (In_CH1_Freq >= 50)&&(In_CH1_Freq <= 50000) )
	{
		ch1_in_flag = SET;		

		OPEN_CH1_OUT;
		Out_CH1_Freq = In_CH1_Freq*CH1_Multiple;

	}
	else
	{
		ch1_in_flag = RESET;
		CLOSE_CH1_OUT;

	}
	
	if( (In_CH2_Freq >= 50)&&(In_CH2_Freq <= 50000) )
	{
		ch2_in_flag = SET;
		OPEN_CH2_OUT;
		Out_CH2_Freq = In_CH2_Freq*CH2_Multiple;
	}
	else
	{
		ch2_in_flag = RESET;
		CLOSE_CH2_OUT;
	}	
}

void USART_Check(void)
{
	static int temp_ch=0,temp_Multiple=0;
	
	if( usart_flag == SET )
	{
		if( USART2_R.flag != RESET  )
		{
			USART2_R.flag = RESET;
			

			sscanf((char*)USART2_R.buff, "SET:%d:%d", &temp_ch, &temp_Multiple);
			if( temp_ch == 1) {CH1_Multiple = (temp_Multiple<=10)&&(temp_Multiple>0)?temp_Multiple:CH1_Multiple;}
			else if( temp_ch == 2) {CH2_Multiple = (temp_Multiple<=10)&&(temp_Multiple>0)?temp_Multiple:CH2_Multiple;}

//			if( (USART2_R.buff[0]=='S')&&(USART2_R.buff[1]=='E')&&(USART2_R.buff[2]=='T')&&(USART2_R.buff[3]==':')&&(USART2_R.buff[5]==':') )
//			{
//				if(USART2_R.buff[4] == '1' )
//				{
//					temp_Multiple = (USART2_R.buff[6]-'0')*10+(USART2_R.buff[7]-'0');
//					CH1_Multiple = temp_Multiple<=10?temp_Multiple:CH1_Multiple;
//				}
//				else if(USART2_R.buff[4] == '2' )
//				{
//					temp_Multiple = (USART2_R.buff[6]-'0')*10+(USART2_R.buff[7]-'0');
//					CH2_Multiple = temp_Multiple<=10?temp_Multiple:CH2_Multiple;	
//				} 	
//			}
//			else
//			{
////				printf("Long Error\r\n");	
//			}	
		}
	}
	else
	{
		USART2_R.flag = RESET;	
	}	
}







//
void Delay_10Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)//10ms
{
	static u8 num_20ms=1,num_200ms=3;

	if( TimingDelay>0 ) {TimingDelay--;}

	if( ++num_20ms >= 2)
	{
		num_20ms = 0;
		key_scan_flag = SET;	
	}
	if( ++num_200ms >= 20)
	{
		num_200ms = 0;
		lcd_display_flag = SET;	
	}

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



/****************************************END OF FILE****************************************/




