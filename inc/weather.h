/* =============================================================================
 *                          ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : weather.h
 *   Author     : MrZloHex
 *   Date       : 2024-12-11
 *
 *   Description:
 *
 * =============================================================================
 */


#ifndef __WEATHER_H__
#define __WEATHER_H__

#include <stdint.h>

typedef struct
{
    int8_t   temp;
    int8_t   feels_temp;
    uint8_t  humidity;
    uint16_t code;
} Weather;

Weather
curr_weather();

#endif /* __WEATHER_H__ */
