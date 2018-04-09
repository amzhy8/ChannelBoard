#ifndef _BSP_UART_H__
#define _BSP_UART_H__

#include "usart.h"


#define UART_SUCCESS 0
#define UART_FAIL 1

#define UART_RECIEVE_FIFO_LEN 256

//使能串口接收二级缓冲器
#define UART_RX_FIFO_EN 1
#define UART_RX_BUF_LEN 256

typedef struct{
	uint8_t *databuf;
	uint16_t buf_size;
	uint16_t readindex;
	uint16_t writeindex;
	uint16_t rxcount;
	uint8_t tmpdata;
}Fifo_Typdef;




typedef struct{
	#ifdef UART_RX_FIFO_EN
		Fifo_Typdef Rx_Fifo;
	#endif
	#ifdef UART_RX_FIFO_EN
		Fifo_Typdef Tx_Fifo;
	#endif
}Uart_Fifo_Typedef;

typedef Uart_Fifo_Typedef* Uart_Fifo_Point_Typedef;
extern Uart_Fifo_Typedef Uart_Fifo;
extern uint8_t UartRxbuf[UART_RX_BUF_LEN];
extern Uart_Fifo_Point_Typedef Uart_Fifo_Point;


void Bsp_UartInit(void);
void Uart1_Send(uint8_t *buf,uint16_t len);
void Uart_Receive_Start(UART_HandleTypeDef *huart);
void UartTask_Func(void *parameter);
void Bsp_UartInit(void);


uint8_t Uart_ReadAByteFromFifo(Uart_Fifo_Point_Typedef fifo,uint8_t *data);
uint8_t Uart_WriteAByteFromFifo(Uart_Fifo_Point_Typedef fifo,uint8_t data);
uint8_t Uart_IsReceivedData(Uart_Fifo_Point_Typedef fifo);

extern uint8_t uart_rxdata;

#endif

