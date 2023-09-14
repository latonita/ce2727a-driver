# API Reference

## Header files

- [include/ce2727a.h](#file-includece2727ah)

## File include/ce2727a.h

## Structures and Types

| Type | Name |
| ---: | :--- |
| struct | [**ce2727a\_config\_t**](#struct-ce2727a_config_t) <br>_CE2727A driver configuration._ |
| typedef  | [**ce2727a\_config\_t**](#typedef-ce2727a_config_t)  <br>_CE2727A driver configuration._ |
| typedef struct ce2727a\_t \* | [**ce2727a\_handle\_t**](#typedef-ce2727a_handle_t)  <br>_CE2727A driver handle._ |
| struct | [**ce2727a\_readings\_energy\_t**](#struct-ce2727a_readings_energy_t) <br>_CE2727A readings._ |
| typedef  | [**ce2727a\_readings\_energy\_t**](#typedef-ce2727a_readings_energy_t)  <br>_CE2727A readings._ |

## Functions

| Type | Name |
| ---: | :--- |
|  esp\_err\_t | [**ce2727a\_deinit**](#function-ce2727a_deinit) (ce2727a\_handle\_t handle) <br>_Deinitialize the CE2727A driver._ |
|  esp\_err\_t | [**ce2727a\_get\_active\_power**](#function-ce2727a_get_active_power) (ce2727a\_handle\_t handle, uint32\_t \* active\_power, uint16\_t timeout\_ms) <br>_Get active power._ |
|  esp\_err\_t | [**ce2727a\_get\_energy**](#function-ce2727a_get_energy) (ce2727a\_handle\_t handle, [**ce2727a\_readings\_energy\_t**](#struct-ce2727a_readings_energy_t)\* readings, uint16\_t timeout\_ms) <br>_Get energy readings._ |
|  esp\_err\_t | [**ce2727a\_init**](#function-ce2727a_init) (const [**ce2727a\_config\_t**](#struct-ce2727a_config_t)\* config, ce2727a\_handle\_t \* out\_handle) <br>_Initialize the CE2727A driver._ |


## Structures and Types Documentation

### struct `ce2727a_config_t`

_CE2727A driver configuration._
Variables:

-  uint32\_t addr  <br>Target power meter address, default is 0x0 - broadcasting to all meters

-  uint32\_t password  <br>Target power meter password, default is 0x0

-  uint8\_t \* rx_buffer  <br>Receive buffer. Shall be enough to hold required responses

-  uint16\_t rx_buffer_size  <br>Size of receive buffer

-  uart\_config\_t uart_config  <br>UART port configuration parameters like baud rate and parity

-  uint8\_t uart_port  <br>UART port number

### typedef `ce2727a_config_t`

_CE2727A driver configuration._
```c
typedef struct ce2727a_config_t ce2727a_config_t;
```

### typedef `ce2727a_handle_t`

_CE2727A driver handle._
```c
typedef struct ce2727a_t* ce2727a_handle_t;
```

### struct `ce2727a_readings_energy_t`

_CE2727A readings._
Variables:

-  uint8\_t active_tariff  <br>Currently active tariff 1...4

-  uint32\_t t1  <br>Cumulative energy for tariff 1

-  uint32\_t t2  <br>Cumulative energy for tariff 2

-  uint32\_t t3  <br>Cumulative energy for tariff 3

-  uint32\_t t4  <br>Cumulative energy for tariff 4

### typedef `ce2727a_readings_energy_t`

_CE2727A readings._
```c
typedef struct ce2727a_readings_energy_t ce2727a_readings_energy_t;
```


## Functions Documentation

### function `ce2727a_deinit`

_Deinitialize the CE2727A driver._
```c
esp_err_t ce2727a_deinit (
    ce2727a_handle_t handle
) 
```

**Parameters:**


* `handle` Driver handle obtained from ce2727a\_init(), or NULL 


**Returns:**

esp\_err\_t ESP\_OK on success.
### function `ce2727a_get_active_power`

_Get active power._
```c
esp_err_t ce2727a_get_active_power (
    ce2727a_handle_t handle,
    uint32_t * active_power,
    uint16_t timeout_ms
) 
```

Get current active power



**Parameters:**


* `handle` Driver handle obtained from ce2727a\_init() 
* `active_power` OUT active power 
* `timeout_ms` Timeout in milliseconds for receiving a response from the meter 


**Returns:**

esp\_err\_t
* ESP\_OK on success
* ESP\_ERR\_TIMEOUT if no message was received within the timeout
* ESP\_ERR\_INVALID\_ARG if the buffer too short
* ESP\_ERR\_INVALID\_RESPONSE
* ESP\_ERR\_INVALID\_CRC
### function `ce2727a_get_energy`

_Get energy readings._
```c
esp_err_t ce2727a_get_energy (
    ce2727a_handle_t handle,
    ce2727a_readings_energy_t * readings,
    uint16_t timeout_ms
) 
```

Get energy readings



**Parameters:**


* `handle` Driver handle obtained from ce2727a\_init() 
* `readings` Destination for readings 
* `timeout_ms` Timeout in milliseconds for receiving a response from the meter 


**Returns:**

esp\_err\_t
* ESP\_OK on success
* ESP\_ERR\_TIMEOUT if no message was received within the timeout
* ESP\_ERR\_INVALID\_ARG if the buffer too short
* ESP\_ERR\_INVALID\_RESPONSE
* ESP\_ERR\_INVALID\_CRC
### function `ce2727a_init`

_Initialize the CE2727A driver._
```c
esp_err_t ce2727a_init (
    const ce2727a_config_t * config,
    ce2727a_handle_t * out_handle
) 
```

**Parameters:**


* `config` Pointer to the configuration struct. The driver makes a copy, so can point to a local variable. 
* `out_handle` Pointer to a variable to receive the driver handle. 


**Returns:**

esp\_err\_t ESP\_OK on success, ESP\_ERR\_NO\_MEM if out of memory.


