#include "upcom.h"
#include "led.h"

//��2, У��, ������, id, cmd, β2
char ACK_CMD[] = {0xff,0xff,0x12,0x02,0x00,0x30,0xfe,0xfe};

//�̼��汾Ŀǰ��2.1
char FW_VERSION[] = {0xff,0xff,0x12,0x04,0x02,0x40,0x02,0x01,0xfe,0xfe};

//���кŰ�,��2, У��, ������, id, cmd, β2
char SN[]={0xff,0xff,0x12,0x0e,0x00,0x50,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0xfe,0xfe};
    
//ȫ����״̬, ��������3 byte, id, cmd, status    
char DOOR_STATUS[]={0xff,0xff,0x12,0x03,0x00,0x24,0x00,0xfe,0xfe};    

//������2, У��, ������, id, cmd(0x10), ledIndex, β2
//ff ff 15 03 01 10 01 fe fe
char BLINK_LED_CMD[]={0xff, 0xff, 0x15 ,0x03 ,0x01 ,0x10 ,0x01 ,0xfe ,0xfe};

//�ص���2, У��, ������, id, cmd(0x11), ledIndex, β2
//ff ff 15 03 01 11 01 fe fe
char TURNOFF_LED_CMD[]={0xff, 0xff, 0x15 ,0x03 ,0x01 ,TURN_OFF_LED,0x01 ,0xfe ,0xfe};

//��������2, У��, ������, id, cmd(0x12), ledIndex, β2
//ff ff 15 03 01 12 01 fe fe
char TURNON_LED_CMD[]={0xff, 0xff, 0x15 ,0x03 ,0x01 ,0x12 ,0x01 ,0xfe ,0xfe};

//ff ff 65 03 01 60 01 fe fe
char NOTE_INBOX_QCMD_MSG[]={0xff, 0xff, 0x65 ,0x03 ,0x01 ,0x60 ,0x01 ,0xfe ,0xfe};

//ff ff 65 03 01 61 01 01 fe fe
char NOTE_INBOX_A_MSG[]={0xff, 0xff, 0x65 ,0x03 ,0x01 ,0x61 ,0x01, 0x01 ,0xfe ,0xfe};


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

void sendCheckBoxMSg(u8 slaveIndex,u8 ledIndex){
    
}

void sendCheckBoxMsgToBT(u8 boxNumber,u8 boxStatus,u8 msgID){
    u8 i = 0;
    u8 chk = 0;
    NOTE_INBOX_A_MSG[ID_INDEX] = msgID;
    NOTE_INBOX_A_MSG[CHECK_BOX_STATUS_IDEX] = boxStatus;
    NOTE_INBOX_A_MSG[CHECK_BOX_IDEX] = boxNumber;
    
    //У�����ĳ���ҲҪע���ַ�������.
    for(i = 3; i< CHECK_BOX_LENTH - 2; i++){
        chk = chk + NOTE_INBOX_A_MSG[i];
    }
    NOTE_INBOX_A_MSG[CHK_INDEX] = chk;
   
    btSend(NOTE_INBOX_A_MSG,CHECK_BOX_LENTH);    
}

void sendSlaveMsg(u8 slaveIndex,u8 ledIndex,char* msg,u8 length, u8 msgID){
    u8 i;
    msg[LED_INDEX] = ledIndex;

    msg[ID_INDEX] = msgID;
    //У�� 
    msg[CHK_INDEX] = 0;
 
    for(i = 3;i < 7; i++){
        msg[CHK_INDEX] =msg[CHK_INDEX] + msg[i];
    }
    
    //����
    switch(slaveIndex){
        case 1:
            uart2Send(msg,length);
        break;
        
        case 2:
            uart4Send(msg,length);
        break;
        
        case 3: 
            uart5Send(msg,length);
        break;    
    }
    
}

void sendLEDMsg(u8 slaveIndex,u8 ledIndex,char* msg,u8 length){
    u8 i;
    msg[LED_INDEX] = ledIndex;

    //У�� 
    msg[CHK_INDEX] = 0;
 
    for(i = 3;i < 7; i++){
        msg[CHK_INDEX] =msg[CHK_INDEX] + msg[i];
    }
    
    //����
    switch(slaveIndex){
        case 1:
            uart2Send(msg,length);
        break;
        
        case 2:
            uart4Send(msg,length);
        break;
        
        case 3: 
            uart5Send(msg,length);
        break;    
    }
}

