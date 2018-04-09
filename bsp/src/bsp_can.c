#include "bsp_can.h"
#include <stdio.h>
#include <string.h>
#include "bsp_config.h"
#include "gpio.h"

#include "bsp_event.h"
#include "bsp_uart.h"
#include "bsp_canprotocol.h"

Bsp_Can_Typedef Bsp_Can_obj;
uint8_t CanSendEventFlag;
uint8_t sendbuf[]="123456789";



uint8_t GetCanAddress(Bsp_Can_Typedef *obj){

	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(CAN_ADDR_BIT0_PORT, CAN_ADDR_BIT0_PIN)){
		obj->Can_Address|=(0x01<<0);
	}else{
		obj->Can_Address&=~(0x01<<0);
	}
	
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(CAN_ADDR_BIT1_PORT, CAN_ADDR_BIT1_PIN)){
		obj->Can_Address|=(0x01<<1);
	}else{
		obj->Can_Address&=~(0x01<<1);
	}
	
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(CAN_ADDR_BIT2_PORT, CAN_ADDR_BIT2_PIN)){
		obj->Can_Address|=(0x01<<2);
	}else{
		obj->Can_Address&=~(0x01<<2);
	}
	
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(CAN_ADDR_BIT3_PORT, CAN_ADDR_BIT3_PIN)){
		obj->Can_Address|=(0x01<<3);
	}else{
		obj->Can_Address&=~(0x01<<3);
	}
	
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(CAN_ADDR_BIT4_PORT, CAN_ADDR_BIT4_PIN)){
		obj->Can_Address|=(0x01<<4);
	}else{
		obj->Can_Address&=~(0x01<<4);
	}
	
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(CAN_ADDR_BIT5_PORT, CAN_ADDR_BIT5_PIN)){
		obj->Can_Address|=(0x01<<5);
	}else{
		obj->Can_Address&=~(0x01<<5);
	}
	
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(CAN_ADDR_BIT6_PORT, CAN_ADDR_BIT6_PIN)){
		obj->Can_Address|=(0x01<<6);
	}else{
		obj->Can_Address&=~(0x01<<6);
	}
	
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(CAN_ADDR_BIT7_PORT, CAN_ADDR_BIT7_PIN)){
		obj->Can_Address|=(0x01<<7);
	}else{
		obj->Can_Address&=~(0x01<<7);
	}
	return obj->Can_Address;
	
}


void CanInit(Bsp_Can_Typedef *obj,CAN_HandleTypeDef *can_ptr){
	obj->can_ptr=can_ptr;
	obj->Can_Address=GetCanAddress(obj);//��ȡCAN��ַ
	CanSendEventFlag=EVENT_FLASE;
	obj->CanIntFlag=CAN_INT_FLAG_FALSE;
	obj->can_ptr->pRxMsg=&obj->RxMessage;
	obj->can_ptr->pTxMsg=&obj->TxMessage;
	
	//���Ͳ�������
//	obj->can_ptr->pTxMsg->StdId=obj->Can_Address;
//	obj->can_ptr->pTxMsg->RTR=CAN_RTR_DATA;
//	obj->can_ptr->pTxMsg->IDE=CAN_ID_STD;
//	obj->can_ptr->pTxMsg->DLC = 8;
	obj->can_ptr->pTxMsg->StdId=0x123;
	obj->can_ptr->pTxMsg->RTR=CAN_RTR_DATA;
	obj->can_ptr->pTxMsg->IDE=CAN_ID_STD;
	obj->can_ptr->pTxMsg->DLC = 8;
	
	//���չ�����
	obj->sFilterConfig.FilterNumber=0;
	obj->sFilterConfig.FilterMode=CAN_FILTERMODE_IDMASK;
	obj->sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT;
	obj->sFilterConfig.FilterIdHigh=0x0000;
	obj->sFilterConfig.FilterIdLow=0x0000;
	obj->sFilterConfig.FilterMaskIdHigh=0x0000;
	obj->sFilterConfig.FilterMaskIdLow=0x0000;
	obj->sFilterConfig.FilterFIFOAssignment=0;
	obj->sFilterConfig.FilterActivation=ENABLE;
	obj->sFilterConfig.BankNumber=14;
	HAL_CAN_ConfigFilter(&hcan, &Bsp_Can_obj.sFilterConfig);
	
	HAL_CAN_Receive_IT(&hcan, CAN_FIFO0); //��CAN�����ж�
}



