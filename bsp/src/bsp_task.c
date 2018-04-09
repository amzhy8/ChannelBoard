#include "bsp_task.h"


TaskHandle_t LedTask_Handler;
TaskHandle_t CanTask_Handler;
TaskHandle_t UartTask_Handler;



void LedTask_Func(void *parameter){
	while(1){
		LED_Toggle();
		vTaskDelay(1000);
		Can_send_test();
	}
}





void CanTask_Func(void *parameter){
	while(1){
		Can_Server();
		vTaskDelay(10);
	}
}

/*

void NetWorkTask_Func(void *parameter){
	while(1){
		vTaskDelay(1000);
	}
}
*/

