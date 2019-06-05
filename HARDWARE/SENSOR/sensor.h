#ifndef __SENSOR_H
#define __SENSOR_H

#include "sys.h"
#include "led.h"
#include "delay.h"

#define sensor1 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)
#define sensor2 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)
#define sensor3 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)
#define sensor4 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)
#define sensor5 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8)

#define sensor6 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#define sensor7 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)

#define sensor8 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

#define sensor9 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define sensor10 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define sensor11 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define sensor12 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

#define sensor13 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_14)
#define sensor14 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0)

#define sensor15 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)
#define sensor16 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)

#define sensor17 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)
#define sensor18 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)

#define sensor19 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define sensor20 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define sensor21 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)

#define sensor22 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)
#define sensor23 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10)
#define sensor24 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)

#define sensor25 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)
#define sensor26 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)

void sensorInit(void);

u8 checkBox(u8 boxNumber);

u8 readBox(u8 box);

#endif




