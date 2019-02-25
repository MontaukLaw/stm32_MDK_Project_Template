/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
void setup(void);

void Delay(u32 count){
  u32 i=0;
	for(;i<count;i++);
}

void setup(void){
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	//初始化C13
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//初始化A5
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//初始化引脚状态
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	//GPIO_Init(GPIOE,&GPIO_InitStructure);
	//GPIO_SetBits(GPIOE,GPIO_Pin_5);
}

int main(void){
  setup();
	while(1){
	  GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		//GPIO_SetBits(GPIOE,GPIO_Pin_5);
		Delay(3000000);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		GPIO_SetBits(GPIOA,GPIO_Pin_5);		
		//GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		Delay(3000000);
	}
}
