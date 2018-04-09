#ifndef __BSP_CONFIG_H__
#define __BSP_CONFIG_H__
#include "stm32f1xx_hal.h"

#define BSP_SUCCESS 0
#define BSP_ERROR 1




typedef struct{
	uint32_t McuSn[8];
	uint32_t BoardSn[8];
	uint16_t BoardAddress;//°å¿¨µØÖ·
}Board_Typedef;

extern Board_Typedef Board;


#endif


