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
 
#define TXD2_PIN 			(GPIO_NUM_33)//(GPIO_NUM_33)  //按照电路进行设计 D33为接受数据接语音板B2
#define RXD2_PIN 			(GPIO_NUM_32)//(GPIO_NUM_32)  //按照电路进行设计 D32为接受数据接语音板B3
//以第三个串口，作为语言板控制和接受端口，进行语音板的信息接受和发送 
uart_event_t    uart2_event;
QueueHandle_t	uart2_queue;
 
 
void uart_event_handle(uart_port_t uart_num, QueueHandle_t queue, uart_event_t *event);

//串口2的初始化
void uart_init(void)
{
    //串口0参数配置
    uart_config_t uart2_config = {
        .baud_rate = 115200,                    //波特率
        .data_bits = UART_DATA_8_BITS,          //数据位
        .parity = UART_PARITY_DISABLE,          //校验位
        .stop_bits = UART_STOP_BITS_1,          //停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE   //硬件流控
    };

  
    //串口0参数配置
	uart_param_config(UART_NUM_2, &uart2_config);		//设置串口
	//IO映射-> T:IO1  R:IO3
	uart_set_pin(UART_NUM_2, TXD2_PIN, RXD2_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	//注册串口服务即使能+设置缓存区大小
	uart_driver_install(UART_NUM_2, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart2_queue, 0);

	uart_pattern_queue_reset(UART_NUM_2, 20);
}

//串口2接收任务
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
	//串口初始化 只对串口2（第三个）进行初始化
	uart_init();
 
 	//创建串口2接收任务
	xTaskCreate(uart2_rx_task, "uart2_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);

	while (1)
	{
		//做自己程序的事情
 
        vTaskDelay(pdMS_TO_TICKS(10));      //Delay 10ms
	}
    
}
 

void uart_event_handle(uart_port_t uart_num, QueueHandle_t queue, uart_event_t *event)
{
	uint8_t *dtmp = malloc(RD_BUF_SIZE+1);
    bzero(dtmp, RD_BUF_SIZE+1);
 
	switch (event->type)
	{
		//UART获得数据的事件触发函数
		case UART_DATA:
			uart_read_bytes(uart_num, dtmp, event->size, portMAX_DELAY);
			//将获取的语音指令，交给语音分析函数处理
			vioce_analysis(uart_num, dtmp);
			//uart_write_bytes(uart_num, (const char *) dtmp, event->size);
			break;
 
		//队列溢出的情况发生
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
 
 