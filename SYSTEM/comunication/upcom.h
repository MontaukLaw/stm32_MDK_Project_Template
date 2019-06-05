#ifndef __UPCOM_H
#define __UPCOM_H

#include "usart.h"
#include "door_task.h"

#define GRID_BLINK  0x10
#define TURN_OFF_LED 0x11
#define GRID_LIGNT_ON 0x12
//#define GRID_LIGNT_OFF 0x13
#define OPEN_DOOR 0x20
#define FW_VER_QUERY 0x40
#define SN_QUERY 0x50
#define ALL_DOOR_STATUS 0x23

#define DOOR_STATUS_QCMD 0x21
#define DOOR_STATUS_ACMD 0x22

#define NOTE_INBOX_QCMD 0x60
#define NOTE_INBOX_ACMD 0x61

#define OFF 0
#define BLINK 1
#define ALWAYS_ON 2

#define LED_NUMBER_INDEX_IN_MSG  2
#define DOOR_NUMBER_INDEX_IN_MSG  2

#define CONTROL_SLAVE_LED_BOX_MSG_LENTH 9

#define ID_INDEX   4
#define CHK_INDEX   2 
#define STATUS_IDEX 6

#define LED_INDEX 6
#define ACK_LEN 8
#define FW_VERSION_PACKAGE_LEN  10
#define SN_PKG_LEN 20
#define DOOR_STATUS_LEN 9
#define LED_TX_TO_SLAVE_LEN 9

#define CHECK_BOX_LENTH 10
#define CHECK_BOX_IDEX 6
#define CHECK_BOX_STATUS_IDEX 7

//这个消息长度length并不包含校验位, 包头跟包尾.
//仅表示包的有效内容字节数.


//BLMsg blMsg;
void Get_ChipID(void);

void transBLMsg(void);

void sendDoorsStatus(void);

void sendFWVer(void);

void sendSN(void);

//void sendSlaveBlinkMsg(u8 slaveIndex);

void sendSlaveBlinkMsg(u8 slaveIndex,u8 ledIndex);

void sendSlaveTurnOnMsg(u8 slaveIndex,u8 ledIndex);

void sendSlaveTurnOffMsg(u8 slaveIndex,u8 ledIndex);

void sendLEDMsg(u8 slaveIndex,u8 ledIndex,char* msg,u8 length);

void sendSlaveMsg(u8 slaveIndex,u8 ledIndex,char* msg, u8 length, u8 msgID);

void sendCheckBoxMsgToBT(u8 boxNumber,u8 boxStatus,u8 msgID);

#endif


