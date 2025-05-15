#include "voice.h"


uint8_t usart_su_TXpacket[14] = {0};	//hex数据包接收数组

/**
  * @brief  处理串口2的数据
  * @param  None
  * @retval None
  * uart_port_t uart_num,uint8_t *info,uint16_t size
  */
void vioce_analysis(uart_port_t uart_num,uint8_t *info){
    memset(usart_su_TXpacket,0,14);
    printf("%s 语音输入,第一个字节为%x\n",info,info[0]);//输出数据
    uint8_t temp = info[0];
		switch(temp){
			case 0x01: //打开灯
				printf("语音输入打开灯\r\n");//输出数据
				break;
			case 0x02://关闭灯
				printf("语音输入关闭灯\r\n");//输出数据
				break;
			case 0x03://风量大一点
			    usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0x06;
				usart_su_TXpacket[3] = 0x11;//根据实际情况修改
				usart_su_TXpacket[4] = 0x00;
				usart_su_TXpacket[5] = 0x00;
				usart_su_TXpacket[6] = 0x00;
				usart_su_TXpacket[7] = 0X55;
				usart_su_TXpacket[8] = 0XAA;
                uart_write_bytes(uart_num, (uint8_t *) usart_su_TXpacket, 9);
				break;
			case 0x04://风量小一点
			  usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0x06;
				usart_su_TXpacket[3] = 0x11;//根据实际情况修改;
				usart_su_TXpacket[4] = 0x00;
				usart_su_TXpacket[5] = 0x00;
				usart_su_TXpacket[6] = 0x00;
				usart_su_TXpacket[7] = 0X55;
				usart_su_TXpacket[8] = 0XAA;
                uart_write_bytes(uart_num, (uint8_t *) usart_su_TXpacket, 9);
				break;
			case 0x05://汽车点火
				//voicest=0x05;
				break;
			case 0x06://汽车关火
				//voicest=0x06;
				break;
			case 0x07://打开车门
			//	voicest=0x07;
				break;
			case 0x08://关闭车门
			//	voicest=0x08;
				break;
			case 0x0A://询问当前温度
                printf("语音输入当前温度为\r\n");//输出数据
				//DHT11_Read_Data(&humih,&humil,&temph,&templ);
				usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0x01;
				usart_su_TXpacket[3] = 0x10;//temph;这句话要根据温湿度情况进行修改
				usart_su_TXpacket[4] = 0x00;
				usart_su_TXpacket[5] = 0x00;
				usart_su_TXpacket[6] = 0x00;
				usart_su_TXpacket[7] = 0x10;//这句话要根据温湿度情况进行修改
				usart_su_TXpacket[8] = 0x00;
				usart_su_TXpacket[9] = 0x00;
				usart_su_TXpacket[10] = 0x00;
				usart_su_TXpacket[11] = 0X55;
				usart_su_TXpacket[12] = 0XAA;
                uart_write_bytes(uart_num, (uint8_t *) usart_su_TXpacket, 13);
				break;
			case 0x0B://询问当前湿度
				//DHT11_Read_Data(&humih,&humil,&temph,&templ);
                printf("语音输入当前湿度为\r\n");//输出数据
				
				//usart_send3su_array(usart_su_TXpacket, 13);
				break;
			case 0x0C://询问当前速度
				break;
			case 0x0D://询问当前亮度
                printf("语音输入当前亮度为\r\n");//输出数据
				
				//usart_send3su_array(usart_su_TXpacket, 9);
				break;
			case 0x0E://冷暖需求状态高一点（温度高一点）
				
				break;
			case 0x0F://冷暖需求状态低一点（温度低一点）
			
				break;
			case 0x09://打开前除雾
			// 	voicest=0x09;
			//   Front_Lamp(0);
				break;
			case 0x00://关闭前除雾
			
				break;
			case 0xB1://打开吹面吹足
			
				break;
			case 0xB2://关闭吹面吹足
			
				break;
			case 0xC1://打开AC
				
			  //AC_Lamp(0);
		
				break;
			case 0xC2://关闭AC
				
			  //AC_Lamp(1);
			 
				break;
			case 0xD1://打开氛围模式
				//voicest=0xD1;
				//TIM_Cmd(TIM3, ENABLE);  //使能TIM4						
				break;
			case 0xD2://关闭氛围模式
				//voicest=0xD2;
				//TIM_Cmd(TIM3, DISABLE);  //不使能TIM4						
				break;
	}
}
