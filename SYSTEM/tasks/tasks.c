#include "tasks.h"

u16 timer3Counter = 0;

//50ms 一次
void runTasks(void){
    timer3Counter ++;
    
    //检查通讯 50ms一次
    comTask();
    
    //1秒1次
    if((timer3Counter % 20 )==0){
        //printf("running\r\n");
        ledTask();
        //just for test ledTask();
        //ledBlink(2);
        //ledBlink(3);
        //just for test sendACK
        //sendACK();
        
        //just for test 
        //openDoor(1);
    } 

        
    
    if(timer3Counter > 10000){
        timer3Counter = 0;
    }
}

