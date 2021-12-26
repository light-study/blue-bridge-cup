/**
  ******************************************************************************
  * @file 	   
  * @author  
  * @version 
  * @date    
  * @brief   µÚ¾Å½ì
  ******************************************************************************
  * @attention
  *	start 18.30 22.00 			3.5	Hour
  *	stare 10.00	11.00			1	Hour    4.5	Hour
  *	EEPFROM   ¿ÉÓÃAddress???
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x.h"
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "at24c02.h"
#include "tim.h"

/* ------------------------------------------------------------------*/
#define STANDBY 0x01
#define SETTING 0x02
#define RUNNING 0x03
#define PAUSE 	0x04

#define SYS_OUT_ON		TIM_SetCompare1(TIM3, 80);SYS_OUT_FLAG = SET;
#define SYS_OUT_OFF		TIM_SetCompare1(TIM3, 0);SYS_OUT_FLAG = RESET;LED_Set( LED_State &= ~LED1);

					
#define GOTO_RUNNING	menu = RUNNING;SYS_OUT_ON;time.s_add=time.h*60*60+time.m*60+time.s;
	
/* External----------------------------------------------------------*/
u32 TimingDelay = 0;//stm32f10x_it.c
FlagStatus KEY_SCAN_FLAG=RESET;//stm32f10x_it.c
FlagStatus SYS_OUT_FLAG=RESET;//stm32f10x_it.c
__IO FlagStatus TIME_REDUCE_FLAG=RESET;//stm32f10x_it.c

u8 LCD_text[40];

extern vu8 key_short_data;//key.c
extern vu8 key_long_data;//key.c

extern u8 LED_State;//led.c
/* ------------------------------------------------------------------*/
u8 menu = STANDBY;

time_struct time;
u8 time_index=0;
u8 setting_index=1;//1-3

/* Functions ------------------------------------------------------- */
void Delay_Ms(u32 nTime);
void LCD_MyDisplayStringLine(u8 Line,u8 index, u8 *ptr);
void IWDG_Init(void);
void LCD_Display(void);
void KEY_Control(void);



//Main Body
int main(void)
{
	SysTick_Config(SystemCoreClock/1000);//1ms
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	LED_Init();
	KEY_Init();
	TIM_ALL_Init();
	i2c_init();
	

	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);

	AT24C02_Read( time_index, &time );
	
	IWDG_Init();
	while(1)
	{
		if( KEY_SCAN_FLAG != RESET )
		{
			KEY_SCAN_FLAG = RESET;
			KEY_Scan();
		}
		
		IWDG_ReloadCounter();//Î¹¹·	
		
		LCD_Display();

		KEY_Control();
		
		if( menu == RUNNING )
		{
			if( TIME_REDUCE_FLAG != RESET )
			{
				TIME_REDUCE_FLAG = RESET;

				if( --time.s_add <= 0)
				{
					SYS_OUT_OFF;
					AT24C02_Read( time_index, &time );
					menu = STANDBY;	
				}
				time.h = time.s_add/3600;
				time.m = time.s_add/60%60;
				time.s = time.s_add%60;				
			}			
		}

	}//End Of while
}


void LCD_Display(void)
{
	switch( menu )
	{
/*----------------------------------------STANDBY--------------------------------------------*/
	 	case STANDBY:

			sprintf((char *)LCD_text, "  No.%1d     ", time_index+1);
			LCD_DisplayStringLine(Line1 , LCD_text);

			sprintf((char *)LCD_text, "  %2d:%2d:%2d     ", time.h, time.m, time.s );
			LCD_DisplayStringLine(Line5 , LCD_text);

			LCD_DisplayStringLine(Line9 ,"      Standby      ");

			break;
/*------------------------------------------SETTING------------------------------------------*/
		case SETTING:
			sprintf((char *)LCD_text, "  No.%1d     ", time_index+1);
			LCD_DisplayStringLine(Line1 , LCD_text);

			sprintf((char *)LCD_text, "  %2d:%2d:%2d     ", time.h, time.m, time.s );
			LCD_MyDisplayStringLine(Line5, setting_index, LCD_text);

			LCD_DisplayStringLine(Line9 ,"      Setting      ");
			
			break;
/*---------------------------------------------RUNNING--------------------------------------*/
		case RUNNING:
			sprintf((char *)LCD_text, "  No.%1d     ", time_index+1);
			LCD_DisplayStringLine(Line1 , LCD_text);

			sprintf((char *)LCD_text, "  %2d:%2d:%2d     ", time.h, time.m, time.s );
			LCD_DisplayStringLine(Line5, LCD_text);

			LCD_DisplayStringLine(Line9 ,"      Running      ");

			break;
/*-------------------------------------------PAUSE-----------------------------------------*/
		case PAUSE:
			sprintf((char *)LCD_text, "  No.%1d     ", time_index+1);
			LCD_DisplayStringLine(Line1 , LCD_text);

			sprintf((char *)LCD_text, "  %2d:%2d:%2d     ", time.h, time.m, time.s );
			LCD_DisplayStringLine(Line5, LCD_text);

			LCD_DisplayStringLine(Line9 ,"       Pause       ");

			break;

		default: 
			menu = STANDBY;
			
			break;
	}		
}




