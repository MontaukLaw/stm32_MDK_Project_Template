#ifndef __COM_TASK_H
#define __COM_TASK_H

#include "usart.h"
#include "upcom.h"

#include "led.h"
#include "test.h"


void comTask(void);
void sendACK(void);


#define OFF 0


#endif
