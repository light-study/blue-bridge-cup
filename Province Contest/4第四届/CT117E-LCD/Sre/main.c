/**
  ******************************************************************************
  * @file     
  * @author  
  * @version 	1.0.0
  * @date    	2019.3.16
  * @brief   	“里程仪”设计任务书
  ******************************************************************************
  * @attention
  *				2019.3.17	9.00	11.50
  * 		   				12.30
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "eeprom.h"
#include "tim.h"


/* Extern typedef ------------------------------------------------------------*/

extern FlagStatus catch_flag;
extern _KEY_DATA key_short;
extern uint32_t TIM2Freq;
extern u16 TIM3_CCR1_Val;
/* Private typedef -----------------------------------------------------------*/
u8 LCD_txt[40]={0};

u32 TimingDelay = 0;

u32 run_time_10ms=0;

_CAR_SPEED_DATA car={0};
u16 freq_out_cont=1;//0-20Hz

_CAR_STATE_DATA car_run_flag=end;

FlagStatus led_flash=RESET;

FlagStatus key_scan_flag=RESET;
FlagStatus lcd_scan_flag=RESET;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

void Delay_10Ms(u32 nTime);
void NVIC_Configuration(void);
void LCD_Display(void);
void KEY_Control(void);


/* Private function prototypes -----------------------------------------------*/





/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	SysTick_Config(SystemCoreClock/100);//10Ms
	NVIC_Configuration();

	LED_Init();
	KEY_Init();
	EEPROM_Init();
	TIM_ALL_Init();



	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	



	EEPROM_Read(EEPROM_TOTAL_S, &car.total_s);

	while(1)
	{
		if( key_scan_flag != RESET )
		{
			key_scan_flag = RESET;

			KEY_Scan();
			KEY_Control();
		}
		
		if( lcd_scan_flag != RESET )
		{
			lcd_scan_flag = RESET;

			LCD_Display();
		}

		if( car_run_flag == stop )//开始停
		{
			car_run_flag = end;//设置为 已经停			
			
			car.total_s += car.total;
			run_time_10ms = 0;
			car.total = 0;
			car.speed = 0;
			car.average_speed = 0;
			car.time_h = 0;
			car.time_m = 0;
			car.time_s = 0;
			
			EEPROM_Write(EEPROM_TOTAL_S, car.total_s);
		}
		else if( car_run_flag == start )//开始启动
		{
			car_run_flag = run;//设置为 正在运行

		
		}
		else if( car_run_flag == run )//正在运行
		{
			if( led_flash != RESET )
			{
				led_flash = RESET;
				
				if( car.speed >= 90 ) {LED_Set( LED_State ^= LED1 );}
				else {LED_Set( LED_State &= ~LED1 );}
			}			
		}
		
		
			
	}
}





void LCD_Display(void)
{
	sprintf((char*)LCD_txt, "  Vim(Km/h):%3d      ",  car.speed);
	LCD_DisplayStringLine(Line1 ,LCD_txt);
	sprintf((char*)LCD_txt, "  Vavg(Km/h):%3d      ",  car.average_speed);
	LCD_DisplayStringLine(Line2 ,LCD_txt);

	sprintf((char*)LCD_txt, "  T(h:m:s):%2d:%2d:%2d      ",  car.time_h,car.time_m,car.time_s);
	LCD_DisplayStringLine(Line4 ,LCD_txt);
	sprintf((char*)LCD_txt, "  S(Km):%2.2lf",  car.total);
	LCD_DisplayStringLine(Line5 ,LCD_txt);

	sprintf((char*)LCD_txt, "  PA6 FreqOut:%3dHz    ",  freq_out_cont);
	LCD_DisplayStringLine(Line7 ,LCD_txt);

	sprintf((char*)LCD_txt, "%20d",  (u16)(car.total_s+car.total) );
	LCD_DisplayStringLine(Line9 ,LCD_txt);	
}

void KEY_Control(void)
{
	if( key_short != KEY_OFF )
	{
		if( key_short == KEY_B1 )
		{
			if( freq_out_cont < 20 ) {freq_out_cont++;}
			
			TIM3_CCR1_Val = (u16)TIM3_frequency*1.0/freq_out_cont/2;

			if( freq_out_cont == 1 ) {TIM_SetCounter(TIM3, 0);TIM_SetCompare1(TIM3, TIM3_CCR1_Val);}			
		}
		else if( key_short == KEY_B2 )
		{
			if( freq_out_cont > 0 ) {freq_out_cont--;}

			if( freq_out_cont == 0 ) { TIM3_CCR1_Val = 0xffff; }
			else {TIM3_CCR1_Val = (u16)TIM3_frequency*1.0/freq_out_cont/2;}	
		}
		
		key_short = KEY_OFF;
	}	
}



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	static u8 num_20ms=0,num_200ms=1,num_500ms=2,num_2s=3;
	
	
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

	if( ++num_500ms >= 50 )
	{
	 	num_500ms = 0;
		led_flash = SET;
	}

	if( ++num_2s >= 200 )
	{
	 	num_2s = 0;
		
		if( catch_flag == RESET )  //RESET
		{
			if( car_run_flag == run ) {car_run_flag = stop;}		
		}
		else					   //SET
		{
			if( car_run_flag == end ) {car_run_flag = start;}		
		}

		catch_flag = RESET;
	}

	if( (car_run_flag == run)||(car_run_flag == start) )
	{
		run_time_10ms++;
		car.speed = TIM2Freq*2.0*3.6;//Km/h
		car.total += TIM2Freq*2.0*0.01/1000.0;//Km
		car.average_speed = car.total*360000/run_time_10ms;

		car.time_add = run_time_10ms/100;
		car.time_h = car.time_add/3600%99;
		car.time_m = car.time_add/60%60;
		car.time_s = car.time_add%60;
	}

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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	
	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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
/*****************************************END OF FILE****************************************/
