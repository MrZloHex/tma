/* =============================================================================
 *                          ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : crc.h
 *   Author     : MrZloHex
 *   Date       : 2025-02-01
 *
 *   Description:
 *
 * =============================================================================
 */


#ifndef __CRC_H__
#define __CRC_H__

#include <stdint.h>
#include <stddef.h>

uint32_t
crc32(const char *data, size_t length);

#endif /* __CRC_H__ */

#ifdef CRC_IMPL

uint32_t
crc32(const char *data, size_t length)
{
    uint32_t crc = 0xFFFFFFFF;

    for (size_t i = 0; i < length; i++)
    {
        crc ^= (uint32_t)data[i];

        for (size_t bit = 0; bit < 32; ++bit)
        {
            if (crc & 0x80000000)
            { crc = (crc << 1) ^ 0x04C11DB7; }
            else
            { crc <<= 1; }
        }
    }
    return crc;
}

#endif /* CRC_IMPL */
