#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//Biblioteca de conexao com o Wifi
#include "include/wifi_connection.h"

//Biblioteca de criacao do servidor
#include "include/server.h"

//Bibioteca para ADC
#include "driver/adc.h"
#include "esp_adc_cal.h"
//esp_adc/adc_oneshot.h and esp_adc/adc_continuous.h


#define LED_RES_PIN 2 //da esp
#define RES_PIN 25 //marrom claro
#define LED_PIN 26 //marrom escuro

#define LDR_PIN 34 //ADC1_6 ADC1_CHANNEL_6 //laranja
#define DHT_PIN 35 //ADC1_7 ADC1_CHANNEL_7 //azul

static const char *TAGwifi = "wifi_task";
static const char *TAGr = "reading_task";
static const char *TAGs = "server_task";


void getReadingsTask(void * parameters){
    uint32_t voltage;
    int reading = 0;
    int led_res_level = 0;
    int led_level = 0;
    esp_adc_cal_characteristics_t adc1_chars; //Calibracao LDR
    adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0); //Atenuacao LDR
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT); //Widht LDR
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    for( ;; ){
        ESP_LOGI(TAGr,"Get readings");

        //LDR
        reading = adc1_get_raw(ADC1_CHANNEL_6);
        //ESP_ERROR_CHECK(adc1_get_raw(ADC2_CHANNEL_3, ADC_WIDTH_BIT_DEFAULT, &reading));
        ESP_LOGI(TAGr, "LDR: %d", reading);
        voltage = esp_adc_cal_raw_to_voltage(reading, &adc1_chars);
        ESP_LOGI(TAGr, "ADC1_CHANNEL_6: %lu mV", voltage);

        //Leds
        led_res_level = ~led_res_level;
        led_level = ~led_level;
        gpio_set_level(LED_RES_PIN, led_res_level);
        gpio_set_level(LED_PIN, led_level);

        //Delay
        vTaskDelay(500);
    }
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

    ESP_LOGI(TAGwifi, "Starting Wifi Login ... ... \n");
    //wifi_init_sta();
    ESP_LOGI(TAGwifi, "Wifi is Running! \n");

    // GPIO initialization POT + LED INBOARD
    gpio_reset_pin(LED_RES_PIN);
    gpio_set_direction(LED_RES_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(RES_PIN);
    gpio_set_direction(RES_PIN, GPIO_MODE_OUTPUT);

    // GPIO initialization LED OFFBOARD
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // GPIO initialization LDR
    gpio_reset_pin(LDR_PIN);
    gpio_set_direction(LDR_PIN, GPIO_MODE_INPUT);

    // GPIO initialization DHT
    gpio_reset_pin(DHT_PIN);
    gpio_set_direction(DHT_PIN, GPIO_MODE_INPUT);

    ESP_LOGI(TAGs, "Starting Web Server ... ...\n");
    //setup_server();
    ESP_LOGI(TAGs, "Web Server is running! \n");

   // xTaskCreate(&serverTask, "serverTask",2048,NULL,5,NULL); //funct_name, task_name, stack_size, task_param, task_priority, task_handle
    xTaskCreate(&getReadingsTask, "getReadingsTask",2048,NULL,5,NULL);
}