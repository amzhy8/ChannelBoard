#include "bsp_usart_protocol.h"
#include "bsp_uart.h"
#include <string.h>

UsartProtocol_Typedef UsartProtocol;


//初始化

int UsartProtocol_Init(UsartProtocol_Typedef *obj){

	//注册获取一个字符的输入函数
	obj->GetByteFunc=NULL;
	
	obj->index=0;
	obj->states=USART_PROTOCOL_HEAD_STATE;
	
	return USART_PROTOCOL_SUCCESS;
}



uint8_t  UsartProtocol_PickFrame(UsartProtocol_Typedef *obj,uint8_t *buf,uint8_t *len){

	uint16_t i;
	__IO uint8_t data=0;

	if(UART_SUCCESS==Uart_ReadAByteFromFifo(Uart_Fifo_Point,(uint8_t *)&data)){
			switch(obj->states){
				case USART_PROTOCOL_HEAD_STATE:
					if(data==0xA5){
						obj->buf[USART_PROTOCOL_HEAD_INDEX]=data;
						obj->states=USART_PROTOCOL_LEN_STATE;
					}else{
						return USART_PROTOCOL_FALSE;
					}
					break;
				
					
					case USART_PROTOCOL_LEN_STATE:
						obj->buf[USART_PROTOCOL_LEN_INDEX]=data;
						obj->len=data;
						obj->states=USART_PROTOCOL_CMD_STATE;
					break;
					
					case USART_PROTOCOL_CMD_STATE:
						obj->buf[USART_PROTOCOL_CMD_INDEX]=data;
						if(obj->len==0){
							obj->states=USART_PROTOCOL_XOR_STATE;
						}else{
							obj->index=0;
							obj->states=USART_PROTOCOL_DATA_STATE;
						}
						
					break;
					
					case USART_PROTOCOL_DATA_STATE:
						obj->buf[USART_PROTOCOL_DATA_INDEX+obj->index]=data;
						obj->index++;
						if(obj->index==obj->len){
							obj->index=0;
							obj->states=USART_PROTOCOL_XOR_STATE;
						}
					break;
						
						case USART_PROTOCOL_XOR_STATE:
							obj->buf[USART_PROTOCOL_DATA_INDEX+obj->len]=0;
							for(i=0;i<obj->len+3;i++){
								obj->buf[USART_PROTOCOL_DATA_INDEX+obj->len]^=obj->buf[i];
							}
							if(data==obj->buf[USART_PROTOCOL_DATA_INDEX+obj->len]){
								obj->states=USART_PROTOCOL_TAIL_STATE;
								break;
							}else{
								obj->states=USART_PROTOCOL_HEAD_STATE;
							}
							
					break;
					
					
					case USART_PROTOCOL_TAIL_STATE:
							if(data==USART_PROTOCOL_TAIL_VALUE){
								obj->buf[USART_PROTOCOL_DATA_STATE+obj->buf[USART_PROTOCOL_LEN_INDEX]+1]=data;
								obj->states=USART_PROTOCOL_HEAD_STATE;
								
								memcpy(buf,obj->buf,5+obj->len);
								*len=5+obj->len;
								
								return USART_PROTOCOL_SUCCESS;
							}else{
								obj->states=USART_PROTOCOL_HEAD_STATE;
								return USART_PROTOCOL_FALSE;
							}
					
				
				default:break;
			}
			return USART_PROTOCOL_FALSE;
	}
	return USART_PROTOCOL_FALSE;
}

