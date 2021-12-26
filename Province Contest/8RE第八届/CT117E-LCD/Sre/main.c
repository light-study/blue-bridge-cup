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
/*
14.30

14.40 led main key 	15.06
		rtc display 15.24
		tim			15.51

*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "rtc.h"
#include "tim.h"
#include "mach.h"


/* Private define ------------------------------------------------------------*/
#define DEFAULT_TIME_H	23
#define DEFAULT_TIME_M	59
#define DEFAULT_TIME_S	50
/* Extern variables ----------------------------------------------------------*/
_KEY_DATA_ENUM 		key_short=KEY_OFF;
_TIME_DATA_STRUCT	time={DEFAULT_TIME_H,DEFAULT_TIME_M,DEFAULT_TIME_S,DEFAULT_TIME_H*3600+DEFAULT_TIME_M*60+DEFAULT_TIME_S}; 

extern u8 OC1_FREQ;//0-100
extern u8 OC2_FREQ;//0-100

/* Private variables ---------------------------------------------------------*/

u32 TimingDelay = 0;
u8 LCD_txt[40]={0};


FlagStatus key_scan_flag=SET;
FlagStatus lcd_display_flag=SET;
FlagStatus led_flash_flag=RESET;
FlagStatus mach_arrive_flag=RESET;

u8 lcd_flash_num=0;
 


u8 key_set_floor_delay=0;
u16 mach_run_delay=0;
u16 dooe_run_delay=0; 
_MACH_STATE_ENUM mach_state=m_stop;
_DOOR_STATE_ENUM door_state=d_open;

u8 set_floor=0;
u8 now_floor=1;//bit
/* Private macro -------------------------------------------------------------*/




/* Private functions ---------------------------------------------------------*/
void Delay_10Ms(u32 nTime);
void LCD_Display(void);
void NVIC_Configuration(void);
void KEY_Control(void);
void LED_Control(void);
void Mach_Control(void);
void Door_Control(void);

/* Private function prototypes -----------------------------------------------*/

//Main Body
int main(void)
{
	NVIC_Configuration();
	SysTick_Config(SystemCoreClock/100);//10ms

	LED_Init();
	RTC_Init();

	
	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);

	KEY_Init();
	TIM_ALL_Init();
	MACH_Init();
	
	while(1)
	{
		
		LED_Control();

		if( lcd_display_flag != RESET )
		{
			lcd_display_flag = RESET;
			LCD_Display();
		}

		if( key_set_floor_delay == 1 )//电机开始运行
		{
 			Mach_Control();	

		}
		else
		{
			if( key_scan_flag != RESET )
			{
				key_scan_flag = RESET;
				KEY_Scan();
	
				KEY_Control();
				
			}
		}




	}
}

void Mach_Control(void)
{

	if( (set_floor&now_floor) != 0 )//stop
	{

		if( door_state == d_close )
		{
			
			if(dooe_run_delay==0)
			{
				MACH_DISABLE;			//关掉电机
				DOOR_OPEN;				//开门
				dooe_run_delay=201;//2S
			}
			
			if( dooe_run_delay == 1 )
			{
				door_state = d_open;//开完了门
				dooe_run_delay = 0;
				DOOR_DISABLE;
			}
		}
		else if( door_state == d_open )
		{
		 
			if(mach_run_delay==0)
			{
//				MACH_UP;				//上升
				mach_run_delay=201;//2S	        进出人
			}

			if( mach_run_delay == 1 )
			{
				mach_run_delay = 0;
				set_floor ^= now_floor;
				mach_arrive_flag = SET;
				
				if( set_floor == 0 )
				{
					key_set_floor_delay = 0;	
				}	
			}
		}

	}
	else if( set_floor > now_floor )//up
	{
	
		if( door_state == d_open )
		{
			
			if(dooe_run_delay==0)
			{
				DOOR_CLOSE;
				dooe_run_delay=201;//2S
			}
			
			if( dooe_run_delay == 1 )//关门
			{
				door_state = d_close;
				dooe_run_delay = 0;
				DOOR_DISABLE;
			}
		}
		else if( door_state == d_close )
		{
		 
			if(mach_run_delay==0)
			{
				MACH_UP;				//上升
				mach_run_delay=601;//6S
			}

			if( mach_run_delay == 1 )
			{
				mach_run_delay = 0;
				now_floor <<= 1;
//				MACH_DISABLE;	
			}
		}	

	}
	else if( set_floor < now_floor )//down
	{
		

		if( door_state == d_open )
		{
			
			if(dooe_run_delay==0)
			{
				DOOR_CLOSE;
				dooe_run_delay=201;//2S
			}
			
			if( dooe_run_delay == 1 )//关门
			{
				door_state = d_close;
				dooe_run_delay = 0;
				DOOR_DISABLE;
			}
		}
		else if( door_state == d_close )
		{
		 
			if(mach_run_delay==0)
			{
				MACH_DOWN;				//下降
				
				mach_run_delay=601;//6S
			}

			if( mach_run_delay == 1 )
			{
				mach_run_delay = 0;
				now_floor >>= 1;
//				MACH_DISABLE;	
			}
		}
	}			
}

