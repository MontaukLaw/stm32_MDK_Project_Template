#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
									  
////////////////////////////////////////////////////////////////////////////////// 
//#define LED0 PBout(5)// PB5
#define LED0 PCout(13)// PC13

#define LED1 PEout(3)
#define LED2 PEout(5)

#define LED3 PFout(1)
#define LED4 PFout(3)
#define LED5 PFout(5)
#define LED6 PFout(7)
#define LED7 PFout(9)

#define LED8 PCout(1)
#define LED9 PCout(3)

#define LED10 PAout(1)
#define LED11 PBout(6)
#define LED12 PAout(5)
#define LED13 PAout(7)
#define LED14 PBout(1)
#define LED15 PFout(15)
#define LED16 PGout(1)

#define LED17 PEout(8)
#define LED18 PEout(11)
#define LED19 PEout(13)
#define LED20 PEout(15)

#define LED21 PBout(11)
#define LED22 PBout(13)
#define LED23 PBout(15)

#define LED24 PDout(9)
#define LED25 PDout(11)
#define LED26 PDout(13)
#define LED27 PDout(6)
#define LED28 PDout(4)

#define LED_WORKING PGout(3)


//void ledOff(u8 ledNumber);
void ledOn(u8 ledNumber);

void LED_Init(void);//≥ı ºªØ
void Switch_Init(void);
		
void ledBlink(u8 ledNumber);  

void allOn(void);
void allOff(void);

void turnOn(u8 ledNumber);
void turnOff(u8 ledNumber);    
#endif



