#include "test.h"

void unitTest1(void){
    while(1){
        LED_WORKING = 1;
        turnOn(1);
        turnOff(2);
        delay_ms(999); 
        LED_WORKING = 0;
        turnOff(1);
        turnOn(2);
        delay_ms(999); 
    }
}

extern u8 lightStatus;
extern u8 blinkGridNumber;

void unitTest2(void){
    lightStatus = BLINK;
    blinkGridNumber = 1;    
    
}

