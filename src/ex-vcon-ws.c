/*
 * Copyright (C) 2016-2023 Davidson Francis <davidsondfgl@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ws.h>
#include <mjson.h>
#include <string.h>
#include <time.h>
#include <wheather.h>

void
onopen(ws_cli_conn_t client)
{
	char *cli, *port;
	cli  = ws_getaddress(client);
	port = ws_getport(client);
	printf("Connection opened, addr: %s, port: %s\n", cli, port);
}

void
onclose(ws_cli_conn_t client)
{
	char *cli;
	cli = ws_getaddress(client);
	printf("Connection closed, addr: %s\n", cli);
}

void
send_responce(ws_cli_conn_t client,
    const char *msg, int type)
{
	printf("Responced: %s (size: %" PRId64 ", type: %d), from: %s\n",
		msg, strlen(msg), type, ws_getaddress(client));
    ws_sendframe(client, msg, strlen(msg), type);
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
	const unsigned char *msg, uint64_t size, int type)
{
	char *cli = ws_getaddress(client);
	printf("I receive a message: %s (size: %" PRId64 ", type: %d), from: %s\n",
		msg, size, type, cli);

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
core(void)
{
	ws_socket(&(struct ws_server){
		.host = "0.0.0.0",
		.port = 8080,
		.thread_loop   = 0,
		.timeout_ms    = 1000,
		.evs.onopen    = &onopen,
		.evs.onclose   = &onclose,
		.evs.onmessage = &onmessage
	});
}

#include <unistd.h>
#include <sys/types.h>

int
main(void)
{
	pid_t pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "WE FUCKED UP\n");
		return 1;
	}

	if (pid == 0)
	{
		core();
	}

	return 0;
}
