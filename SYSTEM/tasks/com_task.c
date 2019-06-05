#include "com_task.h"

//��������
#define MAX_BOX_NUMBER 26

extern u8 transFlagU2;
extern u8 transFlag;
extern u8 transFlagU4;
extern u8 transFlagU5;

extern u8 usartRecieveCounter;
extern u8 usartRecieveCounterU2; 
extern u8 usartRecieveCounterU4;
extern u8 usartRecieveCounterU5;

extern u8 msgFlag;

extern u8 USART2_RX_BUF[USART_REC_LEN]; 
extern u8 USART4_RX_BUF[USART_REC_LEN]; 
extern u8 USART5_RX_BUF[USART_REC_LEN]; 

extern char NOTE_INBOX_QCMD_MSG[];
extern char BLINK_LED_CMD[];
extern char TURNON_LED_CMD[];
extern char TURNOFF_LED_CMD[];

extern u8 SN[];

extern struct BLMSG
{
    u8 cmdBody[255];
    u8 length;
    u8 chk;
    u8 id;
    u8 cmd;
    u8 integrity;
}blMsg;

u8 blinkGridNumber = 0;
u8 lightOnNumber = 0;
u8 lightStatus = OFF;

void u245Analyze(void){
    u8 boxNumber = 0 ;
    u8 cmd = 0;
    u8 boxStatus = 0;
    u8 msgID = 0;
 
    // ���u2����Ϣ    
    if(transFlagU2 == TRANS_END)
	{   
        //�����������Լ��
        //ff ff 65 03 01 61 01 01 fe fe
        cmd = USART2_RX_BUF[5];
        msgID = USART2_RX_BUF[4];
        //u2��25��ʼ��52
        boxNumber = USART2_RX_BUF[6] + MAX_BOX_NUMBER;
        boxStatus = USART2_RX_BUF[7];
        
        //���u2�յ�Ʊ�����
        
        //�������ָ���ʼ״̬, anyway
        transFlagU2 = TRANS_NONE; 
        usartRecieveCounterU2 = 0; 
    } 
   
    // ���u4����Ϣ   
    if(transFlagU4 == TRANS_END){
        cmd = USART4_RX_BUF[5];
        msgID = USART4_RX_BUF[4];
        //u4��53��ʼ��78
        boxNumber = USART4_RX_BUF[6] + MAX_BOX_NUMBER * 2;
        boxStatus = USART4_RX_BUF[7];
        
        //�������ָ���ʼ״̬, anyway
        transFlagU4 = TRANS_NONE; 
        usartRecieveCounterU4 = 0;
    }
 
    // ���u5����Ϣ   
    if(transFlagU5 == TRANS_END){
        cmd = USART5_RX_BUF[5];
        msgID = USART5_RX_BUF[4];
        //u5��79��ʼ��102
        boxNumber = USART5_RX_BUF[6] + MAX_BOX_NUMBER * 3;
        boxStatus = USART5_RX_BUF[7];
        
        //�������ָ���ʼ״̬, anyway
        transFlagU5 = TRANS_NONE; 
        usartRecieveCounterU5 = 0;
    }
    
    if(cmd == NOTE_INBOX_ACMD){            
        sendCheckBoxMsgToBT(boxNumber,boxStatus,msgID);            
    }
}

//ͨѶ����
void comAnalyze(void){
    u8 boxNumber = 0 ;
    u8 boxStatus = 0; 
    u8 slaveIndex = 0;

    u245Analyze();  
    
    if(transFlag == TRANS_END)
	{   
        //���blMsg struct
        transBLMsg();
        
        //�������������
        if (blMsg.integrity){
            blinkGridNumber = blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG];
            
            //�����������������
            if(blinkGridNumber > MAX_BOX_NUMBER){
                
                slaveIndex = (blinkGridNumber - 1) / MAX_BOX_NUMBER;
                boxNumber = blinkGridNumber % MAX_BOX_NUMBER;
                if(boxNumber == 0){
                    boxNumber = MAX_BOX_NUMBER;
                }
            }
                        
            switch(blMsg.cmd){
                case GRID_BLINK:
                    //�ȹ�
                    //allLedOff();
                    allOff();
                                       
                    //�������д���
                    if(slaveIndex == 0){
                        lightStatus = BLINK; 
                    }else{
                        sendSlaveMsg(slaveIndex, boxNumber, BLINK_LED_CMD, LED_TX_TO_SLAVE_LEN, LED_TX_TO_SLAVE_LEN);  
                    }                
                    
                    //if(blinkGridNumber <= MAX_BOX_NUMBER){
                        
                        //lightStatus = BLINK; 
                    //}else {
                        //�����ӻ�����
                        //sendSlaveMsg(slaveIndex,boxNumber,BLINK_LED_CMD,LED_TX_TO_SLAVE_LEN,LED_TX_TO_SLAVE_LEN);                        
                    //}
                   
                    //���������ҪACK
                    sendACK();
                break;
                
                case OPEN_DOOR:
                    openDoor(blMsg.cmdBody[DOOR_NUMBER_INDEX_IN_MSG]);
                    //����Ҳ��ҪACK   
                    sendACK();
                break;                
                
                case TURN_OFF_LED:                   
                    //lightStatus = OFF;
                    //allLedOff();
                    //blinkGridNumber = 0;
                    //turnOff(blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG]);
                
                     //�������д���
                    if(slaveIndex == 0){
                        lightStatus = OFF;
                        allOff();
                    }else{
                        sendSlaveMsg(slaveIndex, boxNumber, TURNOFF_LED_CMD, LED_TX_TO_SLAVE_LEN, LED_TX_TO_SLAVE_LEN);  
                    }    
                
                    //���������ҪACK
                    sendACK();                    
                break; 
                
                case ALL_DOOR_STATUS:
                    sendDoorsStatus();
                break;
                
                case DOOR_STATUS_QCMD:
                    break;
                
                //HJBL..��ѯ�������Ƿ���Ʊ
                case NOTE_INBOX_QCMD:
                    //��ȡҪ��ѯ�ĺ��Ӻ���
                    boxNumber=blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG];
                    //����Ƿ��ѯ��ģ��ĺ���
                    if(boxNumber <= MAX_BOX_NUMBER){
                        //��ȡ����������Ƿ���Ʊ
                        boxStatus = checkBox(boxNumber);
                        //���ͽ����BT
                        sendCheckBoxMsgToBT(boxNumber,boxStatus,blMsg.id);
                    //�����27~52��, �ͷ���U2, �����                        
                    }else{
                        slaveIndex = boxNumber/MAX_BOX_NUMBER;
                        boxNumber = boxNumber % MAX_BOX_NUMBER;
                        if(boxNumber == 0){
                            boxNumber = MAX_BOX_NUMBER;
                        }
                        //������Ϣ��U2, U4, U5
                        sendSlaveMsg(slaveIndex, boxNumber, NOTE_INBOX_QCMD_MSG, LED_TX_TO_SLAVE_LEN,blMsg.id);
                    
                    }                    
                                           
                break;
                
                case GRID_LIGNT_ON:
                    //allLedOff();
                   
                    //�Լ�����Ĳ���
                    if(slaveIndex == 0){
                        lightStatus = ALWAYS_ON;
                        blinkGridNumber = blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG];
                        
                    //�ⷢ
                    }else{
                        sendSlaveMsg(slaveIndex, boxNumber, TURNON_LED_CMD, LED_TX_TO_SLAVE_LEN, LED_TX_TO_SLAVE_LEN);  
                    }    
                
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
        msgFlag = 0;
    }

}


