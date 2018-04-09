#ifndef __BSP_DAC_H__
#define __BSP_DAC_H__
#include "gpio.h"
#include "bsp_config.h"


#define DAC_SCK_PORT GPIOA
#define DAC_SCK_PIN DAC_SCK_Pin


#define DAC_MOSI_PORT GPIOA
#define DAC_MOSI_PIN DAC_MOSI_Pin

#define DAC_CS_PORT GPIOA                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
#define DAC_CS_PIN DAC_CS_Pin

#define DAC_CS(x) if(x)HAL_GPIO_WritePin(DAC_CS_PORT,DAC_CS_PIN,GPIO_PIN_SET);\
									else HAL_GPIO_WritePin(DAC_CS_PORT,DAC_CS_PIN,GPIO_PIN_RESET);
uint8_t Dac_WriteWord(uint16_t data);


void Dac_Init(void);
#endif


