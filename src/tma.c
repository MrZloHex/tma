#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ws.h>
#include <mjson.h>
#include <string.h>
#include <time.h>
#include <wheather.h>
#include <trace.h>

#include "tma.h"

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
    TRACE_INFO("\033[36mSEND\033[0m: `%s`", msg);
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
onmessage(ws_cli_conn_t client,
	const unsigned char *raw, uint64_t size, int type)
{
    char *cli = ws_getaddress(client);

    char *msg = malloc(size);
    strcpy(msg, raw);
    if (*msg == '{')
    {
        msg[size-1] = 0;
        TRACE_INFO("\033[35mRECV\033[0m: `%s`", msg);
    }
    else
    {
        TRACE_DEBUG("RAW RECV: `%s`", msg);
    }
    TRACE_DEBUG("size: %" PRId64 "", size);
    TRACE_DEBUG("type: %d", type);
    TRACE_DEBUG("from: %s", cli);

    char buf[64] = { 0 };
    double id = 0;
    int n = mjson_get_string(msg, size, "$.method", buf, sizeof(buf));
    mjson_get_number(msg, size, "$.id", &id);

    if (strcmp(buf, "boot") == 0)
    {
        char *resp = mjson_aprintf("{%Q:%d,%Q:%Q}\n", "id", (int)id, "result", "ok");
        send_responce(client, resp, type);
    }
    if (strcmp(buf, "heartbeat") == 0)
    {
        char *resp = mjson_aprintf("{%Q:%d,%Q:%Q}\n", "id", (int)id, "result", "ok");
        send_responce(client, resp, type);
    }
    if (strcmp(buf, "time") == 0)
    {
        time_t rawtime;
        struct tm *t;
        time(&rawtime);
        t = localtime(&rawtime);
        t->tm_year -= 100;
        t->tm_mon  += 1;
        
        char *resp = mjson_aprintf
        (
            "{%Q:%d,%Q:\"20%M-%M-%MT%M:%M:%M.000Z\"}\n",
            "id", (int)id, "result",
            z2i, t->tm_year, z2i, t->tm_mon, z2i, t->tm_mday,
            z2i, t->tm_hour, z2i, t->tm_min, z2i, t->tm_sec
        );
        send_responce(client, resp, type);
    }
    if (strcmp(buf, "wheather") == 0)
    {
        Wheather wthr = curr_wheather();
        char *resp = mjson_aprintf
        (
            "{%Q:%d,%Q:{%Q:%d,%Q:%d,%Q:%d,%Q:%d}}\n",
            "id", (int)id, "result",
            "temp", wthr.temp, "feels", wthr.feels_temp,
            "humidity", wthr.humidity, "code", wthr.code
        );
        send_responce(client, resp, type);
    }
    if (strcmp(buf, "") == 0)
    {

    }
}


int
tma(int port)
{
    tracer_init(TRC_INFO, TP_TIME);
    TRACE_INFO("====================================================================");
    TRACE_INFO("STARTING TMA");
    ws_socket(&(struct ws_server){
        .host           = "0.0.0.0",
        .port           = port,
        .thread_loop    = 0,
        .timeout_ms     = 1000,
        .evs.onopen     = &onopen,
        .evs.onclose    = &onclose,
        .evs.onmessage  = &onmessage
    });
}
        
