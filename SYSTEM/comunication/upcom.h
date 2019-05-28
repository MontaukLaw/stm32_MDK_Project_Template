#ifndef __UPCOM_H
#define __UPCOM_H

#include "usart.h"
#include "door_task.h"

#define GRID_BLINK  0x10
#define TURN_OFF_LED 0x11
#define GRID_LIGNT_ON 0x12
#define OPEN_DOOR 0x20
#define FW_VER_QUERY 0x40
#define SN_QUERY 0x50
#define ALL_DOOR_STATUS 0x23

#define OFF 0
#define BLINK 1
#define ALWAYS_ON 2

#define LED_NUMBER_INDEX_IN_MSG  2
#define DOOR_NUMBER_INDEX_IN_MSG  2

//这个消息长度length并不包含校验位, 包头跟包尾.
//仅表示包的有效内容字节数.

//BLMsg blMsg;
void Get_ChipID(void);

#endif


