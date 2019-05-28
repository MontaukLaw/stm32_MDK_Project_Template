#include "door.h"


void Door_Init(void){

    GPIO_InitTypeDef  GPIO_InitStructure;
 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 	
    //PB9 Door1

    //PB8 Door2    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);       
    
    //PB7 Door3    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);
 
    //PB6 Door4    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);   

    //PB5 Door_Sensor    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 
    
}



