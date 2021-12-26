/**
  ******************************************************************************
  * @file    main.c 
  * @author  
  * @version 
  * @date    
  * @brief   Main program body.
  ******************************************************************************
  * @attention	µÚÆß½ì
  *				10.00	14.00	4Hour	
  * 
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
_key_enum key_short=KEY_OFF;//key.c

_Usart_Rx_Struct Us2_Rx={0};//usart.c
/* Private variables ---------------------------------------------------------*/
u32 TimingDelay = 0;
u8 LCD_Text[40];

FlagStatus key_scan_flag=RESET;
FlagStatus adc_read_flag=RESET;
FlagStatus lcd_display_flag=SET;

FlagStatus led_change_flag=RESET;
FlagStatus led_recive_flag=RESET;

u8 Threshold_index=1;//Threshold_index

_Threshold_Struct Threshold={30,50,70};

_Liquid_Struct liquid={0};
_menu_enum menu=Display;

/* Private functions ---------------------------------------------------------*/
void Delay_10Ms(u32 nTime);
void Menu_Display(void);
void Menu_Setting(void);
void Usart_Check(void);
void Get_Liquid( u8 times );
void IWDG_Init(void);


extern void AT24_Write(_Threshold_Struct th);
extern void AT24_Read(_Threshold_Struct *p_th);
/* Private function prototypes -----------------------------------------------*/







//Main Body
int main(void)
{
	SysTick_Config(SystemCoreClock/100);//10ms   100Hz
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	LED_Init();
	KEY_Init();
	ADC_ALL_Init();
	AT24_I2C_Init();
	Usart_Init();

	STM3210B_LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	AT24_Read( &Threshold );
	
	
	/* Check if the system has resumed from IWDG reset */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {LED_Control( LED_State |= LED8 );RCC_ClearFlag();}/* Clear reset flags */
	else {LED_Control( LED_State &= ~LED8 );}

	IWDG_Init();
	while(1)   
	{
		if( key_scan_flag != RESET )
		{
			key_scan_flag = RESET;
			KEY_Scan();
		}

		if( adc_read_flag != RESET )
		{
			adc_read_flag = RESET;
			Get_Liquid(3);
		}

		
		Usart_Check();

		
		if( lcd_display_flag != RESET)
		{
			lcd_display_flag = RESET;
			
			if( menu == Display )
			{
				Menu_Display();
			}
			else
			{
				Menu_Setting();
			}
		}


		/* Reload IWDG counter */
    	IWDG_ReloadCounter();
		
		
			
	}
}


void Menu_Display(void)
{
	LCD_DisplayStringLine(Line1,"     Liquid Level       ");

	sprintf((char *)LCD_Text, "   Height:%d          ", liquid.height);
	LCD_DisplayStringLine(Line4, LCD_Text);

	sprintf((char *)LCD_Text, "   ADC:%0.2f         ", liquid.vol);
	LCD_DisplayStringLine(Line6, LCD_Text);

	sprintf((char *)LCD_Text, "   Level:%d          ", liquid.level);
	LCD_DisplayStringLine(Line8, LCD_Text);


	if( key_short == KEY_B1 )
	{
		key_short = KEY_OFF;
		Threshold_index = 1;
		menu = Setting;
	} else if( key_short == KEY_B2 )
	{
		while(1);
	}
		
}

