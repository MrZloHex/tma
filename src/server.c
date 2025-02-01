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

#define MLP_STRINGS
#include "protocol.h"
#include "trace.h"
#include "mjson.h"
#include "wheather.h"

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
send_responce(ws_cli_conn_t client,
    char *msg, int type)
{
    size_t size = strlen(msg);
    char term = msg[size-1];
    msg[size-1] = 0;
    TRACE_INFO("\033[3;36mSEND\033[0m `%s`", msg);
    msg[size-1] = term;
    TRACE_DEBUG("size: %" PRId64 "", size);
    TRACE_DEBUG("type: %d", type);
    TRACE_DEBUG("from: %s", ws_getaddress(client));
    ws_sendframe(client, msg, size, type);
    free(msg);
}

static int
z2i
(
	mjson_print_fn_t fn,
	void *fndata,
	va_list *ap
)
{
	int value = va_arg(*ap, int);
	if (value < 10)
		return mjson_printf(fn, fndata, "0%u", value);
	else
		return mjson_printf(fn, fndata, "%u", value);
}

void
onmessage
(
    ws_cli_conn_t client,
	const unsigned char *raw, uint64_t size, int type
)
{
    char *cli = ws_getaddress(client);

    char *msg = malloc(size);
    if (!msg)
    {
        TRACE_ERROR("MALLOC");
        exit(1);
    }
    strcpy(msg, (char *)raw);

    TRACE_DEBUG("RAW RECV `%s`", raw);
    TRACE_DEBUG("size: %" PRId64 "", size);
    TRACE_DEBUG("type: %d", type);
    TRACE_DEBUG("from: %s", cli);

    MLP_Msg mlp_msg = mlp_parse_msg(msg);

    TRACE_INFO
    (
        "\x1b[3;35mRECV\x1b[0m %s %s `%s`",
        MLP_MsgType_STR[mlp_msg.type],
        MLP_MsgAction_STR[mlp_msg.action],
        mlp_msg.param
    );

    if (mlp_msg.type == MT_CMD)
    {
        if (mlp_msg.action == MA_REG)
        {
            char *resp = mjson_aprintf("{%Q:%Q}\n", "result", "ok");
            send_responce(client, resp, type);
        }

        if (mlp_msg.action == MA_HRT)
        {
            char *resp = mjson_aprintf("{%Q:%Q}\n", "result", "ok");
            send_responce(client, resp, type);
        }

        if (mlp_msg.action == MA_GET)
        {
            if (strcmp(mlp_msg.param, "TIME") == 0)
            {
                time_t rawtime;
                struct tm *t;
                time(&rawtime);
                t = localtime(&rawtime);
                t->tm_year -= 100;
                t->tm_mon  += 1;
                
                char *resp = mjson_aprintf
                (
                    "{%Q:\"20%M-%M-%MT%M:%M:%M.000Z\"}\n",
                    "result",
                    z2i, t->tm_year, z2i, t->tm_mon, z2i, t->tm_mday,
                    z2i, t->tm_hour, z2i, t->tm_min, z2i, t->tm_sec
                );
                send_responce(client, resp, type);
            }
            if (strcmp(mlp_msg.param, "WEATHER") == 0)
            {
                Wheather wthr = curr_wheather();
                char *resp = mjson_aprintf
                (
                    "{%Q:{%Q:%d,%Q:%d,%Q:%d,%Q:%d}}\n",
                    "result",
                    "temp", wthr.temp, "feels", wthr.feels_temp,
                    "humidity", wthr.humidity, "code", wthr.code
                );
                send_responce(client, resp, type);
            }
        }
    }
}

