#include <stdio.h>
#include <string.h>
#include "ce2727a.h"
#include "frames.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "driver/uart.h"

#include "frames.h"

uint16_t crc16iec(const uint8_t *buffer, uint16_t len);

#define CHECK(x)                \
    do                          \
    {                           \
        esp_err_t __;           \
        if ((__ = x) != ESP_OK) \
            return __;          \
    } while (0)

#define MAGIC_2 0x02

#define COM_ENQ 0x01
#define COM_REC 0x03
#define COM_DRJ 0x0A
#define COM_OK 0x0B

#define ENQ_CMD_INFO 0x00
#define ENQ_CMD_DATE_TIME 0x01
#define ENQ_CMD_ACTIVE_POWER 0x02
#define ENQ_CMD_CONSUMED_ENERGY 0x03

// driver state
struct ce2727a_t
{
    ce2727a_config_t config;
    ce2727a_request_command_t *tx_buffer;
    uint16_t tx_buffer_size;
};

esp_err_t ce2727a_init(const ce2727a_config_t *config, ce2727a_handle_t *out_handle)
{
    struct ce2727a_t *result = calloc(1, sizeof(struct ce2727a_t));
    if (result == NULL)
    {
        return ESP_ERR_NO_MEM;
    }
    result->tx_buffer = calloc(1, sizeof(ce2727a_request_command_t));
    if (result->tx_buffer == NULL)
    {
        return ESP_ERR_NO_MEM;
    }
    result->tx_buffer_size = sizeof(ce2727a_request_command_t);
    result->config = *config;
    *out_handle = result;

    ESP_ERROR_CHECK(
        uart_param_config(config->uart_port, &config->uart_config));
    ESP_ERROR_CHECK(
        uart_driver_install(config->uart_port, 256, 0, 0, NULL, 0));

    return ESP_OK;
}

esp_err_t ce2727a_deinit(ce2727a_handle_t handle)
{
    uart_driver_delete(handle->config.uart_port);
    free(handle->tx_buffer);
    free(handle);
    return ESP_OK;
}

void ce2727a_fill_enq_cmd(ce2727a_handle_t handle, uint8_t com_id)
{
    ce2727a_request_command_t *f = handle->tx_buffer;
    f->header.two = MAGIC_2;
    f->header.length = sizeof(ce2727a_request_command_t);
    f->header.address = handle->config.addr;
    f->header.password = handle->config.password;
    f->header.com = COM_ENQ;
    f->header.id = (uint8_t)com_id;
    f->crc16 = crc16iec((const uint8_t *)f, sizeof(ce2727a_request_command_t) - 2); // minus 2 bytes for CRC
}

esp_err_t ce2727a_send_enq_cmd(ce2727a_handle_t handle, uint8_t com_id, uint16_t timeout_ms)
{
    ce2727a_fill_enq_cmd(handle, com_id);
    // Check initialized
#if HELPER_TARGET_IS_ESP32 && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
    if (!uart_is_driver_installed(handle->config.uart_port))
        return ESP_ERR_INVALID_STATE;
#endif

    uart_flush(handle->config.uart_port);
    uart_write_bytes(handle->config.uart_port, (char *)handle->tx_buffer, handle->tx_buffer_size);
    return ESP_OK;
}

esp_err_t ce2727a_receive_proper_response(ce2727a_handle_t handle, uint16_t expected_response_size, uint16_t timeout_ms)
{
    memset(handle->config.rx_buffer, 0, handle->config.rx_buffer_size);

    // Read response from serial buffer
    int len = uart_read_bytes(handle->config.uart_port, handle->config.rx_buffer,
                              handle->config.rx_buffer_size,
                              timeout_ms / portTICK_PERIOD_MS);

    if (len < expected_response_size)
    {
        return ESP_ERR_TIMEOUT;
    }

    if (len != expected_response_size)
    {
        return ESP_ERR_INVALID_RESPONSE;
    }

    // Check CRC
    u_int16_t *p_crc = (u_int16_t *)(handle->config.rx_buffer + expected_response_size - 2);
    if (crc16iec(handle->config.rx_buffer, expected_response_size - 2) != *p_crc)
    {
        return ESP_ERR_INVALID_CRC;
    }

    return ESP_OK;
}

esp_err_t ce2727a_get_active_power(ce2727a_handle_t handle, uint32_t *active_power, uint16_t timeout_ms)
{
    if (handle->config.rx_buffer_size < sizeof(ce2727a_response_active_power_t) || active_power == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }
    esp_err_t err = ce2727a_send_enq_cmd(handle, ENQ_CMD_ACTIVE_POWER, timeout_ms);
    if (err != ESP_OK)
        return err;
    err = ce2727a_receive_proper_response(handle, sizeof(ce2727a_response_active_power_t), timeout_ms);
    ce2727a_response_active_power_t *res = (ce2727a_response_active_power_t *)handle->config.rx_buffer;

    *active_power = res->activePower;

    return err;
}

esp_err_t ce2727a_get_energy(ce2727a_handle_t handle, ce2727a_readings_energy_t *readings, uint16_t timeout_ms)
{
    if (handle->config.rx_buffer_size < sizeof(ce2727a_response_consumed_energy_t) || readings == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }
    esp_err_t err = ce2727a_send_enq_cmd(handle, ENQ_CMD_CONSUMED_ENERGY, timeout_ms);
    if (err != ESP_OK)
        return err;
    err = ce2727a_receive_proper_response(handle, sizeof(ce2727a_response_consumed_energy_t), timeout_ms);
    ce2727a_response_consumed_energy_t *res = (ce2727a_response_consumed_energy_t *)handle->config.rx_buffer;

    readings->active_tariff = res->currentTariff;
    readings->t1 = res->t1;
    readings->t2 = res->t2;
    readings->t3 = res->t3;
    readings->t4 = res->t4;

    return err;
}

//---------------------------------------------------------------------------------------
//  The standard 16-bit CRC polynomial specified in ISO/IEC 3309 is used.
//             16   12   5
//  Which is: x  + x  + x + 1
//----------------------------------------------------------------------------
uint16_t crc16iec(const uint8_t *buffer, uint16_t len)
{
    uint16_t crc = 0xffff;
    uint8_t d;
    do
    {
        d = *buffer++ ^ (crc & 0xFF);
        d ^= d << 4;
        crc = (d << 3) ^ (d << 8) ^ (crc >> 8) ^ (d >> 4);
    } while (--len);
    crc ^= 0xFFFF;
    return crc;
}
