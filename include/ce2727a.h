#pragma once

#include "esp_err.h"
#include "driver/uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief CE2727A driver configuration
 */
typedef struct ce2727a_config_t {
    uart_config_t uart_config; /*!< UART port configuration parameters like baud rate and parity*/
    uint8_t uart_port; /*!< UART port number*/
    uint32_t addr;  /*!< Target power meter address, default is 0x0 - broadcasting to all meters  */
    uint32_t password;  /*!< Target power meter password, default is 0x0 */
    uint8_t * rx_buffer;   /*!< Receive buffer. Shall be enough to hold required responses */
    uint16_t rx_buffer_size;  /*!< Size of receive buffer */
} ce2727a_config_t;

/**
 * @brief CE2727A readings
*/
typedef struct ce2727a_readings_energy_t {
    uint8_t active_tariff;  /*!< Currently active tariff 1...4 */
    uint32_t t1;  /*!< Cumulative energy for tariff 1 */
    uint32_t t2;  /*!< Cumulative energy for tariff 2 */
    uint32_t t3;  /*!< Cumulative energy for tariff 3 */
    uint32_t t4;  /*!< Cumulative energy for tariff 4 */
} ce2727a_readings_energy_t;

/**
 * @brief CE2727A driver handle
 */
typedef struct ce2727a_t *ce2727a_handle_t;

/**
 * @brief Initialize the CE2727A driver
 *
 * @param config Pointer to the configuration struct. The driver makes a copy, so can point to a local variable.
 * @param[out] out_handle  Pointer to a variable to receive the driver handle.
 * @return esp_err_t  ESP_OK on success, ESP_ERR_NO_MEM if out of memory.
 */
esp_err_t ce2727a_init(const ce2727a_config_t *config, ce2727a_handle_t *out_handle);

/**
 * @brief Deinitialize the CE2727A driver
 *
 * @param handle Driver handle obtained from ce2727a_init(), or NULL
 * @return esp_err_t  ESP_OK on success.
 */
esp_err_t ce2727a_deinit(ce2727a_handle_t handle);

/**
 * @brief Get active power
 *
 * Get current active power
 *
 * @param handle Driver handle obtained from ce2727a_init()
 * @param active_power OUT active power
 * @param timeout_ms Timeout in milliseconds for receiving a response from the meter
 * @return esp_err_t
        - ESP_OK on success
        - ESP_ERR_TIMEOUT if no message was received within the timeout
        - ESP_ERR_INVALID_ARG if the buffer too short
        - ESP_ERR_INVALID_RESPONSE
        - ESP_ERR_INVALID_CRC
 */
esp_err_t ce2727a_get_active_power(ce2727a_handle_t handle, uint32_t * active_power, uint16_t timeout_ms);

/**
 * @brief Get energy readings
 *
 * Get energy readings
 *
 * @param handle Driver handle obtained from ce2727a_init()
 * @param readings Destination for readings
 * @param timeout_ms Timeout in milliseconds for receiving a response from the meter
 * @return esp_err_t
        - ESP_OK on success
        - ESP_ERR_TIMEOUT if no message was received within the timeout
        - ESP_ERR_INVALID_ARG if the buffer too short
        - ESP_ERR_INVALID_RESPONSE
        - ESP_ERR_INVALID_CRC
 */
esp_err_t ce2727a_get_energy(ce2727a_handle_t handle, ce2727a_readings_energy_t * readings, uint16_t timeout_ms);

#ifdef __cplusplus
}
#endif
