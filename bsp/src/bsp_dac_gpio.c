#include "bsp_dac_gpio.h"
#include "bsp_config.h"



static void DacDelay(uint32_t n){
	uint32_t i;
	for(i=0;i<n;i++);
}


void Dac_Init(void){
	DAC_CS(1);
	HAL_Delay(1);
	DAC_SCK(1);
	HAL_Delay(1);
	DAC_MOSI(0);
	HAL_Delay(1);
	DAC_CS(0);
	HAL_Delay(1);

}

void Dac_SendWord(uint16_t data){
	uint32_t temp_data=0;
	uint8_t i;
	DAC_CS(1);
	DacDelay(100);
	DAC_CS(0);
	DacDelay(10);
	
	for(i=0;i<8;i++){
		DAC_SCK(1);
		DacDelay(10);
		DAC_MOSI(0);
		DacDelay(10);
		DAC_SCK(0);
		DacDelay(10);
	}
	
	temp_data=data;
	for(i=0;i<16;i++){
		DAC_SCK(1);
		DacDelay(10);
		if((temp_data<<i)&0x8000){
			DAC_MOSI(1);
		}else{
			DAC_MOSI(0);
		}
		DacDelay(10);
		DAC_SCK(0);
		DacDelay(10);
	}
	
	/*
	for(i=0;i<24;i++){
		DAC_SCK(1);
		DacDelay(10);
		if((temp_data<<i)&0x80000000){
			DAC_MOSI(1);
		}else{
			DAC_MOSI(0);
		}
		HAL_Delay(1);
		DAC_SCK(0);
		HAL_Delay(1);
		//DAC_SCK(1);
		//DacDelay(10);
		//temp_data<<=1;
	}*/
	//DAC_CS(1);
	DacDelay(10);
}



void Dac_Switch(uint8_t status){
	if(status==DAC_INTER_TYPE){
		HAL_GPIO_WritePin(GPIOC,DAC_SWITCH_Pin,GPIO_PIN_SET);
		printf("信号类型:内置信号发生器\r\n");
	}else{
		HAL_GPIO_WritePin(GPIOC,DAC_SWITCH_Pin,GPIO_PIN_RESET);	
		printf("信号类型:外置信号发生器\r\n");		
	} 
}