//�ص�
//u2, u4, u5�����ӻ���ӦindexΪ1, 2, 3
void sendSlaveTurnOffMsg(u8 slaveIndex,u8 ledIndex){
    u8 i;
    TURNOFF_LED_CMD[6] = ledIndex;

    //У�� 
    TURNOFF_LED_CMD[2] = 0;
 
    for(i = 3;i < 7; i++){
        TURNOFF_LED_CMD[2] =TURNOFF_LED_CMD[2] + TURNOFF_LED_CMD[i];
    }
    
    //����
    switch(slaveIndex){
        case 1:
            uart2Send(TURNOFF_LED_CMD,LED_TX_TO_SLAVE_LEN);
        break;
        
        case 2:
            uart4Send(TURNOFF_LED_CMD,LED_TX_TO_SLAVE_LEN);
        break;
        
        case 3: 
            uart5Send(TURNOFF_LED_CMD,LED_TX_TO_SLAVE_LEN);
        break;    
    }
}

//u2, u4, u5�����ӻ���ӦindexΪ1, 2, 3
void sendSlaveTurnOnMsg(u8 slaveIndex,u8 ledIndex){
    u8 i;
    TURNON_LED_CMD[6] = ledIndex;

    //У�� 
    TURNON_LED_CMD[2] = 0;
 
    for(i = 3;i < 7; i++){
        TURNON_LED_CMD[2] =TURNON_LED_CMD[2] + TURNON_LED_CMD[i];
    }
    
    //����
    switch(slaveIndex){
        case 1:
            uart2Send(TURNON_LED_CMD,LED_TX_TO_SLAVE_LEN);
        break;
        
        case 2:
            uart4Send(TURNON_LED_CMD,LED_TX_TO_SLAVE_LEN);
        break;
        
        case 3: 
            uart5Send(TURNON_LED_CMD,LED_TX_TO_SLAVE_LEN);
        break;    
    }
}

//u2, u4, u5�����ӻ���ӦindexΪ1, 2, 3
void sendSlaveBlinkMsg(u8 slaveIndex,u8 ledIndex){
    u8 i;
    BLINK_LED_CMD[6] = ledIndex;

    //У�� 
    BLINK_LED_CMD[2] = 0;
 
    for(i = 3;i < 7; i++){
        BLINK_LED_CMD[2] =BLINK_LED_CMD[2] + BLINK_LED_CMD[i];
    }
    switch(slaveIndex){
        case 1:
            uart2Send(BLINK_LED_CMD,LED_TX_TO_SLAVE_LEN);
        break;
        
        case 2:
            uart4Send(BLINK_LED_CMD,LED_TX_TO_SLAVE_LEN);
        break;
        
        case 3: 
            uart5Send(BLINK_LED_CMD,LED_TX_TO_SLAVE_LEN);
        break;    
    }
}

//�������յ�����Ϣ����struct��װ.
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

//������״̬
void sendDoorsStatus(void){
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

//����ACK
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

//���͹̼��汾
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

//�������к�
void sendSN(void){
    u8 i = 0;
    u8 chk = 0; 
    SN[ID_INDEX] = blMsg.id;
    for(i=3; i < SN_PKG_LEN -2;i++){
        chk = chk + SN[i];
    }
    SN[CHK_INDEX]=chk;
    btSend(SN,SN_PKG_LEN);    
}

//��ѯ���к�
//�����ϲ�Ӧ�������.
void Get_ChipID(void)
{
    u32 ChipUniqueID[3];

    ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8); 
    ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); 
    ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0);  
    //����˵Ҫע����С��...
    SN[6]=ChipUniqueID[0] >> 24;
    SN[7]=ChipUniqueID[0] >> 16;
    SN[8]=ChipUniqueID[0] >> 8;
    SN[9]=ChipUniqueID[0];
    SN[10]=ChipUniqueID[1] >> 24;
    SN[11]=ChipUniqueID[1] >> 16 ;
    SN[12]=ChipUniqueID[1] >> 8;
    SN[13]=ChipUniqueID[1];
    SN[14]=ChipUniqueID[2] >> 24;
    SN[15]=ChipUniqueID[2] >> 16;
    SN[16]=ChipUniqueID[2] >> 8;
    SN[17]=ChipUniqueID[2];
}
