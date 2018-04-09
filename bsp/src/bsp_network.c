#include "bsp_network.h"
#include <string.h>
#include "bsp_uart.h"

W5500_Conifg_Typedef W5500_Conifg;
xQueueHandle NetWorkRecieveFifo;
uint8_t USART_RX_BUF[USART_REC_LEN];     // 定义一个用于串口数据接收的缓存，大小为USART_REC_LEN
uint16_t USART_RX_STA=0;       					// 定义并初始化串口接收标志位

uint8_t buffer[NETWORK_FIFO_LEN];		
void Reset_W5500(void)
{
	HAL_GPIO_WritePin(W5500_RST_GPIO_PORT,W5500_RST_GPIO_PIN,GPIO_PIN_RESET);
	vTaskDelay(1);
	HAL_GPIO_WritePin(W5500_RST_GPIO_PORT,W5500_RST_GPIO_PIN,GPIO_PIN_SET);
  vTaskDelay(2);
}




uint8_t NetWorkModuleInit(){
	//创建网络模块中的接收FIFO
	NetWorkRecieveFifo=xQueueCreate(NETWORK_FIFO_LEN,sizeof(uint8_t));
	
	

	return NETWORK_SUCCESS;
}


//检查网线连接状态
uint8_t NetWork_LinkIsChanged(W5500_Conifg_Typedef *obj,uint8_t *curentstatus){
	if(0==ctlwizchip(CW_GET_PHYLINK, curentstatus)){
		obj->link_status.pre_status=obj->link_status.cur_status;
		obj->link_status.cur_status=*curentstatus;
		if(obj->link_status.cur_status!=obj->link_status.pre_status){
			return NETWORK_SUCCESS;
		}else{
			return NETWORK_FALSE;
		}
	}
	return NETWORK_FALSE;
}


//检查网络连接状态
uint8_t NetWork_SocketStatusIsChanged(W5500_Conifg_Typedef *obj){
	uint8_t status=getSn_SR(0);

		obj->socket_stauts.pre_status=obj->socket_stauts.cur_status;
		obj->socket_stauts.cur_status=status;
		if(obj->socket_stauts.cur_status!=obj->socket_stauts.pre_status){
					switch(status){
				case SOCK_INIT:
					printf("Socket Status:SOCK_INIT\r\n");
					break;
				case SOCK_LISTEN:
					printf("Socket Status:SOCK_LISTEN\r\n");
					break;
				case SOCK_ESTABLISHED:
					printf("Socket Status:SOCK_ESTABLISHED\r\n");
					break;
				case SOCK_CLOSE_WAIT:
					printf("Socket Status:SOCK_CLOSE_WAIT\r\n");
					break;
				case SOCK_CLOSED:
					printf("Socket Status:SOCK_CLOSED\r\n");
					break;
				case SOCK_SYNSENT:
					printf("Socket Status:SOCK_SYNSENT\r\n");
					break;
				case SOCK_SYNRECV:
					printf("Socket Status:SOCK_SYNRECV\r\n");
					break;
				case SOCK_FIN_WAIT:
					printf("Socket Status:SOCK_FIN_WAIT\r\n");
					break;
				
				case SOCK_CLOSING:
					printf("Socket Status:SOCK_CLOSING\r\n");
					break;
				
				case SOCK_TIME_WAIT:
					printf("Socket Status:SOCK_TIME_WAIT\r\n");
					break;		
				case SOCK_LAST_ACK:
					printf("Socket Status:SOCK_LAST_ACK\r\n");
					break;
				case SOCK_UDP:
					printf("Socket Status:SOCK_UDP\r\n");
					break;
				case SOCK_IPRAW:
					printf("Socket Status:SOCK_IPRAW\r\n");
					break;
				default:break;
			}
	}
			
			

	return NETWORK_SUCCESS;
}




