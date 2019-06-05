#include "sensor.h"

u8 readBox(u8 box){
    u8 status = 0;
    switch(box){
        case 1:
            status = sensor1;
        break;
        
        case 2:
            status = sensor2;
        break;

        case 3:
            status = sensor3;
        break;

        case 4:
            status = sensor4;
        break;
        
        case 5:
            status = sensor5;
        break;

        case 6:
            status = sensor6;
        break;

        case 7:
            status = sensor7;
        break;
        
        case 8:
            status = sensor8;
        break;

        case 9:
            status = sensor9;
        break;

        case 10:
            status = sensor10;
        break;
        
        case 11:
            status = sensor11;
        break;

        case 12:
            status = sensor12;
        break;        

        case 13:
            status = sensor13;
        break;
        
        case 14:
            status = sensor14;
        break;

        case 15:
            status = sensor15;
        break;

        case 16:
            status = sensor16;
        break;
        
        case 17:
            status = sensor17;
        break;

        case 18:
            status = sensor18;
        break;

        case 19:
            status = sensor19;
        break;
        
        case 20:
            status = sensor20;
        break;

        case 21:
            status = sensor21;
        break;

        case 22:
            status = sensor22;
        break;
        
        case 23:
            status = sensor23;
        break;

        case 24:
            status = sensor24;
        break;         

        case 25:
            status = sensor25;
        break;

        case 26:
            status = sensor26;
        break;        
    }

    return status;
}

void sensorInit(void){

 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    
    //sensor 1:PF0 2: PF2 3: PF4 4: PF6 5: PF8 13:PF14  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_8 |GPIO_Pin_14;
 	GPIO_Init(GPIOF, &GPIO_InitStructure);

    //sensor 6:PC0 7:PC2
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_2;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);   

    //sensor 8:PA0 10:PA4 11:PA6
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_6;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);   

    //sensor 9:PB 12:PB0 19:PB10 20:PB12 21:PB14
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_0 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);   

    //sensor 14:PG0  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 ;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);

    //sensor 15:PE7 16: PE10 17: PE12 18: PE14
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7 |GPIO_Pin_10 | GPIO_Pin_12 |GPIO_Pin_14;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);

    //sensor 22:PD8 23:PD10 24:PD12 25:PD5 26:PD3
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_5 | GPIO_Pin_3;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
}


u8 checkBox(u8 boxNumber){
    u8 status;
    
    //开灯检查
    turnOn(boxNumber);
    turnOn(boxNumber + 2);
    
    //10ms之后检查
    delay_ms(1);
    status = readBox(boxNumber);
    
    //关灯
    turnOff(boxNumber);
    turnOff(boxNumber + 2);  

    if(status){
       LED_WORKING = 1;
    }else{
       LED_WORKING = 0;
    }  

    //根据定义, 没票的时候是1
    return !status;    
    
}



