#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "voice.h"
 
 
#define BUF_SIZE				(1024)
#define RD_BUF_SIZE 			(BUF_SIZE)
 
#define TXD2_PIN 			(GPIO_NUM_33)//(GPIO_NUM_33)  //���յ�·������� D33Ϊ�������ݽ�������B2
#define RXD2_PIN 			(GPIO_NUM_32)//(GPIO_NUM_32)  //���յ�·������� D32Ϊ�������ݽ�������B3
//�Ե��������ڣ���Ϊ���԰���ƺͽ��ܶ˿ڣ��������������Ϣ���ܺͷ��� 
uart_event_t    uart2_event;
QueueHandle_t	uart2_queue;
 
 
void uart_event_handle(uart_port_t uart_num, QueueHandle_t queue, uart_event_t *event);

//����2�ĳ�ʼ��
void uart_init(void)
{
    //����0��������
    uart_config_t uart2_config = {
        .baud_rate = 115200,                    //������
        .data_bits = UART_DATA_8_BITS,          //����λ
        .parity = UART_PARITY_DISABLE,          //У��λ
        .stop_bits = UART_STOP_BITS_1,          //ֹͣλ
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE   //Ӳ������
    };

  
    //����0��������
	uart_param_config(UART_NUM_2, &uart2_config);		//���ô���
	//IOӳ��-> T:IO1  R:IO3
	uart_set_pin(UART_NUM_2, TXD2_PIN, RXD2_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	//ע�ᴮ�ڷ���ʹ��+���û�������С
	uart_driver_install(UART_NUM_2, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart2_queue, 0);

	uart_pattern_queue_reset(UART_NUM_2, 20);
}

//����2��������
void uart2_rx_task()
{
    while (1) {
        if (xQueueReceive(uart2_queue, (uint32_t *) &uart2_event, 0))
		{
			uart_event_handle(UART_NUM_2, uart2_queue, &uart2_event);
		}
		 vTaskDelay(pdMS_TO_TICKS(10));      //Delay 10ms
    }
}


void app_main(void)
{
	//���ڳ�ʼ�� ֻ�Դ���2�������������г�ʼ��
	uart_init();
 
 	//��������2��������
	xTaskCreate(uart2_rx_task, "uart2_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);

	while (1)
	{
		//���Լ����������
 
        vTaskDelay(pdMS_TO_TICKS(10));      //Delay 10ms
	}
    
}
 

void uart_event_handle(uart_port_t uart_num, QueueHandle_t queue, uart_event_t *event)
{
	uint8_t *dtmp = malloc(RD_BUF_SIZE+1);
    bzero(dtmp, RD_BUF_SIZE+1);
 
	switch (event->type)
	{
		//UART������ݵ��¼���������
		case UART_DATA:
			uart_read_bytes(uart_num, dtmp, event->size, portMAX_DELAY);
			//����ȡ������ָ���������������������
			vioce_analysis(uart_num, dtmp);
			//uart_write_bytes(uart_num, (const char *) dtmp, event->size);
			break;
 
		//����������������
		case UART_FIFO_OVF:
			// If fifo overflow happened, you should consider adding flow control for your application.
			// The ISR has already reset the rx FIFO,
			// As an example, we directly flush the rx buffer here in order to read more data.
			uart_flush_input(uart_num);
			xQueueReset(queue);
			break;
 
		//Event of UART ring buffer full
		case UART_BUFFER_FULL:
			// If buffer full happened, you should consider encreasing your buffer size
			// As an example, we directly flush the rx buffer here in order to read more data.
			uart_flush_input(uart_num);
			xQueueReset(queue);
			break;
 
		//Event of UART RX break detected
		case UART_BREAK:
			break;
 
		//Event of UART parity check error
		case UART_PARITY_ERR:
			break;
 
		//Event of UART frame error
		case UART_FRAME_ERR:
			break;
 
		//UART_PATTERN_DET
		case UART_PATTERN_DET:
			break;
 
		//Others
		default:
			break;
	}
    
  	free(dtmp);
	dtmp = NULL;
}
 
 