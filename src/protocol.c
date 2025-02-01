/* =============================================================================
 *                          ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : protocol.c
 *   Author     : MrZloHex
 *   Date       : 2025-02-01
 *
 *   Description:
 *      MLP - MonoLith's Protocol
 *
 * =============================================================================
 */

#include "protocol.h"

#include "trace.h"
#include <string.h>

static MLP_MsgType
mlp_get_msg_type(char *msg)
{
    char *end = strchr(msg, ':');
    if (!end)
    { TRACE_ERROR("FAIELD TO PARSE TYPE"); }

    *end = 0;
    TRACE_DEBUG("TYPE `%s`", msg);
    MLP_MsgType type = MT_UNKNOWN;
    
    if (strcmp(msg, "CMD") == 0)
    { type = MT_CMD; }
    if (strcmp(msg, "LOG") == 0)
    { type = MT_LOG; }
    if (strcmp(msg, "OK") == 0)
    { type = MT_OK; }
    if (strcmp(msg, "ERR") == 0)
    { type = MT_ERR; }

    *end = ':';
    return type;
}

static
MLP_MsgAction
mlp_get_msg_action(char *msg)
{
    msg = strchr(msg, ':') +1;
    char *end = strchr(msg, ':');
    if (!end || !msg)
    { TRACE_ERROR("FAILED TO PARSE ACTION"); }

    *end = 0;
    TRACE_DEBUG("ACT `%s`", msg);
    MLP_MsgAction action = MA_UNKNOWN;
    
    if (strcmp(msg, "REG") == 0)
    { action = MA_REG; }
    if (strcmp(msg, "GET") == 0)
    { action = MA_GET; }
    if (strcmp(msg, "SET") == 0)
    { action = MA_SET; }
    if (strcmp(msg, "HRT") == 0)
    { action = MA_HRT; }

    *end = ':';
    return action;
}

MLP_Msg
mlp_parse_msg(char *msg)
{
    MLP_MsgType   type   = mlp_get_msg_type(msg);
    MLP_MsgAction action = mlp_get_msg_action(msg);

    if (!type || !action)
    { TRACE_ERROR("FAILED TO PARSE MSG"); }

    return (MLP_Msg)
    {
        .type   = type,
        .action = action,
        .param  = strrchr(msg, ':') +1
    };
}