// 设置默认MAC、IP、GW、SUB、DNS
void set_default(void)	
{  
	
	wiz_NetInfo netinfo={
		.mac={0x00,0x08,0xdc,0x11,0x11,0x11},
		.ip={192,168,16,150},
		.sn={255,255,255,0},
		.gw={192,168,16,254},
		.dns={8,8,8,8},
		.dhcp=NETINFO_STATIC
	};
	memcpy(&W5500_Conifg.netinfo, &netinfo, sizeof(wiz_NetInfo));
	
  W5500_Conifg.debug=1;
  W5500_Conifg.fw_len=0;
  
  W5500_Conifg.state=NORMAL_STATE;
  W5500_Conifg.sw_ver[0]=FW_VER_HIGH;
  W5500_Conifg.sw_ver[1]=FW_VER_LOW;
}
// 配置初始化IP信息并打印，初始化8个Socket
void set_network(void)														
{
	uint8_t link;
  uint8_t ip[4];
	/*
	wiz_PhyConf phyConf={
		.by=PHY_CONFBY_SW,       ///< set by @ref PHY_CONFBY_HW or @ref PHY_CONFBY_SW
    .mode=PHY_MODE_MANUAL,     ///< set by @ref PHY_MODE_MANUAL or @ref PHY_MODE_AUTONEGO
    .speed=PHY_SPEED_100,    ///< set by @ref PHY_SPEED_10 or @ref PHY_SPEED_100
    .duplex=PHY_DUPLEX_FULL  ///< set by @ref PHY_DUPLEX_HALF @ref PHY_DUPLEX_FULL 
	};
	*/
	ctlnetwork(CN_SET_NETINFO, &W5500_Conifg.netinfo);//设置网络参数
	vTaskDelay(10);

	//ctlwizchip(CW_SET_PHYCONF, &phyConf);//设置PHY
	vTaskDelay(1000);
	//获取网线连接情况
	if(0==ctlwizchip(CW_GET_PHYLINK, &link)){
		if(link==PHY_LINK_ON){
			printf("网线已连接！\r\n");
		}else{
			printf("网线未连接！\r\n");
		}
	}
//set rx tx buf 2k
	wizchip_init(NULL,NULL);
  
  setRTR(2000);																		// 设置重传超时时间
  setRCR(3);																			// 设置最大重新发送次数
  
	getSIPR (ip);
  printf("IP : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  getSUBR(ip);
  printf("SN : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  getGAR(ip);
  printf("GW : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]); 
}

void NetWorkTask_Func(void *parameter){
	uint16_t local_port=9001;	
	uint16_t len=0; 
	uint16_t  reallen;
	uint8_t i;
	uint8_t linkstatus;
	uint8_t data;
	
	Reset_W5500();//复位W5500
	vTaskDelay(10);
	set_default();
	vTaskDelay(10);
	set_network();
	vTaskDelay(10);
	//setkeepalive(0);
	vTaskDelay(10);
	printf("TCP Server Port: %d\r\n",local_port);
	printf("W5500 Init Complete!\r\n");
  printf("Start TCP Server Test!\r\n"); 
	
	while(1){
		
		switch(getSn_SR(0))										// 获取socket0的状态
			{ 
				case SOCK_INIT:											// Socket处于初始化完成(打开)状态
						listen(0);											// 监听刚刚打开的本地端口，等待客户端连接
				break;
				case SOCK_LISTEN:
				break;
				case SOCK_ESTABLISHED:							// Socket处于连接建立状态
						if(getSn_IR(0) & Sn_IR_CON)			
						{
							setSn_IR(0, Sn_IR_CON);				// Sn_IR的CON位置1，通知W5500连接已建立
						}
						// 数据回环测试程序：数据从上位机客户端发给W5500，W5500接收到数据后再回给客户端
						len=getSn_RX_RSR(0);						// 读取W5500空闲接收缓存寄存器的值并赋给len，Sn_RX_RSR表示接收缓存中已接收和保存的数据大小
						if(len>0)
						{
							reallen=recv(0,buffer,len);						// W5500接收来自客户端的数据，并通过SPI发送给MCU
							//将数据存入队列
							for(i=0;i<reallen;i++){
								xQueueSend(NetWorkRecieveFifo,&buffer[i],0);
							}
							reallen=0;
							printf("%s\r\n",buffer);			// 串口打印接收到的数据
							//send(0,buffer,len);						// 接收到数据后再回给客户端，完成数据回环
						}
						// W5500从串口发数据给客户端程序，数据需以回车结束
						if(USART_RX_STA & 0x8000)				// 判断串口数据是否接收完成
						{					   
							len=USART_RX_STA & 0x3fff;		// 获取串口接收到数据的长度
							send(0,USART_RX_BUF,len);			// W5500向客户端发送数据
							USART_RX_STA=0;								// 串口接收状态标志位清0
							memset(USART_RX_BUF,0,len+1);	// 串口接收缓存清0
						}
						
									
						
				break;
				case SOCK_CLOSE_WAIT:								// Socket处于等待关闭状态
						close(0);												// 关闭Socket0
				break;
				case SOCK_CLOSED:										// Socket处于关闭状态
						socket(0,Sn_MR_TCP,local_port,Sn_MR_ND);		// 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
				break;
			}
		
			
		//检测网线连接状态
			//NetWork_LinkIsChanged(&W5500_Conifg,&linkstatus);
			if(NETWORK_SUCCESS==NetWork_LinkIsChanged(&W5500_Conifg,&linkstatus)){
							if(linkstatus==PHY_LINK_ON){
								printf("网线已连接！\r\n");
							}else{
								printf("网线已断开！\r\n");
								close(0);	
							}
						}
			
			NetWork_SocketStatusIsChanged(&W5500_Conifg);					
			vTaskDelay(5);	
	}
}

