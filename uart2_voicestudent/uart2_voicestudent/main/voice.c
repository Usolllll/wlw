#include "voice.h"


uint8_t usart_su_TXpacket[14] = {0};	//hex���ݰ���������

/**
  * @brief  ������2������
  * @param  None
  * @retval None
  * uart_port_t uart_num,uint8_t *info,uint16_t size
  */
void vioce_analysis(uart_port_t uart_num,uint8_t *info){
    memset(usart_su_TXpacket,0,14);
    printf("%s ��������,��һ���ֽ�Ϊ%x\n",info,info[0]);//�������
    uint8_t temp = info[0];
		switch(temp){
			case 0x01: //�򿪵�
				printf("��������򿪵�\r\n");//�������
				break;
			case 0x02://�رյ�
				printf("��������رյ�\r\n");//�������
				break;
			case 0x03://������һ��
			    usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0x06;
				usart_su_TXpacket[3] = 0x11;//����ʵ������޸�
				usart_su_TXpacket[4] = 0x00;
				usart_su_TXpacket[5] = 0x00;
				usart_su_TXpacket[6] = 0x00;
				usart_su_TXpacket[7] = 0X55;
				usart_su_TXpacket[8] = 0XAA;
                uart_write_bytes(uart_num, (uint8_t *) usart_su_TXpacket, 9);
				break;
			case 0x04://����Сһ��
			  usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0x06;
				usart_su_TXpacket[3] = 0x11;//����ʵ������޸�;
				usart_su_TXpacket[4] = 0x00;
				usart_su_TXpacket[5] = 0x00;
				usart_su_TXpacket[6] = 0x00;
				usart_su_TXpacket[7] = 0X55;
				usart_su_TXpacket[8] = 0XAA;
                uart_write_bytes(uart_num, (uint8_t *) usart_su_TXpacket, 9);
				break;
			case 0x05://�������
				//voicest=0x05;
				break;
			case 0x06://�����ػ�
				//voicest=0x06;
				break;
			case 0x07://�򿪳���
			//	voicest=0x07;
				break;
			case 0x08://�رճ���
			//	voicest=0x08;
				break;
			case 0x0A://ѯ�ʵ�ǰ�¶�
                printf("�������뵱ǰ�¶�Ϊ\r\n");//�������
				//DHT11_Read_Data(&humih,&humil,&temph,&templ);
				usart_su_TXpacket[0] = 0XAA;
				usart_su_TXpacket[1] = 0X55;
				usart_su_TXpacket[2] = 0x01;
				usart_su_TXpacket[3] = 0x10;//temph;��仰Ҫ������ʪ����������޸�
				usart_su_TXpacket[4] = 0x00;
				usart_su_TXpacket[5] = 0x00;
				usart_su_TXpacket[6] = 0x00;
				usart_su_TXpacket[7] = 0x10;//��仰Ҫ������ʪ����������޸�
				usart_su_TXpacket[8] = 0x00;
				usart_su_TXpacket[9] = 0x00;
				usart_su_TXpacket[10] = 0x00;
				usart_su_TXpacket[11] = 0X55;
				usart_su_TXpacket[12] = 0XAA;
                uart_write_bytes(uart_num, (uint8_t *) usart_su_TXpacket, 13);
				break;
			case 0x0B://ѯ�ʵ�ǰʪ��
				//DHT11_Read_Data(&humih,&humil,&temph,&templ);
                printf("�������뵱ǰʪ��Ϊ\r\n");//�������
				
				//usart_send3su_array(usart_su_TXpacket, 13);
				break;
			case 0x0C://ѯ�ʵ�ǰ�ٶ�
				break;
			case 0x0D://ѯ�ʵ�ǰ����
                printf("�������뵱ǰ����Ϊ\r\n");//�������
				
				//usart_send3su_array(usart_su_TXpacket, 9);
				break;
			case 0x0E://��ů����״̬��һ�㣨�¶ȸ�һ�㣩
				
				break;
			case 0x0F://��ů����״̬��һ�㣨�¶ȵ�һ�㣩
			
				break;
			case 0x09://��ǰ����
			// 	voicest=0x09;
			//   Front_Lamp(0);
				break;
			case 0x00://�ر�ǰ����
			
				break;
			case 0xB1://�򿪴��洵��
			
				break;
			case 0xB2://�رմ��洵��
			
				break;
			case 0xC1://��AC
				
			  //AC_Lamp(0);
		
				break;
			case 0xC2://�ر�AC
				
			  //AC_Lamp(1);
			 
				break;
			case 0xD1://�򿪷�Χģʽ
				//voicest=0xD1;
				//TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM4						
				break;
			case 0xD2://�رշ�Χģʽ
				//voicest=0xD2;
				//TIM_Cmd(TIM3, DISABLE);  //��ʹ��TIM4						
				break;
	}
}
