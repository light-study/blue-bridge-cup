/*
  程序说明: 
  软件环境: Keil uVision 4.10 
  硬件环境: CT117E嵌入式竞赛板
  日    期: 
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "tim.h" 
#include "at24c02.h"
#include "rtc.h"
#include "usart.h"

/* Exported constants --------------------------------------------------------*/
#define USART_RX_BUFF_MAX_SIZE	128


//#define CH1_OUT_EN	TIM_SetCompare2( TIM2, DutyCycle_1*1000/100 );	
//#define CH2_OUT_EN	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);\
//USART_Cmd(USART2,DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);\
//TIM_SetCompare3( TIM2, DutyCycle_2*1000/100 );
//
//#define CH1_OUT_DIS	TIM_SetCompare2( TIM2, 0 );
//#define CH2_OUT_DIS	TIM_SetCompare3( TIM2, 0 );RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);\
//USART_Cmd(USART2,ENABLE);USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	u8 h;//hour
	u8 m;//minute
	u8 s;//second
	u32 s_add;	
}_TIME_STRUCT;

typedef struct
{
	int h;//hour
	int m;//minute
	int s;//second
	int con_s;//持续时间
	int ch;	
}_USART_SET_STRUCT;

typedef struct
{
	u8 buff[USART_RX_BUFF_MAX_SIZE];
	u8 size;
	FlagStatus flag;	
}_USART_RX;

typedef enum
{	
	CH_OFF=0x00,
	CH_1=0x01,
	CH_2=0x02,
	CH_1_2=0x03	
}_CHANNEL_ENUM;

typedef enum
{	
	KEY_OFF=0,
	KEY_B1=1,
	KEY_B2=2,
	KEY_B3=3,
	KEY_B4=4	
}_KEY_ENUM;



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void CH_OUT(_CHANNEL_ENUM ch, FunctionalState NewState);



#endif 

/***************************************END OF FILE**********************************/

