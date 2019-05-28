#ifndef __DOOR_H
#define __DOOR_H 	
#include "sys.h" 

void Door_Init(void);//≥ı ºªØ

#define Door1 PBout(9)
#define Door2 PBout(8)
#define Door3 PBout(7)
#define Door4 PBout(6)

#define DoorSensor GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define DOOR_CLOSED DoorSensor==0
#define DOOR_IS_OPEN DoorSensor==1

#endif



