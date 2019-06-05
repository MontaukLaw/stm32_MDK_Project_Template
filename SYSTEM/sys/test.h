#ifndef __TEST_H
#define __TEST_H	 

#include "sys.h"
#include "led.h"
#include "delay.h"
#include "com_task.h"
#include "usart.h"
#include "sensor.h"
#include "upcom.h"

void unitTest1(void);

void unitTest2(void);

void unitTest3(void);

void unitTest4(void);

void blinkTest(void);

void problemTest(void);

void sensorTest(void);

void lightBoardTest(void);

void keepLightSensorTest(void);

void slaveBlinkTest(void);
  
void slaveBlinkFlipTest(void);

void allSlaveBlinkTest(void);

void sendCheckBoxTest(void);

void sendCheckBoxTest2(void);

#endif
