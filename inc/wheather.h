#ifndef __WHEATHER_H__
#define __WHEATHER_H__

#include <stdint.h>

typedef struct
{
    int8_t   temp;
    int8_t   feels_temp;
    uint8_t  humidity;
    uint16_t code;
} Wheather;

Wheather
curr_wheather();

#endif /* __WHEATHER_H__ */
