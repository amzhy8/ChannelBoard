#ifndef _BSP_CAN_H__
#define _BSP_CAN_H__
#include "can.h"


#define BSP_CAN_FALSE 1
#define BSP_CAN_SUCCESS 0


#define CAN_INT_FLAG_TRUE 1
#define CAN_INT_FLAG_FALSE 0


#define EVENT_TURE 1
#define EVENT_FLASE 0



#define CAN_ADDR_BIT7_PORT ADDR_0_GPIO_Port
#define CAN_ADDR_BIT6_PORT GPIOC
#define CAN_ADDR_BIT5_PORT GPIOC
#define CAN_ADDR_BIT4_PORT GPIOC
#define CAN_ADDR_BIT3_PORT GPIOC
#define CAN_ADDR_BIT2_PORT GPIOB
#define CAN_ADDR_BIT1_PORT GPIOB
#define CAN_ADDR_BIT0_PORT GPIOB

#define CAN_ADDR_BIT7_PIN ADDR_0_Pin
#define CAN_ADDR_BIT6_PIN ADDR_1_Pin
#define CAN_ADDR_BIT5_PIN ADDR_2_Pin
#define CAN_ADDR_BIT4_PIN ADDR_3_Pin
#define CAN_ADDR_BIT3_PIN ADDR_4_Pin
#define CAN_ADDR_BIT2_PIN ADDR_5_Pin
#define CAN_ADDR_BIT1_PIN ADDR_6_Pin
#define CAN_ADDR_BIT0_PIN ADDR_7_Pin


#define CAN_ADDR_CAST 0x00000700



typedef struct{
	CAN_HandleTypeDef *can_ptr;
	CAN_FilterConfTypeDef  				sFilterConfig;
	CanTxMsgTypeDef        TxMessage;
	CanRxMsgTypeDef        RxMessage;
	uint32_t Can_Address;
	uint8_t CanIntFlag;
}Bsp_Can_Typedef;



extern Bsp_Can_Typedef Bsp_Can_obj;
extern uint8_t CanSendEventFlag;

void CanInit(Bsp_Can_Typedef *obj,CAN_HandleTypeDef *can_ptr);
uint8_t Can_Send(Bsp_Can_Typedef *obj,uint16_t id,uint8_t *buf,uint8_t len);
uint8_t Can_Receive(Bsp_Can_Typedef *obj,uint8_t *buf,uint8_t *len);
uint8_t Can_Server(void);
void Can_send_test(void);


void Can_ItCallbackFunc(Bsp_Can_Typedef *obj);
uint8_t GetCanAddress(Bsp_Can_Typedef *obj);
#endif