//��������
uint8_t Can_Send(Bsp_Can_Typedef *obj,uint16_t id,uint8_t *buf,uint8_t len){
	uint16_t i;
	if(len>8) {
		printf("Error:Can_Send argument len is bigger than 8\r\n");
		return BSP_CAN_FALSE;
	}
	obj->TxMessage.StdId=id;	//��׼��ʶ��
	obj->TxMessage.ExtId=id;	//��չ��ʶ��
	obj->TxMessage.IDE=CAN_ID_STD;	//ʹ�ñ�׼֡
	obj->TxMessage.RTR=CAN_RTR_DATA;	//����֡
	obj->TxMessage.DLC=len;	//���ݳ���
	for(i=0;i<len;i++)
	obj->TxMessage.Data[i]=buf[i];
	if(HAL_CAN_Transmit(obj->can_ptr,10)!=HAL_OK) return BSP_CAN_FALSE; //
			return BSP_CAN_SUCCESS;
}





//��������
uint8_t Can_Receive(Bsp_Can_Typedef *obj,uint8_t *buf,uint8_t *len){
	uint8_t i;
	if(HAL_CAN_Receive(obj->can_ptr,CAN_FIFO0,0)!=HAL_OK) return BSP_CAN_FALSE;//��FIFO�ж�ȡ����
	for(i=0;i<obj->RxMessage.DLC;i++)
	buf[i]=obj->RxMessage.Data[i];
	*len=obj->RxMessage.DLC;
	return BSP_CAN_SUCCESS;
}

void Can_ItCallbackFunc(Bsp_Can_Typedef *obj){
	if ((obj->can_ptr->pRxMsg->StdId == CAN_ADDR_CAST)&&(obj->can_ptr->pRxMsg->IDE == CAN_ID_STD) && (obj->can_ptr->pRxMsg->DLC == 8))
  {
		Evnet_Set(&Event_obj,CAN_CALLME_EVENT,obj->can_ptr->pRxMsg->Data,obj->can_ptr->pRxMsg->DLC);//����Can�¼�
	}
	
	if ((obj->can_ptr->pRxMsg->StdId == obj->Can_Address)&&(obj->can_ptr->pRxMsg->IDE == CAN_ID_STD) && (obj->can_ptr->pRxMsg->DLC == 8))
  {
		Evnet_Set(&Event_obj,CAN_CAST_EVENT,obj->can_ptr->pRxMsg->Data,obj->can_ptr->pRxMsg->DLC);//����Can�¼�
  }
  
  /* Receive */
  if(HAL_CAN_Receive_IT(obj->can_ptr, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }
}


//CAN�жϻص�����
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
	//Bsp_Can_obj.CanIntFlag=CAN_INT_FLAG_TRUE;
	Can_ItCallbackFunc(&Bsp_Can_obj);
}




uint8_t Can_Server(void){
	uint8_t buf[50];
	uint8_t len;
	uint8_t i;
	if(BSP_SUCCESS==Event_Check(&Event_obj,CAN_CAST_EVENT,buf,&len)){//���㲥�¼�
		//for(i=0;i<len;i++){
			//Uart1_Send(&buf[i],1);
		//}
		if(CanFrameAnasyse(BFRAME,buf,len)){
			printf("Receive Can CMD\r\n");
			printf("BoardAddr:%x\r\n",buf[2]);
			printf("Channel:%x\r\n",buf[3]);
		}
	}
	
	if(BSP_SUCCESS==Event_Check(&Event_obj,CAN_CALLME_EVENT,buf,&len)){//��ⵥ���¼�
		//for(i=0;i<len;i++){
		//	Uart1_Send(&buf[i],1);
		//}
		if(CanFrameAnasyse(BFRAME,buf,len)){
			printf("Receive Can CMD\r\n");
		}
	}
	/*
	if(Can_Send(&Bsp_Can_obj,0x123,sendbuf,4)==BSP_CAN_SUCCESS){
		printf("Can Send Success!\r\n");
	}
	*/

	
	return BSP_CAN_SUCCESS;
}




