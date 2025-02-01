/* ==============================================================================
 *
 *  ███╗   ███╗ ██████╗ ███╗   ██╗ ██████╗ ██╗     ██╗████████╗██╗  ██╗
 *  ████╗ ████║██╔═══██╗████╗  ██║██╔═══██╗██║     ██║╚══██╔══╝██║  ██║
 *  ██╔████╔██║██║   ██║██╔██╗ ██║██║   ██║██║     ██║   ██║   ███████║
 *  ██║╚██╔╝██║██║   ██║██║╚██╗██║██║   ██║██║     ██║   ██║   ██╔══██║
 *  ██║ ╚═╝ ██║╚██████╔╝██║ ╚████║╚██████╔╝███████╗██║   ██║   ██║  ██║
 *  ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝   ╚═╝   ╚═╝  ╚═╝
 *
 *                           ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : tma.c
 *   Author     : MrZloHex
 *   Date       : 2024-12-28
 *
 * ==============================================================================
 */

#include "tma.h"

#include "trace.h"
#include "server.h"

int
tma(int port, bool verbose)
{
    tracer_init(verbose ? TRC_DEBUG : TRC_INFO, TP_TIME);

    TRACE_INFO("====================================================================");
    TRACE_INFO("STARTING TMA");
    TRACE_DEBUG("Verbose mode: %s", verbose ? "true" : "false");

    ws_socket
    (&(struct ws_server)
        {
            .host           = "0.0.0.0",
            .port           = port,
            .thread_loop    = 0,
            .timeout_ms     = 1000,
            .evs.onopen     = &onopen,
            .evs.onclose    = &onclose,
            .evs.onmessage  = &onmessage
        }
    );

    TRACE_FATAL("LOOP ENDED");

    return 1;
}

