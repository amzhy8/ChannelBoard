#ifndef _BSP_LED_H__
#define _BSP_LED_H__
#include "gpio.h"
#define LED_TOGGLE_PERIOD 1000   //MS

#define LED_GPIO_PORT GPIOA
#define LED_GPIO_PIN 	RUN_LED_Pin


void LED_Control(uint8_t state);
void LED_Toggle(void);
void LED_Time_Callback(void);
void LedTask_Func(void *parameter);

extern uint32_t Led_Cunter;


#endif

