#include "com_task.h"
#define ID_INDEX   4
#define CHK_INDEX   2 
#define STATUS_IDEX 6

#define ACK_LEN 8
#define FW_VERSION_PACKAGE_LEN  10
#define SN_PKG_LEN 20
#define DOOR_STATUS_LEN 9

//��2, У��, ������, id, cmd, β2
char ACK_CMD[] = {0xff,0xff,0x12,0x02,0x00,0x30,0xfe,0xfe};

//�̼��汾Ŀǰ��1.1
char FW_VERSION[] = {0xff,0xff,0x12,0x04,0x02,0x40,0x01,0x01,0xfe,0xfe};


//���кŰ�,��2, У��, ������, id, cmd, β2
char SN[]={0xff,0xff,0x12,0x0e,0x00,0x50,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0xfe,0xfe};

//ȫ����״̬, ��������3 byte, id, cmd, status    
char DOOR_STATUS[]={0xff,0xff,0x12,0x03,0x00,0x24,0x00,0xfe,0xfe};    

extern u8 msgFlag;
extern u8 transFlag;
extern u8 usartRecieveCounter;

struct BLMSG
{
    u8 cmdBody[255];
    u8 length;
    u8 chk;
    u8 id;
    u8 cmd;
    u8 integrity;
}blMsg;

void transBLMsg(void){
    u8 chk = 0;
    u8 i = 0;    
    if(USART_RX_BUF[0] == 0xFF && USART_RX_BUF[1] == 0xFF && USART_RX_BUF[usartRecieveCounter-2] == 0xFE && USART_RX_BUF[usartRecieveCounter-1] == 0xFE){
        blMsg.chk = USART_RX_BUF[2];
        blMsg.length = USART_RX_BUF[3];
        blMsg.id = USART_RX_BUF[4];     
        blMsg.cmd = USART_RX_BUF[5];       
        //blMsg.cmdBody = &USART_RX_BUF[6];
        for(i = 3; i < usartRecieveCounter - 2; i++){
            chk = chk + USART_RX_BUF[i];
            blMsg.cmdBody[i-3] = USART_RX_BUF[i+1];
        }
        if(chk == blMsg.chk){
            
            blMsg.integrity = 1;
        }else{
 
            blMsg.integrity = 0;
        }
         
    }else{

        blMsg.integrity = 0;
    }
}

void sendDoorsStatus(){
    u8 i = 0;
    u8 chk = 0;
    //�������Э��, 1�ǿ�.
    u8 status = DoorSensor;
    DOOR_STATUS[ID_INDEX] = blMsg.id;
    DOOR_STATUS[STATUS_IDEX] = status;
    
    //У�����ĳ���ҲҪע���ַ�������.
    for(i = 3;i< DOOR_STATUS_LEN - 2;i++){
        chk = chk + DOOR_STATUS[i];
    }
    DOOR_STATUS[CHK_INDEX] = chk;
    //����Door״̬���ַ���
    btSend(DOOR_STATUS,DOOR_STATUS_LEN);
}

void sendACK(void){
    u8 i = 0;
    u8 chk = 0;
    ACK_CMD[ID_INDEX] = blMsg.id;
    for(i = 3;i< ACK_LEN - 2;i++){
        chk = chk + ACK_CMD[i];
    }
    ACK_CMD[CHK_INDEX] = chk;
    btSend(ACK_CMD,ACK_LEN);
}

void sendFWVer(void){  
   u8 i = 0;
   u8 chk = 0; 
   FW_VERSION[ID_INDEX] = blMsg.id;
   
   for(i=3;i< FW_VERSION_PACKAGE_LEN - 2;i++){
       chk = chk + FW_VERSION[i];
   }
   FW_VERSION[CHK_INDEX] = chk;
   btSend(FW_VERSION,FW_VERSION_PACKAGE_LEN);    
}

void sendSN(){
    u8 i = 0;
    u8 chk = 0; 
    SN[ID_INDEX] = blMsg.id;
    for(i=3; i < SN_PKG_LEN -2;i++){
        chk = chk + SN[i];
    }
    SN[CHK_INDEX]=chk;
    btSend(SN,SN_PKG_LEN);    
}

u8 blinkGridNumber = 0;
u8 lightOnNumber = 0;
u8 lightStatus = OFF;

//�ƿ���
void comAnalyze(void){

    if(transFlag == TRANS_END)
	{   
        //���blMsg struct
        transBLMsg();
        
        //�������������
        if (blMsg.integrity){
            switch(blMsg.cmd){
                case GRID_BLINK:
                    //�ȹ�
                    //allLedOff();
                    lightStatus = BLINK;
                    blinkGridNumber = blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG];
                
                    //���������ҪACK
                    sendACK();
                break;
                
                case OPEN_DOOR:
                    openDoor(blMsg.cmdBody[DOOR_NUMBER_INDEX_IN_MSG]);
                    //����Ҳ��ҪACK   
                    sendACK();
                break;
                
                case TURN_OFF_LED:                   
                    lightStatus = OFF;
                    //allLedOff();
                    blinkGridNumber = 0;
                    allOff(); 
                    //���������ҪACK
                    sendACK();                    
                break; 
                
                case ALL_DOOR_STATUS:
                    sendDoorsStatus();
                break;
                
                case GRID_LIGNT_ON:
                    //allLedOff();
                    lightStatus = ALWAYS_ON;
                    blinkGridNumber = blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG];
                
                    //���������ҪACK
                    sendACK();
                break;
                
                //��ѯ�̼��汾
                case FW_VER_QUERY:
                    sendFWVer();
                break;
                
                case SN_QUERY:
                    sendSN();
                break;
            }
            
            //ֻҪ������������, �ͷ�ACK
           
        }
        
        //�������ָ���ʼ״̬, anyway
        transFlag = TRANS_NONE; 
        usartRecieveCounter = 0;    
    }
    
}

void comTask(void){
    if(msgFlag == NEW_MSG){
        comAnalyze();
    }

}


