#ifndef DHT11_H
#define DHT11_H

#include "driver/gpio.h"
#include "esp_err.h"

#define DHT11_GPIO 4

typedef struct
{
  float temperature;
  float humidity;
} dht11_data_t;

/**
 * @brief 初始化DHT11传感器
 *
 * @return esp_err_t ESP_OK表示成功，其他表示失败
 */
esp_err_t dht11_init();

/**
 * @brief 读取DHT11传感器数据
 *
 * @param data 存储读取到的温湿度数据
 * @return esp_err_t ESP_OK表示成功，其他表示失败
 */
esp_err_t dht11_read(dht11_data_t *data);

#endif // DHT11_H
