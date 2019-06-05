#include "tasks.h"

u16 timer3Counter = 0;

//50ms 一次
void runTasks(void){
    timer3Counter ++;
    
    //检查通讯 50ms一次
    comTask();
   
    //6秒一次
    if((timer3Counter % 120 )==0){
           
    } 
    
    if((timer3Counter % 40 )==0){
        //从机测试
        //slaveBlinkFlipTest();
        //sendCheckBoxTest();
        
    }
    
    //1秒1次
    if((timer3Counter % 20 )==0){
        //printf("running\r\n");
        //闪动任务.
        ledTask();
        
        //测试检测
        //unitTest4();
        
        //灯板点亮测试
        //blinkTest();
        //checkBox(1);
        
        //发送从机闪烁测试
        //slaveBlinkTest();
                
        //检测带u2发送
        //sendCheckBoxTest2();
        
        //感应测试
        //sensorTest();
        
        //板子测试
        //lightBoardTest();
        
        //unitTest3();
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

