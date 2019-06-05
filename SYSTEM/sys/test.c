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

void unitTest3(void){
     
   uart4Send("abcde",5);
   uart5Send("fghij",5);
   uart2Send("klmln",5); 
}

void unitTest4(void){
   
    checkBox(1);

}

u8 testBlinkBox = 0;
void blinkTest(void){
    testBlinkBox++;
    
    turnOff(testBlinkBox - 1);
    
    if(testBlinkBox == 27){
        testBlinkBox = 1;
    }
    

    turnOn(testBlinkBox);
    
}

//14,16�ŵƲ���, ������������.
void problemTest(){
    turnOn(14);
    turnOn(16);
}

u8 testSensorIndex = 0;
void sensorTest(void){
    
    //checkBox(1);
    testSensorIndex ++;
    checkBox(testSensorIndex);
    if(testSensorIndex == 26){
        testSensorIndex = 0;
    }

}

u8 testFlag = 0;
void lightBoardTest(void){
        
    if(testFlag){
        checkBox(1);
    }else{
        checkBox(2);
    }
    testFlag= !testFlag;    

}

void keepLightSensorTest(void){
    u8 status;
    
    
    //���Ƽ��
    turnOn(1);
    turnOn(3);
    
    //10ms֮����
    delay_ms(10);
    status = readBox(1);
    
    //�ص�
    //turnOff(boxNumber);
    //turnOff(boxNumber + 2);  

    if(status){
       LED_WORKING = 1;
    }else{
       LED_WORKING = 0;
    }   

}

//���Դӻ�����ͨѶ
void allSlaveBlinkTest(void){
    sendSlaveBlinkMsg(1,1);
    sendSlaveBlinkMsg(2,1);
    sendSlaveBlinkMsg(3,1);    
}

extern char BLINK_LED_CMD[];
extern char TURNOFF_LED_CMD[];
extern char TURNON_LED_CMD[];

u8 ledIndex = 0;
u8 flipFlag = 0;
//�������Ӧ�ñ���, �صƸ�����, ����˵��������֮��, Ҫ���50ms����, ��Ϊ���
void slaveBlinkFlipTest(void){
    //u8 i = 0;
    
    //sendSlaveBlinkMsg(1,ledIndex);
    //sendSlaveTurnOnMsg(1,ledIndex);
    
    //
    if(flipFlag){
        sendLEDMsg(1,ledIndex,TURNON_LED_CMD,9);

    }else{
        sendLEDMsg(1,ledIndex,TURNOFF_LED_CMD,9);
        ledIndex ++;
    }
    flipFlag = !flipFlag;
    
    if(ledIndex == 27){
        ledIndex = 1;
    }
    //sendSlaveTurnOnMsg(1,ledIndex);
    //BLINK_LED_CMD[6] = ledIndex;
    
    //BLINK_LED_CMD[2] = 0;
    
    //for(i = 3;i < 7; i++){
        //BLINK_LED_CMD[2] =BLINK_LED_CMD[2] + BLINK_LED_CMD[i];
    //}
    
    //uart2Send(BLINK_LED_CMD,9);  
}

extern char NOTE_INBOX_QCMD_MSG[];

void sendCheckBoxTest(void){

    sendSlaveMsg(1, 1, NOTE_INBOX_QCMD_MSG,CONTROL_SLAVE_LED_BOX_MSG_LENTH,1);
}

void sendCheckBoxTest2(void){
    u8 status = checkBox(1);
    NOTE_INBOX_QCMD_MSG[7] = status;
    
    //sendSlaveMsg(1, 1, NOTE_INBOX_QCMD_MSG,CONTROL_SLAVE_LED_BOX_MSG_LENTH);
}

