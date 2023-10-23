#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//Biblioteca de conexao com o Wifi
#include "include/wifi_connection.h"

//Biblioteca de criacao do servidor
#include "include/server.h"

#define LED_PIN 2

static const char *TAGwifi = "wifi_task";
static const char *TAGr = "reading_task";
static const char *TAGs = "server_task";


void getReadingsTask(void * parameters){
    ESP_LOGI(TAGr,"Get readings");
    vTaskDelay(3000);
}

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAGwifi, "ESP_WIFI_MODE_STA");
    wifi_init_sta();

    // GPIO initialization
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAGs, "LED Control Web Server is running ... ...\n");
    setup_server();

   // xTaskCreate(&serverTask, "serverTask",2048,NULL,5,NULL); //funct_name, task_name, stack_size, task_param, task_priority, task_handle
   // xTaskCreate(&getReadingsTask, "getReadingsTask",2048,NULL,5,NULL);
}