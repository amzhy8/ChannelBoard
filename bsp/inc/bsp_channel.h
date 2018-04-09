#ifndef __BSP_CHANNEL_H__
#define __BSP_CHANNEL_H__
#include "bsp_config.h"
#include "gpio.h"




#define CHANNEL_STATUS_WORKMODE 0
#define CHANNEL_STATUS_DETECTIONMODE 1

#define RELAY_CH0_PORT GPIOB
#define RELAY_CH1_PORT GPIOB
#define RELAY_CH2_PORT GPIOB
#define RELAY_CH3_PORT GPIOB
#define RELAY_CH4_PORT GPIOB
#define RELAY_CH5_PORT RELAY_CH6_GPIO_Port
#define RELAY_CH6_PORT GPIOC
#define RELAY_CH7_PORT GPIOC
#define RELAY_DACSWITCH_PORT  GPIOC

#define RELAY_CH0_PIN RELAY_CH1_Pin
#define RELAY_CH1_PIN RELAY_CH2_Pin
#define RELAY_CH2_PIN RELAY_CH3_Pin
#define RELAY_CH3_PIN RELAY_CH4_Pin
#define RELAY_CH4_PIN RELAY_CH5_Pin
#define RELAY_CH5_PIN RELAY_CH6_Pin
#define RELAY_CH6_PIN RELAY_CH7_Pin
#define RELAY_CH7_PIN RELAY_CH8_Pin
#define RELAY_DACSWITCH_PIN DAC_SWITCH_Pin


#define RELAY_CH0 (0x00)
#define RELAY_CH1 (0x01)
#define RELAY_CH2 (0x02)
#define RELAY_CH3 (0x03)
#define RELAY_CH4 (0x04)
#define RELAY_CH5 (0x05)
#define RELAY_CH6 (0x06)
#define RELAY_CH7 (0x07)
#define RELAY_CH_ALL (0xff)

#define RELAY_ON (0x01)
#define RELAY_OFF (0x00)


typedef union{
	struct{
		uint8_t relay0:1;
		uint8_t relay1:1;
		uint8_t relay2:1;
		uint8_t relay3:1;
		uint8_t relay4:1;
		uint8_t relay5:1;
		uint8_t relay6:1;
		uint8_t relay7:1;
	}channel_bit;
	uint8_t ByteValue;
}RelayReg_Typedef;


typedef struct{
	RelayReg_Typedef RelayReg;
	

}Relay_Typedef;



extern Relay_Typedef Relay_obj;

void Channel_Server(Relay_Typedef *obj);
void Channel_Ctrl(RelayReg_Typedef value);
uint8_t Channel_Config(Relay_Typedef *obj,uint8_t id,uint8_t value);
void PrintChannelStatus(Relay_Typedef *obj);

#endif


