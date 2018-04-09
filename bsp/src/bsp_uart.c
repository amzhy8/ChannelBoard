#include "bsp_uart.h"
#include <stdio.h>

uint8_t uart_rxdata;



//串口接收二级缓冲区
uint8_t UartRxbuf[UART_RX_BUF_LEN];


//初始化缓冲区属性 串口1buf
Uart_Fifo_Typedef UartFifo={
	.Rx_Fifo={
		.databuf=UartRxbuf,
		.buf_size=UART_RX_BUF_LEN,
		.readindex=0,
		.writeindex=0,
		.rxcount=0
	}
};



Uart_Fifo_Point_Typedef Uart_Fifo_Point=&UartFifo;


//判断缓冲区是否有数据
uint8_t Uart_IsReceivedData(Uart_Fifo_Point_Typedef fifo){
	if(fifo->Rx_Fifo.readindex!=fifo->Rx_Fifo.writeindex){
		return 0;
	}else return 1;
}



//从缓冲区取出一个数据
uint8_t Uart_ReadAByteFromFifo(Uart_Fifo_Point_Typedef fifo,uint8_t *data){
	if(fifo->Rx_Fifo.readindex!=fifo->Rx_Fifo.writeindex){
		*data=fifo->Rx_Fifo.databuf[fifo->Rx_Fifo.readindex];
		fifo->Rx_Fifo.readindex++;
		fifo->Rx_Fifo.readindex=fifo->Rx_Fifo.readindex%fifo->Rx_Fifo.buf_size;
		fifo->Rx_Fifo.rxcount--;
		return UART_SUCCESS;
	}else{
		return UART_FAIL;
	}	
}


//写入缓冲区一个数据
uint8_t Uart_WriteAByteFromFifo(Uart_Fifo_Point_Typedef fifo,uint8_t data){
	if(fifo->Rx_Fifo.readindex!=(fifo->Rx_Fifo.writeindex+1)%fifo->Rx_Fifo.buf_size){
		fifo->Rx_Fifo.databuf[fifo->Rx_Fifo.writeindex]=data;
		fifo->Rx_Fifo.writeindex=(fifo->Rx_Fifo.writeindex+1)%fifo->Rx_Fifo.buf_size;
		fifo->Rx_Fifo.rxcount++;
		return UART_SUCCESS;
	}else{
		return UART_FAIL;
	}
}


#ifdef __GNUC__  
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf  
set to 'Yes') calls __io_putchar() */  
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)  
#else  
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)  
#endif /* __GNUC__ */  

PUTCHAR_PROTOTYPE  
{  
	while(HAL_OK!=HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&ch, 1)){
	}
  return ch;  
}  



void Uart1_Send(uint8_t *buf,uint16_t len){
		while(HAL_OK!=HAL_UART_Transmit_DMA(&huart1, buf, len));
}


void Bsp_UartInit(void){
	Uart_Receive_Start(&huart1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1){
		HAL_UART_Receive_IT(huart,&Uart_Fifo_Point->Rx_Fifo.tmpdata,1);
		Uart_WriteAByteFromFifo(Uart_Fifo_Point,Uart_Fifo_Point->Rx_Fifo.tmpdata);
	}
}

//开启串口接收
void Uart_Receive_Start(UART_HandleTypeDef *huart){
	//HAL_UART_Receive_IT(huart,&uart_rxdata,1);
	HAL_UART_Receive_IT(huart,&Uart_Fifo_Point->Rx_Fifo.tmpdata,1);
	Uart_WriteAByteFromFifo(Uart_Fifo_Point,Uart_Fifo_Point->Rx_Fifo.tmpdata);
}