void Door_Control(void)
{
//	if( mach_state == stop )
//	{
//		DOOR_OPEN;	
//	}
//	else if( mach_state == up )
//	{
//		DOOR_CLOSE;
//		if( dooe_run_delay == 1 )
//		{
//		 	
//		}	
//	}
//	else if( mach_state == down )
//	{
//		DOOR_CLOSE;	
//	}	
}


void LCD_Display(void)
{
	if( mach_arrive_flag != RESET )
	{
		if( lcd_flash_num%2 == 1 )
		{
			LCD_DisplayStringLine(Line2 , "                 ");
		}
		else
		{
			sprintf((char*)LCD_txt,"    Now Floor:%2d      ", now_floor );
			LCD_DisplayStringLine(Line2 , LCD_txt);
		}
	}
	else
	{
		sprintf((char*)LCD_txt,"    Now Floor:%2d      ", now_floor );
		LCD_DisplayStringLine(Line2 , LCD_txt);
	}	

//	sprintf((char*)LCD_txt,"    Set Floor:%2d      ", set_floor );
//	LCD_DisplayStringLine(Line3 , LCD_txt);
	
//	if( (door_state == d_open)&&(mach_state == m_down) ) 			{sprintf((char*)LCD_txt,"  Door:Open  Mach:Down      ");}
//	else if( (door_state == d_open)&&(mach_state == m_up) ) 		{sprintf((char*)LCD_txt,"  Door:Open  Mach:Up        ");}
//	else if( (door_state == d_close)&&(mach_state == m_down) ) 		{sprintf((char*)LCD_txt,"  Door:Close Mach:Down      ");}
//	else if( (door_state == d_close)&&(mach_state == m_up) ) 		{sprintf((char*)LCD_txt,"  Door:Close Mach:Up        ");}
//	LCD_DisplayStringLine(Line5 , LCD_txt);


	sprintf((char*)LCD_txt,"      %2d:%2d:%2d       ", time.h, time.m, time.s );	
	LCD_DisplayStringLine(Line8 , LCD_txt);		
}

void LED_Control(void)
{
	static u8 led_temp=1;

	if( (set_floor&0x01) != 0 )//1
	{
		LED_Set( LED_State |= LED1 );
	}
	else
	{
	 	LED_Set( LED_State &= ~LED1 );
	}

	if( (set_floor&0x02) != 0 )//2
	{
		LED_Set( LED_State |= LED2 );
	}
	else
	{
	 	LED_Set( LED_State &= ~LED2 );
	}

	if( (set_floor&0x04) != 0 )//3
	{
		LED_Set( LED_State |= LED3 );
	}
	else
	{
	 	LED_Set( LED_State &= ~LED3 );
	}

	if( (set_floor&0x08) != 0 )//4
	{
		LED_Set( LED_State |= LED4 );
	}
	else
	{
	 	LED_Set( LED_State &= ~LED4 );
	}

	if( led_flash_flag != RESET )
	{
		led_flash_flag = RESET;
		if( mach_state == m_up )
		{
			LED_State &= 0x0f;
			LED_Set( LED_State |= (led_temp<<4) );
			led_temp <<= 1;
			if( led_temp == 0x10 ) {led_temp=1;}		
		}
		else if( mach_state == m_down )
		{
			LED_State &= 0x0f;
			LED_Set( LED_State |= (led_temp<<4) );
			led_temp >>= 1;
			if( led_temp == 0x00 ) {led_temp=0x08;}
		}
		else
		{
			LED_Set( LED_State &= 0x0f );
		}
	}
	
				
}


void KEY_Control(void)
{
	if( key_short != KEY_OFF )
	{	
		switch(key_short)
		{
			case KEY_B1:
				set_floor |= FLOOR1;
				break;

			case KEY_B2:
				set_floor |= FLOOR2;
				break;

			case KEY_B3:
				set_floor |= FLOOR3;
				break;

			case KEY_B4:
				set_floor |= FLOOR4;
				break;

			default:break;
		}

		key_set_floor_delay = 100+10;//1S+100ms
		key_short = KEY_OFF;
	}		
}





/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)//10ms
{
	static u8 num_20ms=0,num_200ms=1;
	
	if(TimingDelay>0) {TimingDelay--;}

	if( ++num_20ms >= 2 )
	{
		num_20ms = 0;
		key_scan_flag = SET;
	}

	if( ++num_200ms >= 20 )//
	{
		num_200ms = 0;
		lcd_display_flag = SET;

		led_flash_flag = SET;

		if(mach_arrive_flag != RESET)
		{
			lcd_flash_num++;
			if( lcd_flash_num >= 4 ) {lcd_flash_num=0;mach_arrive_flag=RESET;}
		}
	}

	if( key_set_floor_delay>1 ) {--key_set_floor_delay;}//减到1

	if( mach_run_delay>1 ) {--mach_run_delay;}//减到1
	
	if( dooe_run_delay>1 ) {--dooe_run_delay;}//减到1	

}







//
void Delay_10Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
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

	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	/* Enable the TIM5 Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
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

/***************************************END OF FILE******************************/

