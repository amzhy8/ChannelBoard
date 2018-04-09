#include "bsp_timer.h"
#include "bsp_led.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	
	if(Led_Cunter>=LED_TOGGLE_PERIOD){
		Led_Cunter=0;
		LED_Toggle();
		
	}
	

	Led_Cunter++;
	

}


