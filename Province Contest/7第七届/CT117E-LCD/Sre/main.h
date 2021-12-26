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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "led.h"
#include "adc.h"
#include "key.h"
#include "at24c02.h"
#include "usart.h"

/* Exported constants --------------------------------------------------------*/
#define USART2_RX_BUFF_SIZE		255


/* Exported types ------------------------------------------------------------*/
typedef struct
{
	u8 height;//�߶�
	u8 level;//�ȼ�
	u8 level_old;//old�ȼ�
	float vol;//adc��ѹ
}_Liquid_Struct;

typedef struct
{
	u8 L;//��ֵ1
	u8 M;//��ֵ2
	u8 H;//��ֵ3
}_Threshold_Struct;

typedef struct
{
	u8 buff[USART2_RX_BUFF_SIZE];//buffer
	u16 size;//size
	FlagStatus flag;//
}_Usart_Rx_Struct;

typedef enum{
	Display=0,	//��ʾ���� 
	Setting=1	//���ý���
} _menu_enum;


typedef enum {
	KEY_B1=1,
	KEY_B2=2,
	KEY_B3=3,
	KEY_B4=4,
	KEY_OFF=0,
} _key_enum;


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */



#endif

/************************************************END OF FILE********************************************/
