/* =============================================================================
 *                          ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : server.c
 *   Author     : MrZloHex
 *   Date       : 2025-02-01
 *
 *   Description:
 *      
 *
 * =============================================================================
 */

#include "server.h"

#include "protocol.h"
#include "trace.h"
#include "util.h"
#include "weather.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void
onopen(ws_cli_conn_t client)
{
    char *cli, *port;
    cli  = ws_getaddress(client);
    port = ws_getport(client);
    TRACE_INFO("CONNECTION OPENED");
    TRACE_DEBUG("addr: %s", cli);
    TRACE_DEBUG("port: %s", port);
}

void
onclose(ws_cli_conn_t client)
{
    char *cli;
    cli = ws_getaddress(client);
    TRACE_INFO("CONNECTION CLOSED");
    TRACE_DEBUG("addr: %s", cli);
}

void
send_responce(ws_cli_conn_t client, MLP_Msg mlp_msg, int type)
{
    char *dump = mlp_dump(mlp_msg);
    TRACE_INFO("\033[3;36mSEND\033[0m\t%s", dump);
    free(dump);

    char *msg= mlp_make_msg(mlp_msg);
    size_t size = strlen(msg);
    TRACE_DEBUG("size: %" PRId64 "", size);
    TRACE_DEBUG("type: %d", type);
    TRACE_DEBUG("from: %s", ws_getaddress(client));
    ws_sendframe(client, msg, size, type);
    free(msg);
}

void
onmessage
(
    ws_cli_conn_t client,
	const unsigned char *raw, uint64_t size, int type
)
{
    char *msg = calloc(size, sizeof(char));
    if (!msg)
    {
        TRACE_ERROR("MALLOC");
        exit(1);
    }
    strncpy(msg, (char *)raw, size-1);

    char *cli = ws_getaddress(client);
    TRACE_DEBUG("RAW RECV `%s`", raw);
    TRACE_DEBUG("size: %" PRId64 "", size);
    TRACE_DEBUG("type: %d", type);
    TRACE_DEBUG("from: %s", cli);

    MLP_Msg mlp_msg = mlp_parse_msg(msg);

    char *dump = mlp_dump(mlp_msg);
    TRACE_INFO("\x1b[3;35mRECV\x1b[0m\t%s", dump);
    free(dump);

    if (mlp_msg.type == MT_CMD)
    {
        if (mlp_msg.action == MA_REG)
        {
            MLP_Msg mlp =
            {
                .type = MT_OK,
                .action = mlp_msg.action,
                .params = { mlp_msg.params[0] }
            };
            send_responce(client, mlp, type);
        }

        if (mlp_msg.action == MA_HRT)
        {
            MLP_Msg mlp =
            {
                .type = MT_OK,
                .action = mlp_msg.action,
                .params = { mlp_msg.params[0] }
            };
            send_responce(client, mlp, type);
        }

        if (mlp_msg.action == MA_GET)
        {
            if (strcmp(mlp_msg.params[0], "TIME") == 0)
            {
                time_t rawtime;
                struct tm *t;
                time(&rawtime);
                t = localtime(&rawtime);

                char hour[3], min[3], sec[3];
                int_to_charset(t->tm_hour, hour, 2);
                int_to_charset(t->tm_min,  min,  2);
                int_to_charset(t->tm_sec,  sec,  2);
                
                MLP_Msg mlp = 
                {
                    .type = MT_OK,
                    .action = mlp_msg.action,
                    .params = 
                    {
                        mlp_msg.params[0],
                        hour, min, sec
                    }
                };
                send_responce(client, mlp, type);
            }
            if (strcmp(mlp_msg.params[0], "DATE") == 0)
            {
                time_t rawtime;
                struct tm *t;
                time(&rawtime);
                t = localtime(&rawtime);
                t->tm_year -= 100;
                t->tm_mon  += 1;

                char year[5] = { '2', '0' }, mon[3], day[3];
                int_to_charset(t->tm_year, year+2, 2);
                int_to_charset(t->tm_mon,  mon,    2);
                int_to_charset(t->tm_mday, day,    2);

                MLP_Msg mlp = 
                {
                    .type = MT_OK,
                    .action = mlp_msg.action,
                    .params = 
                    {
                        mlp_msg.params[0],
                        year, mon, day
                    }
                };

                send_responce(client, mlp, type);
            }
            if (strcmp(mlp_msg.params[0], "WEATHER") == 0)
            {
                Weather wthr = curr_weather();

                char feels[4], temp[4], hum[4], code[4];
                int_to_charset(wthr.temp,       temp,  1);
                int_to_charset(wthr.feels_temp, feels, 1);
                int_to_charset(wthr.humidity,   hum ,  1);
                int_to_charset(wthr.code,       code,  1);

                MLP_Msg mlp =
                {
                    .type = MT_OK,
                    .action = mlp_msg.action,
                    .params = 
                    {
                        mlp_msg.params[0],
                        temp, feels, hum, code
                    }
                };

                send_responce(client, mlp, type);
            }
        }
    }

    free(msg);
}