void KEY_Control(void)
{
	switch( key_short_data )
	{
		case KEY_OFF:break;
		case KEY_B1:
			switch( menu )
			{
				case STANDBY:
					if( ++time_index >= 5 ) {time_index=0;}
					AT24C02_Read( time_index, &time ); 
					break;

				default: break;	
			}
			break;

		case KEY_B2:
			switch( menu )
			{
				case STANDBY:
					menu = SETTING;
					setting_index = 1; 
					break;

				case SETTING:
					if( ++setting_index >= 4 ) {setting_index = 1;}
					break;

				default: break;	
			} 
			break;

		case KEY_B3:
			switch( menu )
			{
				case SETTING:
					if( setting_index == 1 ) { if(++time.h >= 100) {time.h=0;} }
						else if( setting_index == 2 ) { if(++time.m >= 60) {time.m=0;} }
						else if( setting_index == 3 ) { if(++time.s >= 60) {time.s=0;} } 
					break;

				default: break;	
			} 
			break;

		case KEY_B4:
			switch( menu )
			{
				case STANDBY:
					GOTO_RUNNING
					break;

				case SETTING:
					GOTO_RUNNING
					break;

				case RUNNING:
					menu = PAUSE;
					SYS_OUT_OFF;
					break;

				case PAUSE:
					GOTO_RUNNING
					break;

					default: break;	
			} 
			break;

		default: break;	
	}
	key_short_data = KEY_OFF;

	switch( key_long_data )
	{
		case KEY_B2:
			switch( menu )
			{
				case SETTING:
					AT24C02_Write( time_index, time );
					menu = STANDBY;
					break;
				default: break;	
			}
			break;

		case KEY_B3:
			switch( menu )
			{
				case SETTING:
					if( setting_index == 1 ) { if(++time.h >= 100) {time.h=0;} }
					else if( setting_index == 2 ) { if(++time.m >= 60) {time.m=0;} }
					else if( setting_index == 3 ) { if(++time.s >= 60) {time.s=0;} }
					break;
				default: break;			
			}
			break;

		case KEY_B4: 
			switch( menu )
			{
				case RUNNING:
					SYS_OUT_OFF;
					AT24C02_Read( time_index, &time );
					menu = STANDBY;
					break;
				case PAUSE:
					SYS_OUT_OFF;
					AT24C02_Read( time_index, &time );
					menu = STANDBY;
					break;
				default: break;	
			}
			break; 

		default: break; 
	}		
}

void IWDG_Init(void)
{
	uint32_t LsiFreq = 40000;
	/* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
	dispersion) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* IWDG counter clock: LSI/32 */
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	/* Set counter reload value to obtain 250ms IWDG TimeOut.
	 Counter Reload Value = 250ms/IWDG counter clock period
	                      = 250ms / (LSI/32)
	                      = 0.25s / (LsiFreq/32)
	                      = LsiFreq/(32 * 4)
	                      = LsiFreq/128
	*/
	IWDG_SetReload(LsiFreq/320);
	
	/* Reload IWDG counter */
	IWDG_ReloadCounter();
	
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}



void LCD_MyDisplayStringLine(u8 Line,u8 index, u8 *ptr)
{
	u32 i = 0;
	u16 refcolumn = 319;//319;

	if( index == 1 ) {index=2;}
	else if( index == 2 ) {index=5;}
	else if( index == 3 ) {index=8;} 

	while ((*ptr != 0) && (i < 20))	 //	20
	{
		if( (i==index)||(i==index+1) )
		{
			LCD_SetTextColor(Red);	
		}
		else
		{
			LCD_SetTextColor(White);
		}

		LCD_DisplayChar(Line, refcolumn, *ptr);
		refcolumn -= 16;
		ptr++;
		i++;
	}
}

//
void Delay_Ms(u32 nTime)
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

