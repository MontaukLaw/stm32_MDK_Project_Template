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
#include "delay.h"
#include "usart.h"
#include "cmd.h"
#include "led.h"
#include "string.h"
#include "timer.h"
#include "tasks.h"
#include "door.h"

#include "test.h"

extern u8 txBuffer[];

void test(void){
    
    //if(DOOR_CLOSED){
        //nop();    
    //}else{
        //nop();
    //};
    
    //unitTest1();
}

void setup(void){
    delay_init();
    //Switch_Init();
	uart_init(9600);
    LED_Init();
    //Door_Init(); 

    //ledOn(25);    
    
    //��ʱ����ʼ��
  	TIM3_Int_Init(499,7199);//10Khz�ļ���Ƶ�ʣ�������500Ϊ50ms  
    
    Get_ChipID();
    
    test();
}


int main(void){
    
    setup();  
    
    unitTest2();
    
    while(1){
    
    }
    
    while(0){
        LED_WORKING = 0;
        delay_ms(999); 
        LED_WORKING = 1;
        delay_ms(999); 
        //openDoor(1);
        //delay_ms(999);                
        //delay_ms(999); 
    }


    

}


//��ʱ��3�жϷ������, Ҳ����һ��СOS
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
    {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		runTasks();
        //LED1=!LED1;
	}
}

