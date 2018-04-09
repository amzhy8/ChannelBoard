#ifndef __BSP_USART_PROTOCOL_H__
#define __BSP_USART_PROTOCOL_H__

#include "stm32f1xx_hal.h"


#define USART_PROTOCOL_BUF_LEN 128

#define USART_PROTOCOL_HEAD_VALUE 0xA5
#define USART_PROTOCOL_TAIL_VALUE 0x5A


#define USART_PROTOCOL_HEAD_STATE 0
#define USART_PROTOCOL_HEAD_INDEX 0
#define USART_PROTOCOL_HEAD_LEN 1


#define USART_PROTOCOL_LEN_STATE 1
#define USART_PROTOCOL_LEN_INDEX (USART_PROTOCOL_HEAD_INDEX+USART_PROTOCOL_HEAD_LEN)
#define USART_PROTOCOL_LEN_LEN 1


#define USART_PROTOCOL_CMD_STATE 2
#define USART_PROTOCOL_CMD_INDEX (USART_PROTOCOL_LEN_INDEX+USART_PROTOCOL_LEN_LEN)
#define USART_PROTOCOL_CMD_LEN 1

#define USART_PROTOCOL_DATA_STATE 3
#define USART_PROTOCOL_DATA_INDEX (USART_PROTOCOL_CMD_INDEX+USART_PROTOCOL_CMD_LEN)

#define USART_PROTOCOL_XOR_STATE 4
#define USART_PROTOCOL_TAIL_STATE 5


#define USART_PROTOCOL_SUCCESS 0
#define USART_PROTOCOL_FALSE 1


typedef int (*IntVoidFunc)(void);

typedef struct{
	//接收一个字节的函数注册
	IntVoidFunc GetByteFunc;
	//分离帧函数
	
	//解析帧函数
	
	//状态记录
	uint8_t states;
	//临时指针记录
	uint8_t index;
	uint8_t len;
	//帧buf
	uint8_t buf[USART_PROTOCOL_BUF_LEN];


}UsartProtocol_Typedef;


extern UsartProtocol_Typedef UsartProtocol;


uint8_t  UsartProtocol_PickFrame(UsartProtocol_Typedef *obj,uint8_t *buf,uint8_t *len);

#endif

