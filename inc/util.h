/* =============================================================================
 *                          ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : util.h
 *   Author     : MrZloHex
 *   Date       : 2025-02-01
 *
 *   Description:
 *      Collection of utility functions
 *
 * =============================================================================
 */


#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdbool.h>

bool
is_valid_port(const char *port_str);

int
int_to_charset
(
	int  value,
	char set[],
	int  digits
);

void
reverse
(
	char *set,
	int len
);

#endif /* __UTIL_H__  */
