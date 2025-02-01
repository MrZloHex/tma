/* =============================================================================
 *                          ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : util.c
 *   Author     : MrZloHex
 *   Date       : 2025-02-01
 *
 *   Description: 
 *      Collection of utility functions
 *
 * =============================================================================
 */

#include "util.h"

#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>

bool
is_valid_port(const char *port_str)
{
    if (!port_str || port_str[0] == '\0')
    { return false; }

    for (size_t i = 0; port_str[i] != '\0'; ++i)
    {
        if (!isdigit((unsigned char)port_str[i]))
        { return false; }
    }

    char *endptr;
    long port = strtol(port_str, &endptr, 10);
    if (*endptr != '\0' || port <= 0 || port > 65535)
    { return false; }
    return true;
}
