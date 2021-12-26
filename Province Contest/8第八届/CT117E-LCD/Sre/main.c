/**
  ******************************************************************************
  * @file     
  * @author   	Jack
  * @version	  
  * @date     
  * @brief		第八届   
  ******************************************************************************
  * @attention	14.30	17.00	2.5
  *				19.30	22.00	2.5
  *				
  *				完全没有设计思路，下次还得再多做几次。
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
_LED_enum 		led_rotate_flag=STOP;//xxxxit.c      正反转
_time_struct	time={0,0,0, 12*3600+59*60+55};//rtc.c
FlagStatus KEY_Scan_flag=RESET;//xxxxit.c
FlagStatus Machinery_Run_flag=RESET;//xxxxit.c		电机运行指示
FlagStatus Machinery_Door_flag=RESET;//RESET = OPEN  SET = CLOSE
u8 key_over_num=0;//xxxxit.c      按键结束检测

u8 set_floor=0;///xxxxit.c

extern _KEY_data_enum Key_Short;
/* Private variables ---------------------------------------------------------*/
u32 TimingDelay = 0;
u8 LCD_Str[40];
_floor_data_enum now_floor=FLOOR_1;


/*
set_floor bit 
MSB		7	8	6	5	4	3	2	1	0	LSB
							8	4	2	1
Floor						4	3	2	1
*/										   
/* Private functions ---------------------------------------------------------*/
void LCD_Display(void);
void KEY_Control(void);
void Machinery_Control(void);
void Delay_key(u16 n);

//Main Body
int main(void)
{
	SysTick_Config(SystemCoreClock/10);//100ms
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	LED_Init();
	TIM3_Init();
	KEY_Init();
	RTC_Init();
	Machinery_Init();

	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);


	LCD_DisplayStringLine(Line2 ,"      Now Floor     ");
	while(1)
	{

		if( Machinery_Run_flag == RESET )//电机不在运行
		{
			KEY_Scan();
			Delay_key(60);
		}
		
		 
		KEY_Control();

		if( key_over_num == 1 )//按键结束
		{
//			key_over_num = 0;
			Machinery_Control();
		}

			
	}
}

void Machinery_Control(void)
{
	if( set_floor != 0 )
	{
		if( (set_floor >= now_floor)&&(led_rotate_flag!=REV) )//往上走
		{
			Machinery_Run_flag = SET;
			led_rotate_flag = ROS;
			
			if( (now_floor&set_floor) != 0 )//本层停	开门
			{
				Machinery_Door_flag = RESET;//OPEN
				DOOR_OPEN;
				MACH_OpenClose_EN;
				Delay_100Ms(20);
				MACH_OpenClose_DIS;
				

				Delay_100Ms(20);//停留两秒

				set_floor &= (~now_floor); 
			}

			if( set_floor != 0 )
			{
				if( Machinery_Door_flag == RESET )//if  OPEN
				{
					Machinery_Door_flag = SET;//CLOSE
	
					DOOR_CLOSE;
					MACH_OpenClose_EN;
					Delay_100Ms(20);
					MACH_OpenClose_DIS;
				}
			}
			
			if(set_floor >= now_floor) 
			{
				MACH_UP;
				MACH_UpDown_EN;
				Delay_100Ms(60);
				MACH_UpDown_DIS;
				now_floor <<= 1;
			}
//			else if( set_floor != 0 )
//			{
//				MACH_DOWN;
//				MACH_UpDown_EN;
//				Delay_100Ms(60);
//				MACH_UpDown_DIS;
//				now_floor >>= 1;	
//			}						
		}
		else if( set_floor != 0 )//往下走
		{
			Machinery_Run_flag = SET;
			led_rotate_flag = REV;
			
			if( (now_floor&set_floor) != 0 )//本层停	开门
			{
				Machinery_Door_flag = RESET;//OPEN
				DOOR_OPEN;
				MACH_OpenClose_EN;
				Delay_100Ms(20);
				MACH_OpenClose_DIS;

				Delay_100Ms(20);//停留两秒

				set_floor &= (~now_floor); 
			}

			if( set_floor != 0) 
			{
				if( Machinery_Door_flag == RESET )//if  OPEN
				{
					Machinery_Door_flag = SET;//CLOSE
					DOOR_CLOSE;
					MACH_OpenClose_EN;
					Delay_100Ms(20);
					MACH_OpenClose_DIS;
					
				}
				
				
				MACH_DOWN;
				MACH_UpDown_EN;
				Delay_100Ms(60);
				MACH_UpDown_DIS;
				now_floor >>= 1;
			}
		}
		
		if( set_floor == 0 )//运行完了
		{
			Machinery_Run_flag = RESET;
			led_rotate_flag = STOP;	
		}
	}	
}

void KEY_Control(void)
{
	switch( Key_Short )
	{
		case KEY_B1:
			set_floor |= 0x01;
			key_over_num=10+1;//100ms*10 = 1s
			Key_Short = KEY_OFF; 
			break;

		case KEY_B2:
			set_floor |= 0x02;
			key_over_num=10+1;
			Key_Short = KEY_OFF; 
			break;

		case KEY_B3:
			set_floor |= 0x04;
			key_over_num=10+1;
			Key_Short = KEY_OFF; 
			break;

		case KEY_B4:
			set_floor |= 0x08;
			key_over_num=10+1;
			Key_Short = KEY_OFF; 
			break;

		default: break;
	}			
}

void LCD_Display(void)
{
	u8 num_now_floor;
	switch(now_floor)
	{
		case 1: num_now_floor=1;break;
		case 2: num_now_floor=2;break; 
		case 4: num_now_floor=3;break; 
		case 8: num_now_floor=4;break;  
	}	

	if( Machinery_Door_flag == RESET )
		sprintf((char *)LCD_Str, "SetFloor:%1d,OPEN         ", set_floor );
	else
		sprintf((char *)LCD_Str, "SetFloor:%1d,CLOSE         ", set_floor );
	LCD_DisplayStringLine(Line3 , LCD_Str);

	sprintf((char *)LCD_Str, "       Floor:%1d         ", num_now_floor );
	LCD_DisplayStringLine(Line5 , LCD_Str);
	
	sprintf((char *)LCD_Str, "      %2d:%2d:%2d        ", time.h, time.m, time.s );
	LCD_DisplayStringLine(Line8 , LCD_Str);			
}




/* Private function prototypes -----------------------------------------------*/
void Delay_100Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}

void Delay_key(u16 n)
{
	u16 i,j;
	for (i = 0;i<n;++i)
		for(j=0;j<3000;++j);
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

