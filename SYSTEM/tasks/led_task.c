#include "led_task.h"

u8 ledOnFlag = 0;
extern u8 lightStatus;
extern u8 blinkGridNumber;

void ledTask(void){
    switch(lightStatus){
        case OFF:
            break;
            
        case BLINK:            
            ledBlink(blinkGridNumber);                              
        break;
            
        case ALWAYS_ON:
            turnOn(blinkGridNumber);
        break;       
    }

}

