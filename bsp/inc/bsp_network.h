#ifndef __BSP_NETWORK_H__
#define __BSP_NETWORK_H__
#include "gpio.h"
#include "socket.h"
#include "cmsis_os.h"


#define W5500_RST_GPIO_PORT GPIOB
#define W5500_RST_GPIO_PIN GPIO_PIN_0

#define W5500_EINT_GPIO_PORT GPIOB
#define W5500_EINT_GPIO_PIN GPIO_PIN_1

#define NETWORK_FIFO_LEN 2048

#define NETWORK_SUCCESS 0
#define NETWORK_FALSE 1

#define SOCK_DHCP             0
#define SOCK_HTTP             1							 

#define W5500_ON	                 	1
#define W5500_OFF	                 	0
#define W5500_HIGH	           	 		1
#define W5500_LOW		             		0

#define CONFIG_MSG_LEN        sizeof(CONFIG_MSG) - 4 // the 4 bytes OP will not save to EEPROM

#define W5500_MAX_BUF_SIZE		 			1460
#define W5500_KEEP_ALIVE_TIME	     	30	// 30sec
// SRAM address range is 0x2000 0000 ~ 0x2000 BFFF (48KB)
#define SOCK_BUF_ADDR 	      0x20000000
#define AppBackAddress        0x08020000 //from 128K
#define ConfigAddr		      	0x0800FC00

#define NORMAL_STATE          0
#define NEW_APP_IN_BACK       1 //there is new app in back address
#define CONFIGTOOL_FW_UP      2 //configtool update f/w in app

#define FW_VER_HIGH  		1
#define FW_VER_LOW    	0




typedef struct{
//����״̬�ı��־
	uint8_t pre_status;
	uint8_t cur_status;
}PreCur_Status_Typedef;







//#pragma pack(1)
typedef struct _CONFIG_MSG
{
  uint8_t op[4];//header: FIND;SETT;FACT...
  uint8_t sw_ver[2];
  uint8_t debug;
  uint8_t fw_len;
  uint8_t state;
	
	
	wiz_NetInfo netinfo;
	
	PreCur_Status_Typedef link_status;
	PreCur_Status_Typedef socket_stauts;
  
}W5500_Conifg_Typedef;
//#pragma pack()

/*
typedef enum
{
   LinkChanged,
	 LinkChanged
}W5500_LinkChange_type;
*/




extern W5500_Conifg_Typedef W5500_Conifg;
extern xQueueHandle NetWorkRecieveFifo;
extern uint8_t buffer[2048];	

#define USART_REC_LEN  		200  							// ���崮�ڽ��ջ����С
#define EN_USART1_RX 			1									// ʹ�ܴ���1���գ�0Ϊ��ֹ


void Reset_W5500(void);
uint8_t NetWork_LinkIsChanged(W5500_Conifg_Typedef *obj,uint8_t *curentstatus);
uint8_t NetWorkModuleInit(void);

#endif

