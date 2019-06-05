#include "led.h"
#define MAX_POS 12
#define MAX_NEG 12
#define LED_NUM 28

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化

//新版本仅需控制28个LED, 且分别直接控制
void LED_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);	 

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		

    //working: PG3 16: PG1  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_1;	    		
    GPIO_Init(GPIOG, &GPIO_InitStructure);    
    
    //1: PE3 2: PE5 17: PE8 18: PE11 19: PE13 20: PE15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15;	    		
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    

    //3: PF1 4: PF3 5: PF5 6; PF7 7: PF9 15: PF15 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_15 ;	    		
    GPIO_Init(GPIOF, &GPIO_InitStructure);    
    
    //8: PC1 9: PC3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_3;	    		
    GPIO_Init(GPIOC, &GPIO_InitStructure);         

    //10: PA1 12: PA5 13: PA7 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_5 |GPIO_Pin_7 ;	    		
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //11: PB6 21: PB11 22: PB13 23: PB15 14: PB1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_1;	    		
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //24: PD9 25: PD11 26: PD13 27: PD6 28: PD4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_6 | GPIO_Pin_4;	    		
    GPIO_Init(GPIOD, &GPIO_InitStructure);
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

        case 15:
            LED15 = 1;   
        break;

        case 16:
            LED16 = 1;
        break;

        case 17:
            LED17 = 1;
        break;       
        
        case 18:
            LED18 = 1;
        break;
        
        case 19:
            LED19 = 1;
        break;
        
        case 20:
            LED20 = 1;
        break;
        
        case 21:
            LED21 = 1;
        break;
        
        case 22:
            LED22 = 1;
        break;
        
        case 23:
            LED23 = 1;
        break;
        
        case 24:
            LED24 = 1;
        break;
        
        case 25:
            LED25 = 1;
        break;
        
        case 26:
            LED26 = 1;
        break; 

        case 27:
            LED27 = 1;
        break;        

        case 28:
            LED28 = 1;
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

        case 15:
            LED15 = 0;   
        break;

        case 16:
            LED16 = 0;
        break;

        case 17:
            LED17 = 0;
        break;       
        
        case 18:
            LED18 = 0;
        break;
        
        case 19:
            LED19 = 0;
        break;
        
        case 20:
            LED20 = 0;
        break;
        
        case 21:
            LED21 = 0;
        break;
        
        case 22:
            LED22 = 0;
        break;
        
        case 23:
            LED23 = 0;
        break;
        
        case 24:
            LED24 = 0;
        break;
        
        case 25:
            LED25 = 0;
        break;
        
        case 26:
            LED26 = 0;
        break; 

        case 27:
            LED27 = 0;
        break;        

        case 28:
            LED28 = 0;
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
    for(i=0; i< LED_NUM; i++){
        turnOn(i);
    }
}

void allOff(void){
    u8 i=0;
    for(i=0; i< LED_NUM; i++){
        turnOff(i);
    }
}


