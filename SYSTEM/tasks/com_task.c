#include "com_task.h"

//盒子总数
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
 
    // 如果u2有消息    
    if(transFlagU2 == TRANS_END)
	{   
        //不进行完整性检查
        //ff ff 65 03 01 61 01 01 fe fe
        cmd = USART2_RX_BUF[5];
        msgID = USART2_RX_BUF[4];
        //u2从25开始到52
        boxNumber = USART2_RX_BUF[6] + MAX_BOX_NUMBER;
        boxStatus = USART2_RX_BUF[7];
        
        //如果u2收到票检测结果
        
        //分析完后恢复初始状态, anyway
        transFlagU2 = TRANS_NONE; 
        usartRecieveCounterU2 = 0; 
    } 
   
    // 如果u4有消息   
    if(transFlagU4 == TRANS_END){
        cmd = USART4_RX_BUF[5];
        msgID = USART4_RX_BUF[4];
        //u4从53开始到78
        boxNumber = USART4_RX_BUF[6] + MAX_BOX_NUMBER * 2;
        boxStatus = USART4_RX_BUF[7];
        
        //分析完后恢复初始状态, anyway
        transFlagU4 = TRANS_NONE; 
        usartRecieveCounterU4 = 0;
    }
 
    // 如果u5有消息   
    if(transFlagU5 == TRANS_END){
        cmd = USART5_RX_BUF[5];
        msgID = USART5_RX_BUF[4];
        //u5从79开始到102
        boxNumber = USART5_RX_BUF[6] + MAX_BOX_NUMBER * 3;
        boxStatus = USART5_RX_BUF[7];
        
        //分析完后恢复初始状态, anyway
        transFlagU5 = TRANS_NONE; 
        usartRecieveCounterU5 = 0;
    }
    
    if(cmd == NOTE_INBOX_ACMD){            
        sendCheckBoxMsgToBT(boxNumber,boxStatus,msgID);            
    }
}

//通讯分析
void comAnalyze(void){
    u8 boxNumber = 0 ;
    u8 boxStatus = 0; 
    u8 slaveIndex = 0;

    u245Analyze();  
    
    if(transFlag == TRANS_END)
	{   
        //填充blMsg struct
        transBLMsg();
        
        //检查数据完整性
        if (blMsg.integrity){
            blinkGridNumber = blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG];
            
            //如果不是由主机处理
            if(blinkGridNumber > MAX_BOX_NUMBER){
                
                slaveIndex = (blinkGridNumber - 1) / MAX_BOX_NUMBER;
                boxNumber = blinkGridNumber % MAX_BOX_NUMBER;
                if(boxNumber == 0){
                    boxNumber = MAX_BOX_NUMBER;
                }
            }
                        
            switch(blMsg.cmd){
                case GRID_BLINK:
                    //先关
                    //allLedOff();
                    allOff();
                                       
                    //主机进行处理
                    if(slaveIndex == 0){
                        lightStatus = BLINK; 
                    }else{
                        sendSlaveMsg(slaveIndex, boxNumber, BLINK_LED_CMD, LED_TX_TO_SLAVE_LEN, LED_TX_TO_SLAVE_LEN);  
                    }                
                    
                    //if(blinkGridNumber <= MAX_BOX_NUMBER){
                        
                        //lightStatus = BLINK; 
                    //}else {
                        //发给从机处理
                        //sendSlaveMsg(slaveIndex,boxNumber,BLINK_LED_CMD,LED_TX_TO_SLAVE_LEN,LED_TX_TO_SLAVE_LEN);                        
                    //}
                   
                    //开灯命令都需要ACK
                    sendACK();
                break;
                
                case OPEN_DOOR:
                    openDoor(blMsg.cmdBody[DOOR_NUMBER_INDEX_IN_MSG]);
                    //开门也需要ACK   
                    sendACK();
                break;                
                
                case TURN_OFF_LED:                   
                    //lightStatus = OFF;
                    //allLedOff();
                    //blinkGridNumber = 0;
                    //turnOff(blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG]);
                
                     //主机进行处理
                    if(slaveIndex == 0){
                        lightStatus = OFF;
                        allOff();
                    }else{
                        sendSlaveMsg(slaveIndex, boxNumber, TURNOFF_LED_CMD, LED_TX_TO_SLAVE_LEN, LED_TX_TO_SLAVE_LEN);  
                    }    
                
                    //开灯命令都需要ACK
                    sendACK();                    
                break; 
                
                case ALL_DOOR_STATUS:
                    sendDoorsStatus();
                break;
                
                case DOOR_STATUS_QCMD:
                    break;
                
                //HJBL..查询盒子内是否有票
                case NOTE_INBOX_QCMD:
                    //获取要查询的盒子号码
                    boxNumber=blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG];
                    //检查是否查询本模组的盒子
                    if(boxNumber <= MAX_BOX_NUMBER){
                        //获取这个盒子内是否有票
                        boxStatus = checkBox(boxNumber);
                        //发送结果到BT
                        sendCheckBoxMsgToBT(boxNumber,boxStatus,blMsg.id);
                    //如果是27~52号, 就发到U2, 如果是                        
                    }else{
                        slaveIndex = boxNumber/MAX_BOX_NUMBER;
                        boxNumber = boxNumber % MAX_BOX_NUMBER;
                        if(boxNumber == 0){
                            boxNumber = MAX_BOX_NUMBER;
                        }
                        //发送消息到U2, U4, U5
                        sendSlaveMsg(slaveIndex, boxNumber, NOTE_INBOX_QCMD_MSG, LED_TX_TO_SLAVE_LEN,blMsg.id);
                    
                    }                    
                                           
                break;
                
                case GRID_LIGNT_ON:
                    //allLedOff();
                   
                    //自己处理的部分
                    if(slaveIndex == 0){
                        lightStatus = ALWAYS_ON;
                        blinkGridNumber = blMsg.cmdBody[LED_NUMBER_INDEX_IN_MSG];
                        
                    //外发
                    }else{
                        sendSlaveMsg(slaveIndex, boxNumber, TURNON_LED_CMD, LED_TX_TO_SLAVE_LEN, LED_TX_TO_SLAVE_LEN);  
                    }    
                
                    //开灯命令都需要ACK
                    sendACK();
                break;
                
                //查询固件版本
                case FW_VER_QUERY:
                    sendFWVer();
                break;
                
                case SN_QUERY:
                    sendSN();
                break;
            }
            
            //只要数据是完整的, 就发ACK
           
        }
        
        //分析完后恢复初始状态, anyway
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


