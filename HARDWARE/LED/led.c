#include "led.h"
#define MAX_POS 12
#define MAX_NEG 12

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化

// N: PB0 PA7 PA6 PA5 PA4 PA3 PA2 PA1 PA0 PC15 PC14 PC13
// P: PA15 PB3 PA12 PA11 PA8 PB15 PB14 PB13 PB12 PB11 PB10 PB1

//新版本仅需控制13个LED, 且分别直接控制
//
void LED_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);	 

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		

    //working PG3    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		
    GPIO_Init(GPIOG, &GPIO_InitStructure);    
    
    //1 PE3    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    //2 PE5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    //3 PF1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		
    GPIO_Init(GPIOF, &GPIO_InitStructure);    

    //4 PF3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		
    GPIO_Init(GPIOF, &GPIO_InitStructure);      

    //5 PF5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		
    GPIO_Init(GPIOF, &GPIO_InitStructure); 
    
    //6 PF7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		
    GPIO_Init(GPIOF, &GPIO_InitStructure); 
    
    //7 PF9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	    		
    GPIO_Init(GPIOF, &GPIO_InitStructure); 
    
    //8 PC1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
    
    //9 PC3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		
    GPIO_Init(GPIOC, &GPIO_InitStructure);       

    //10 PA1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //11 PB6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	    		
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //12 PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //13 PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //14 PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void turnOn(u8 ledNumber){
    switch(ledNumber){
        case 1:
            LED1 = 1;   
        break;

        case 2:
            LED2 = 1;
        break;

        case 3:
            LED3 = 1;
        break;       
        
        case 4:
            LED4 = 1;
        break;
        
        case 5:
            LED5 = 1;
        break;
        
        case 6:
            LED6 = 1;
        break;
        
        case 7:
            LED7 = 1;
        break;
        
        case 8:
            LED8 = 1;
        break;
        
        case 9:
            LED9 = 1;
        break;
        
        case 10:
            LED10 = 1;
        break;
        
        case 11:
            LED11 = 1;
        break;
        
        case 12:
            LED12 = 1;
        break; 

        case 13:
            LED13 = 1;
        break;        

        case 14:
            LED14 = 1;
        break;        
    }

}

void turnOff(u8 ledNumber){
    switch(ledNumber){
        case 1:
            LED1 = 0;   
        break;

        case 2:
            LED2 = 0;
        break;

        case 3:
            LED3 = 0;
        break;       
        
        case 4:
            LED4 = 0;
        break;
        
        case 5:
            LED5 = 0;
        break;
        
        case 6:
            LED6 = 0;
        break;
        
        case 7:
            LED7 = 0;
        break;
        
        case 8:
            LED8 = 0;
        break;
        
        case 9:
            LED9 = 0;
        break;
        
        case 10:
            LED10 = 0;
        break;
        
        case 11:
            LED11 = 0;
        break;
        
        case 12:
            LED12 = 0;
        break; 

        case 13:
            LED13 = 0;
        break;        

        case 14:
            LED14 = 0;
        break;        
    }


}

void Switch_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
    GPIO_SetBits(GPIOA,GPIO_Pin_10);						 //PB.5 输出高

}


u8 ledBlinkFlag = 0;

void ledBlink(u8 ledNumber){
    
    if(ledBlinkFlag){
        turnOn(ledNumber); 
        
    }else{        
        turnOff(ledNumber);
       
    }
    
    ledBlinkFlag=!ledBlinkFlag;
}

void allOn(void){
    u8 i=0;
    for(i=0; i<13; i++){
        turnOn(i);
    }
}

void allOff(void){
    u8 i=0;
    for(i=0; i<14; i++){
        turnOff(i);
    }
}


