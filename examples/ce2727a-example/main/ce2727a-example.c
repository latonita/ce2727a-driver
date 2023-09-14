#include <stdio.h>
#include "esp_log.h"
#include "ce2727a.h"
#include "driver/uart.h"

static const char *TAG = "ce2727a-example";

#define BUF_SIZE 128
uint8_t buff[BUF_SIZE];

void app_main(void)
{
    ESP_LOGI(TAG, "CE2727a example started");

    ce2727a_config_t config = {
        .uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_EVEN,
            .stop_bits = UART_STOP_BITS_1},
        .uart_port = UART_NUM_1,
        .addr = 0x0,
        .rx_buffer = buff,
        .rx_buffer_size = BUF_SIZE};

    ce2727a_handle_t handle = NULL;
    ESP_ERROR_CHECK(ce2727a_init(&config, &handle));

    ce2727a_readings_energy_t readings = {0};
    uint32_t active_power = 0;
    
    esp_err_t err = ESP_OK;

    ESP_LOGI(TAG, "Requesting active power...");
    err = ce2727a_get_active_power(handle, &active_power, 1000);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Got active power");
    }
    else
    {
        ESP_LOGE(TAG, "Error getting active power: 0x%x", err);
    }

    ESP_LOGI(TAG, "Requesting readings...");
    err = ce2727a_get_energy(handle, &readings, 1000);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Got");
        // ESP_LOGI(TAG, "Got readings: T1 = %d", readings.t1);
        // ESP_LOGI(TAG, "Got readings: T2 = %d", readings.t2);
    }
    else
    {
        ESP_LOGE(TAG, "Error getting readings: 0x%x", err);
    }

    // char nmea_msg_buf[200];
    // while (true) {
    //     esp_err_t err = pa1010d_get_nmea_msg(handle,
    //                                          nmea_msg_buf,
    //                                          sizeof(nmea_msg_buf),
    //                                          1000);
    //     if (err != ESP_OK) {
    //         ESP_LOGE(TAG, "Error getting NMEA message: 0x%x", err);
    //         continue;
    //     }

    //     ESP_LOGI(TAG, "Got message: '%s'", nmea_msg_buf);
    // }
}
