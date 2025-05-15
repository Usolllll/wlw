#include <esp_log.h>
#include <inttypes.h>
#include "rc522.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*参照文件 https://gitcode.com/gh_mirrors/es/esp-idf-rc522 */
static const char* TAG = "rc522-demo";
static rc522_handle_t scanner;

static void rc522_handler(void* arg, esp_event_base_t base, int32_t event_id, void* event_data)
{
    rc522_event_data_t* data = (rc522_event_data_t*) event_data;

    switch(event_id) {
        case RC522_EVENT_TAG_SCANNED: {
                rc522_tag_t* tag = (rc522_tag_t*) data->ptr;
                ESP_LOGI(TAG, "Tag scanned (sn: %" PRIu64 ")", tag->serial_number);
            }
            //安全打印 uint64_t	printf("%" PRIu64, num);	<inttypes.h>
            //简单场景（现代平台）	printf("%llu", num);	<stdint.h>
            break;
    }
}

void rc522_task(void *pvParameters)
{
    while(1) {
        
        
        rc522_start(scanner);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void app_main()
{
    rc522_config_t config = {
        .spi.host = VSPI_HOST,
        .spi.miso_gpio = 25,
        .spi.mosi_gpio = 23,
        .spi.sck_gpio = 19,
        .spi.sda_gpio = 22,
    };

    
    rc522_create(&config, &scanner);
    rc522_register_events(scanner, RC522_EVENT_ANY, rc522_handler, NULL);
    // 创建RC522任务
    xTaskCreate(rc522_task, "rc522_task", 4096, NULL, 5, NULL);
}