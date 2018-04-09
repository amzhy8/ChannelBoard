#ifndef __BSP_DAC_GPIO_H__
#define __BSP_DAC_GPIO_H__
#include "gpio.h"

#define DAC_SCK_PORT GPIOA
#define DAC_SCK_PIN GPIO_PIN_5


#define DAC_MOSI_PORT GPIOA
#define DAC_MOSI_PIN GPIO_PIN_7

#define DAC_CS_PORT GPIOA                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
#define DAC_CS_PIN GPIO_PIN_4

#define DAC_INTER_TYPE 0x00
#define DAC_EXTER_TYPE 0x01

#define DAC_CS(x) if(x)HAL_GPIO_WritePin(DAC_CS_PORT,DAC_CS_PIN,GPIO_PIN_SET);\
									else HAL_GPIO_WritePin(DAC_CS_PORT,DAC_CS_PIN,GPIO_PIN_RESET)


#define DAC_SCK(x) if(x)HAL_GPIO_WritePin(DAC_SCK_PORT,DAC_SCK_PIN,GPIO_PIN_SET);\
									else HAL_GPIO_WritePin(DAC_SCK_PORT,DAC_SCK_PIN,GPIO_PIN_RESET)
									
#define DAC_MOSI(x) if(x)HAL_GPIO_WritePin(DAC_MOSI_PORT,DAC_MOSI_PIN,GPIO_PIN_SET);\
									else HAL_GPIO_WritePin(DAC_MOSI_PORT,DAC_MOSI_PIN,GPIO_PIN_RESET)									
									
#define DAC_SWITCH(x) if(x)HAL_GPIO_WritePin(GPIOC,DAC_SWITCH_Pin,GPIO_PIN_SET);\
									else HAL_GPIO_WritePin(GPIOC,DAC_SWITCH_Pin,GPIO_PIN_RESET)										
								
									
static void DacDelay(uint32_t n);								
void Dac_Init(void);
void Dac_SendWord(uint16_t data);
void Dac_Switch(uint8_t status);
#endif

