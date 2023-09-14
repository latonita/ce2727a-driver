#pragma once

#include <stdio.h>

#pragma pack (1)
typedef struct {
    uint8_t two;
    uint8_t length;
    uint32_t address;
    uint32_t password;
    uint8_t com;
    uint8_t id;
} ce2727a_frame_header_t;

typedef struct {
    ce2727a_frame_header_t header;
    uint16_t crc16;
} ce2727a_request_command_t;

typedef struct {
    ce2727a_frame_header_t header;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t weekdayAndSummer;
    uint8_t dstAllowed;
    uint8_t correctionSecondsLeft;
    uint16_t crc16;
} ce2727a_response_date_time_t;

typedef struct {
    ce2727a_frame_header_t header;
    uint32_t activePower;
    uint16_t crc16;
} ce2727a_response_active_power_t;

typedef struct {
    ce2727a_frame_header_t header;
    uint8_t currentTariff;
    uint32_t total;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint16_t crc16;
} ce2727a_response_consumed_energy_t;
#pragma pack (0)