void Menu_Setting(void)
{
	LCD_DisplayStringLine(Line1,"   Parameter Setup     ");

	sprintf((char *)LCD_Text, "  Threshold 1:%3d     ", Threshold.L);
	if( Threshold_index==1 ) {LCD_SetTextColor(Red);LCD_DisplayStringLine(Line4, LCD_Text);LCD_SetTextColor(White);}
	else {LCD_DisplayStringLine(Line4, LCD_Text);} 

	sprintf((char *)LCD_Text, "  Threshold 2:%3d     ", Threshold.M);
	if( Threshold_index==2 ) {LCD_SetTextColor(Red);LCD_DisplayStringLine(Line6, LCD_Text);LCD_SetTextColor(White);}
	else {LCD_DisplayStringLine(Line6, LCD_Text);}

	sprintf((char *)LCD_Text, "  Threshold 3:%3d     ", Threshold.H);
	if( Threshold_index==3 ) {LCD_SetTextColor(Red);LCD_DisplayStringLine(Line8, LCD_Text);LCD_SetTextColor(White);}
	else {LCD_DisplayStringLine(Line8, LCD_Text);}
	
	switch(key_short)
	{
		case KEY_OFF:break;
	   	
		case KEY_B1:
			key_short = KEY_OFF;
			AT24_Write( Threshold );
			menu = Display;
			break;
		
		case KEY_B2:
			key_short = KEY_OFF;
			if( ++Threshold_index >= 4 ) {Threshold_index=1;};
			break;
		
		case KEY_B3:
			key_short = KEY_OFF;
			switch(Threshold_index)
			{		 
				case 1: if( (Threshold.L<Threshold.M-5)&&(Threshold.L<95) ) {Threshold.L+=5;} break;
				case 2: if( (Threshold.M<Threshold.H-5)&&(Threshold.M<95) ) {Threshold.M+=5;} break; 
				case 3: if( Threshold.H<95 ) {Threshold.H+=5;} break;  	
			}
			break;
		
		case KEY_B4:
			key_short = KEY_OFF;
			switch(Threshold_index)
			{		 
				case 1:if( Threshold.L>5 ) {Threshold.L-=5;}break;
				case 2:if( (Threshold.L+5<Threshold.M)&&(Threshold.M>5) ) {Threshold.M-=5;} break; 
				case 3:if( (Threshold.M+5<Threshold.H)&&(Threshold.H>5) ) {Threshold.H-=5;} break;  	
			}
			break;
	}	
}


void IWDG_Init(void)
{
	RCC_LSICmd(ENABLE);
	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	/* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
	dispersion) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* IWDG counter clock: LSI/32 */
	IWDG_SetPrescaler(IWDG_Prescaler_32);

	IWDG_SetReload(40000/320);//100ms
	
	/* Reload IWDG counter */
	IWDG_ReloadCounter();
	
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

void Get_Liquid( u8 times )
{
	u8 i;
	u32 temp=0;
	for(i=0; i<times; i++)
	{
		temp += ADC_GetVal();

		{u16 j;for(j=0;j<0xffff;++j);}		
	}
	
	liquid.vol = temp*1.0/times*3.3/4095;
	liquid.height = liquid.vol*100/3.3;
	
	if( liquid.height <=  Threshold.L ) {liquid.level = 0;}
	else if( liquid.height <=  Threshold.M ) {liquid.level = 1;}
	else if( liquid.height <=  Threshold.H ) {liquid.level = 2;}
	else {liquid.level = 3;}

	if( liquid.level != liquid.level_old )
	{
		led_change_flag = SET;
		if( liquid.level > liquid.level_old ) {printf("A:H%2d+L%1d+U\r\n", liquid.height,liquid.level);}
		else {printf("A:H%2d+L%1d+D\r\n", liquid.height,liquid.level);}
		
		liquid.level_old = liquid.level;	
	}
	 	
}


void Usart_Check(void)
{
	if( Us2_Rx.flag != RESET )
	{
		Us2_Rx.flag = RESET;
		
		led_recive_flag = SET;		

		if( Us2_Rx.size == 1 )
		{
			if( Us2_Rx.buff[0] == 'C' )
			{
				printf("C:H%2d+L%1d\r\n", liquid.height,liquid.level);	
			}
			else if( Us2_Rx.buff[0] == 'S' )
			{
				printf("S:TL%2d+TM%2d+TH%2d\r\n", Threshold.L,Threshold.M,Threshold.H);
			}
			else
			{
				printf("Check you command. You command unknow\r\n");	
			}
		}
		else
		{
			printf("Check you command. You command size is %d\r\n",Us2_Rx.size);
		}		
	}		
}




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
void SysTick_Handler(void)
{
	static u8 num_1s=1,num_200ms=0, led_change_times=0,led_recive_times=0;

	if( TimingDelay>0 ) {TimingDelay--;}

	if( key_scan_flag == RESET ) {key_scan_flag = SET;}

	if( ++num_1s >= 100 )
	{
		num_1s = 0;
		adc_read_flag = SET;
		LED_Control( LED_State ^= LED1 );
	}
	
	
	if( ++num_200ms >= 20 ) 
	{
		num_200ms = 0;			

		lcd_display_flag = SET;		

		if( led_change_flag!=RESET )
		{
		 	LED_Control( LED_State ^= LED2 );
			if( ++led_change_times >= 10)
			{
				led_change_times = 0;
				led_change_flag = RESET;		
			}
		}
		if( led_recive_flag!=RESET )
		{
		 	LED_Control( LED_State ^= LED3 );
			if( ++led_recive_times >= 10)
			{
				led_recive_times = 0;
				led_recive_flag = RESET;		
			}
		}
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

/************************************************END OF FILE********************************************/
