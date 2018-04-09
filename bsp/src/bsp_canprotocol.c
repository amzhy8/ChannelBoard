#include "bsp_canprotocol.h"
#include "bsp_can.h"
#include "bsp_channel.h"
#include "bsp_dac_gpio.h"


static uint16_t Aframe_index=0;
static Protocol_Typedef Protocol;
AFrame_Typedef AFrame;



//Э���ʼ��
uint8_t ProtocolInit(void *frame){
	Protocol.frame=frame;//ע����Ҫ֡�������
	return PROTOCOL_SUCCESS;
}

//Э���ȡһ���ֽ�
static uint8_t ProtocolGetByte(uint8_t *data){
	
	return PROTOCOL_SUCCESS;
}


#define AFRAME_HEAD 0xa5
#define AFRAME_LEN

//���Э��֡
uint8_t CheckFrame(uint8_t type,void *frame){
	uint8_t data;
	AFrame_Typedef *aframe=(AFrame_Typedef*)frame;
	if(AFRAME==type){
		
			if(ProtocolGetByte(&data)!=PROTOCOL_SUCCESS){
			return PROTOCOL_ERROR;
		}
	
		switch(Protocol.status){
			case HEAD_CHECK:
				if(data==AFRAME_HEAD){
					aframe->head=AFRAME_HEAD;
					Protocol.status=LEN_CHECK;
				}
				break;
			case LEN_CHECK:
					aframe->len=data;
					Protocol.status=CMD_CHECK;
				break;
			case CMD_CHECK:
					aframe->cmd=data;
					Aframe_index=0;
					Protocol.status=DATA_CHECK;
				break;
			case DATA_CHECK:
					aframe->data[Aframe_index]=data;
					Aframe_index++;
					if(Aframe_index==5){
						Protocol.status=PARITY_CHECK;
					}
				break;
			case PARITY_CHECK:
				if(data==((aframe->len)^(aframe->cmd)^(aframe->data[0])^(aframe->data[1])^(aframe->data[2])^(aframe->data[3])^(aframe->data[4]))){
					aframe->parity=data;
					Protocol.status=HEAD_CHECK;
					return PROTOCOL_SUCCESS;
				}
				break;
			default:
				break;	
		}
	}
	return PROTOCOL_ERROR;
}



//���Э��֡
uint8_t CanFrameAnasyse(ProtocolType_Typedef type,uint8_t *buf,uint8_t len){
	uint32_t adc_value=0;
	uint8_t temp;
	float voltage=0;
	if(type==BFRAME){
		if(buf[0]!=0xa5)return PROTOCOL_ERROR;//���֡ͷ
		if(buf[1]!=0x04)return PROTOCOL_ERROR;//���Len�ֶ�
		//if((buf[2]!=0xff)&&(buf[2]!=GetCanAddress(&Bsp_Can_obj)))return PROTOCOL_ERROR;//����Ƿ��ǹ㲥���ͻ���Ϊ���͸��Լ����ź�
		temp=buf[1]^buf[2]^buf[3]^buf[4]^buf[5]^buf[6];
		if(buf[7]!=temp)return PROTOCOL_ERROR;//���У��
		
		//�л�����ͨ��
		Channel_Config(&Relay_obj,RELAY_CH_ALL,buf[3]);
		//�л��ź�Դ����
		Dac_Switch(buf[4]);
		//����DACֵ
		adc_value+=((uint16_t)buf[5]);
		adc_value+=(((uint16_t)buf[6])<<8);
		Dac_SendWord((uint16_t)adc_value);
		voltage=((2500-((((float)(adc_value*1000))/65536)*2.5)*2)*2);
		printf("�����ź����:%02f mv\r\n\r\n",voltage);
		return PROTOCOL_SUCCESS;
	}
	return PROTOCOL_ERROR;
}




