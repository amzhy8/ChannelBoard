#include "bsp_channel.h"
Relay_Typedef Relay_obj;


void Channel_Ctrl(RelayReg_Typedef value){
	if(value.channel_bit.relay0){
		HAL_GPIO_WritePin(RELAY_CH0_PORT,RELAY_CH0_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(RELAY_CH0_PORT,RELAY_CH0_PIN,GPIO_PIN_RESET);
	}
	
	if(value.channel_bit.relay1){
		HAL_GPIO_WritePin(RELAY_CH1_PORT,RELAY_CH1_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(RELAY_CH1_PORT,RELAY_CH1_PIN,GPIO_PIN_RESET);
	}
	
	if(value.channel_bit.relay2){
		HAL_GPIO_WritePin(RELAY_CH2_PORT,RELAY_CH2_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(RELAY_CH2_PORT,RELAY_CH2_PIN,GPIO_PIN_RESET);
	}
	
	if(value.channel_bit.relay3){
		HAL_GPIO_WritePin(RELAY_CH3_PORT,RELAY_CH3_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(RELAY_CH3_PORT,RELAY_CH3_PIN,GPIO_PIN_RESET);
	}
	
	if(value.channel_bit.relay4){
		HAL_GPIO_WritePin(RELAY_CH4_PORT,RELAY_CH4_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(RELAY_CH4_PORT,RELAY_CH4_PIN,GPIO_PIN_RESET);
	}
	
	if(value.channel_bit.relay5){
		HAL_GPIO_WritePin(RELAY_CH5_PORT,RELAY_CH5_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(RELAY_CH5_PORT,RELAY_CH5_PIN,GPIO_PIN_RESET);
	}
	
	if(value.channel_bit.relay6){
		HAL_GPIO_WritePin(RELAY_CH6_PORT,RELAY_CH6_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(RELAY_CH6_PORT,RELAY_CH6_PIN,GPIO_PIN_RESET);
	}
	
	if(value.channel_bit.relay7){
		HAL_GPIO_WritePin(RELAY_CH7_PORT,RELAY_CH7_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(RELAY_CH7_PORT,RELAY_CH7_PIN,GPIO_PIN_RESET);
	}
}


uint8_t Channel_Config(Relay_Typedef *obj,uint8_t id,uint8_t value){
	
	if(id==RELAY_CH_ALL){
		obj->RelayReg.ByteValue=value;
		Channel_Ctrl(obj->RelayReg);
		PrintChannelStatus(&Relay_obj);
		return BSP_SUCCESS;
	}
	
	if(value>0x07)return BSP_ERROR;
	
	if(id==RELAY_CH0){
		obj->RelayReg.channel_bit.relay0=value;
	}
	
	if(id==RELAY_CH1){
		obj->RelayReg.channel_bit.relay1=value;
	}
	
	if(id==RELAY_CH2){
		obj->RelayReg.channel_bit.relay2=value;
	}
	
	if(id==RELAY_CH3){
		obj->RelayReg.channel_bit.relay3=value;
	}
	
	if(id==RELAY_CH4){
		obj->RelayReg.channel_bit.relay4=value;
	}
	
	if(id==RELAY_CH5){
		obj->RelayReg.channel_bit.relay5=value;
	}
	
	if(id==RELAY_CH6){
		obj->RelayReg.channel_bit.relay6=value;
	}
	
	if(id==RELAY_CH7){
		obj->RelayReg.channel_bit.relay7=value;
	}
	Channel_Ctrl(obj->RelayReg);
	PrintChannelStatus(&Relay_obj);
	return BSP_SUCCESS;
}



void PrintChannelStatus(Relay_Typedef *obj){
	uint8_t i=0;
	printf("通道状态:\r\n");
	if(obj->RelayReg.channel_bit.relay0==CHANNEL_STATUS_DETECTIONMODE){
		printf("0:检测模式\r\n");
	}else if(obj->RelayReg.channel_bit.relay0==CHANNEL_STATUS_WORKMODE){
		printf("0:工作模式\r\n");
	}
	if(obj->RelayReg.channel_bit.relay1==CHANNEL_STATUS_DETECTIONMODE){
		printf("1:检测模式\r\n");
	}else if(obj->RelayReg.channel_bit.relay1==CHANNEL_STATUS_WORKMODE){
		printf("1:工作模式\r\n");
	}
	if(obj->RelayReg.channel_bit.relay2==CHANNEL_STATUS_DETECTIONMODE){
		printf("2:检测模式\r\n");
	}else if(obj->RelayReg.channel_bit.relay2==CHANNEL_STATUS_WORKMODE){
		printf("2:工作模式\r\n");
	}
	if(obj->RelayReg.channel_bit.relay3==CHANNEL_STATUS_DETECTIONMODE){
		printf("3:检测模式\r\n");
	}else if(obj->RelayReg.channel_bit.relay3==CHANNEL_STATUS_WORKMODE){
		printf("3:工作模式\r\n");
	}
	if(obj->RelayReg.channel_bit.relay4==CHANNEL_STATUS_DETECTIONMODE){
		printf("4:检测模式\r\n");
	}else if(obj->RelayReg.channel_bit.relay4==CHANNEL_STATUS_WORKMODE){
		printf("4:工作模式\r\n");
	}
	if(obj->RelayReg.channel_bit.relay5==CHANNEL_STATUS_DETECTIONMODE){
		printf("5:检测模式\r\n");
	}else if(obj->RelayReg.channel_bit.relay5==CHANNEL_STATUS_WORKMODE){
		printf("5:工作模式\r\n");
	}
	if(obj->RelayReg.channel_bit.relay6==CHANNEL_STATUS_DETECTIONMODE){
		printf("6:检测模式\r\n");
	}else if(obj->RelayReg.channel_bit.relay6==CHANNEL_STATUS_WORKMODE){
		printf("6:工作模式\r\n");
	}
	if(obj->RelayReg.channel_bit.relay7==CHANNEL_STATUS_DETECTIONMODE){
		printf("7:检测模式\r\n");
	}else if(obj->RelayReg.channel_bit.relay7==CHANNEL_STATUS_WORKMODE){
		printf("7:工作模式\r\n");
	}
}


uint8_t SingleChannel_Ctrl(Relay_Typedef *obj,uint8_t id,uint8_t value){
	if(value>7){
		return BSP_ERROR;
	}


return BSP_SUCCESS;
}

