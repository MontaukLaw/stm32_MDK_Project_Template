#include "tasks.h"

u16 timer3Counter = 0;

//50ms һ��
void runTasks(void){
    timer3Counter ++;
    
    //���ͨѶ 50msһ��
    comTask();
   
    //6��һ��
    if((timer3Counter % 120 )==0){
           
    } 
    
    if((timer3Counter % 40 )==0){
        //�ӻ�����
        //slaveBlinkFlipTest();
        //sendCheckBoxTest();
        
    }
    
    //1��1��
    if((timer3Counter % 20 )==0){
        //printf("running\r\n");
        //��������.
        ledTask();
        
        //���Լ��
        //unitTest4();
        
        //�ư��������
        //blinkTest();
        //checkBox(1);
        
        //���ʹӻ���˸����
        //slaveBlinkTest();
                
        //����u2����
        //sendCheckBoxTest2();
        
        //��Ӧ����
        //sensorTest();
        
        //���Ӳ���
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

