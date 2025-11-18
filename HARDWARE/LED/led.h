#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED PCout(13)// PC13 - User board LED
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5

void LED_Init(void);

#endif
