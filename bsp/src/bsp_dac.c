#include "bsp_dac.h"


void Dac_Init(void){
	DAC_CS(0);
	DAC_CS(1);
}


uint8_t Dac_WriteWord(uint16_t data){
	uint8_t buf[3];
	buf[0]=0x00;
	buf[1]=(uint8_t)((data>>8)&0x00ff);
	buf[2]=(uint8_t)((data)&0x00ff);
	DAC_CS(0);
	HAL_Delay(1);
	HAL_SPI_Transmit(&hspi1, buf, 3, 10);
	HAL_Delay(1);
	DAC_CS(1);
return BSP_SUCCESS;
}
