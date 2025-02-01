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

int
int_to_charset
(
	int  value,
	char set[],
	int  digits
)
{
    int val = value;
    value = abs(value);
	int i = 0;
	while (value)
	{
		set[i++] = (value % 10) + '0';
		value /= 10;
	}

	while (i < digits)
	{
		set[i++] = '0';
	}

    if (val < 0)
    { set[i++] = '-'; }

	reverse(set, i);
	set[i] = '\0';
	return i;
}

void
reverse
(
	char *set,
	int len
)
{
	int i = 0;
	int j = len - 1;
	int temp;

	while (i < j)
	{
		temp = set[i];
		set[i] = set[j];
		set[j] = temp;

		i++;
		j--;
	}
}

