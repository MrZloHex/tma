/* =============================================================================
 *                          ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : server.h
 *   Author     : MrZloHex
 *   Date       : 2025-02-01
 *
 *   Description:
 *      
 *
 * =============================================================================
 */

#ifndef __SERVER_H__
#define __SERVER_H__

#include "ws/ws.h"

void
onopen(ws_cli_conn_t client);

void
onclose(ws_cli_conn_t client);

void
onmessage
(
    ws_cli_conn_t client, const unsigned char *raw,
    uint64_t size, int type
);

#endif /* __SERVER_H__ */
