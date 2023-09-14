#include <stdio.h>
#include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
#include "esp_log.h"
#include "ce2727a.h"
#include "driver/uart.h"

static const char *TAG = "ce2727a-example";

#define BUF_SIZE 128
uint8_t rx_buffer[BUF_SIZE];

void app_main(void)
{
    ESP_LOGI(TAG, "CE2727a example started");

    ce2727a_config_t config = {
        .uart_config_override = NULL,
        .uart_port = UART_NUM_2,
        .uart_rx_pin = 16,
        .uart_tx_pin = 17,
        .uart_de_pin = 18,
        .addr = 0x0,
        .password = 0x0,
        .rx_buffer = rx_buffer,
        .rx_buffer_size = BUF_SIZE};

    ESP_LOGI(TAG, "Init ce2727a driver");
    ce2727a_handle_t handle = NULL;
    ESP_ERROR_CHECK(ce2727a_init(&config, &handle));

    while (true)
    {
        ce2727a_readings_energy_t readings = {0};
        uint32_t active_power = 0;

        esp_err_t err = ESP_OK;

        ESP_LOGI(TAG, "Requesting active power...");
        err = ce2727a_get_active_power(handle, &active_power, 1000);
        if (err == ESP_OK)
        {
            ESP_LOGI(TAG, "Got active power: %lu Watts", active_power);
        }
        else
        {
            ESP_LOGE(TAG, "Error getting active power: 0x%x", err);
        }

        ESP_LOGI(TAG, "Requesting readings...");
        err = ce2727a_get_energy(handle, &readings, 1000);
        if (err == ESP_OK)
        {
            ESP_LOGI(TAG, "Got readings: T1 = %lu, T2 = %lu", readings.t1, readings.t2);
        }
        else
        {
            ESP_LOGE(TAG, "Error getting readings: 0x%x", err);
        }

        ESP_LOGI(TAG, "Free heap %lu ", esp_get_free_heap_size());

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
