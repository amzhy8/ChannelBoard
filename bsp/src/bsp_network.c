#include "bsp_network.h"
#include <string.h>
#include "bsp_uart.h"

W5500_Conifg_Typedef W5500_Conifg;
xQueueHandle NetWorkRecieveFifo;
uint8_t USART_RX_BUF[USART_REC_LEN];     // ����һ�����ڴ������ݽ��յĻ��棬��СΪUSART_REC_LEN
uint16_t USART_RX_STA=0;       					// ���岢��ʼ�����ڽ��ձ�־λ

uint8_t buffer[NETWORK_FIFO_LEN];		
void Reset_W5500(void)
{
	HAL_GPIO_WritePin(W5500_RST_GPIO_PORT,W5500_RST_GPIO_PIN,GPIO_PIN_RESET);
	vTaskDelay(1);
	HAL_GPIO_WritePin(W5500_RST_GPIO_PORT,W5500_RST_GPIO_PIN,GPIO_PIN_SET);
  vTaskDelay(2);
}




uint8_t NetWorkModuleInit(){
	//��������ģ���еĽ���FIFO
	NetWorkRecieveFifo=xQueueCreate(NETWORK_FIFO_LEN,sizeof(uint8_t));
	
	

	return NETWORK_SUCCESS;
}


//�����������״̬
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


//�����������״̬
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




// ����Ĭ��MAC��IP��GW��SUB��DNS
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
// ���ó�ʼ��IP��Ϣ����ӡ����ʼ��8��Socket
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
	ctlnetwork(CN_SET_NETINFO, &W5500_Conifg.netinfo);//�����������
	vTaskDelay(10);

	//ctlwizchip(CW_SET_PHYCONF, &phyConf);//����PHY
	vTaskDelay(1000);
	//��ȡ�����������
	if(0==ctlwizchip(CW_GET_PHYLINK, &link)){
		if(link==PHY_LINK_ON){
			printf("���������ӣ�\r\n");
		}else{
			printf("����δ���ӣ�\r\n");
		}
	}
//set rx tx buf 2k
	wizchip_init(NULL,NULL);
  
  setRTR(2000);																		// �����ش���ʱʱ��
  setRCR(3);																			// ����������·��ʹ���
  
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
	
	Reset_W5500();//��λW5500
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
		
		switch(getSn_SR(0))										// ��ȡsocket0��״̬
			{ 
				case SOCK_INIT:											// Socket���ڳ�ʼ�����(��)״̬
						listen(0);											// �����ոմ򿪵ı��ض˿ڣ��ȴ��ͻ�������
				break;
				case SOCK_LISTEN:
				break;
				case SOCK_ESTABLISHED:							// Socket�������ӽ���״̬
						if(getSn_IR(0) & Sn_IR_CON)			
						{
							setSn_IR(0, Sn_IR_CON);				// Sn_IR��CONλ��1��֪ͨW5500�����ѽ���
						}
						// ���ݻػ����Գ������ݴ���λ���ͻ��˷���W5500��W5500���յ����ݺ��ٻظ��ͻ���
						len=getSn_RX_RSR(0);						// ��ȡW5500���н��ջ���Ĵ�����ֵ������len��Sn_RX_RSR��ʾ���ջ������ѽ��պͱ�������ݴ�С
						if(len>0)
						{
							reallen=recv(0,buffer,len);						// W5500�������Կͻ��˵����ݣ���ͨ��SPI���͸�MCU
							//�����ݴ������
							for(i=0;i<reallen;i++){
								xQueueSend(NetWorkRecieveFifo,&buffer[i],0);
							}
							reallen=0;
							printf("%s\r\n",buffer);			// ���ڴ�ӡ���յ�������
							//send(0,buffer,len);						// ���յ����ݺ��ٻظ��ͻ��ˣ�������ݻػ�
						}
						// W5500�Ӵ��ڷ����ݸ��ͻ��˳����������Իس�����
						if(USART_RX_STA & 0x8000)				// �жϴ��������Ƿ�������
						{					   
							len=USART_RX_STA & 0x3fff;		// ��ȡ���ڽ��յ����ݵĳ���
							send(0,USART_RX_BUF,len);			// W5500��ͻ��˷�������
							USART_RX_STA=0;								// ���ڽ���״̬��־λ��0
							memset(USART_RX_BUF,0,len+1);	// ���ڽ��ջ�����0
						}
						
									
						
				break;
				case SOCK_CLOSE_WAIT:								// Socket���ڵȴ��ر�״̬
						close(0);												// �ر�Socket0
				break;
				case SOCK_CLOSED:										// Socket���ڹر�״̬
						socket(0,Sn_MR_TCP,local_port,Sn_MR_ND);		// ��Socket0��������ΪTCP����ʱģʽ����һ�����ض˿�
				break;
			}
		
			
		//�����������״̬
			//NetWork_LinkIsChanged(&W5500_Conifg,&linkstatus);
			if(NETWORK_SUCCESS==NetWork_LinkIsChanged(&W5500_Conifg,&linkstatus)){
							if(linkstatus==PHY_LINK_ON){
								printf("���������ӣ�\r\n");
							}else{
								printf("�����ѶϿ���\r\n");
								close(0);	
							}
						}
			
			NetWork_SocketStatusIsChanged(&W5500_Conifg);					
			vTaskDelay(5);	
	}
}

