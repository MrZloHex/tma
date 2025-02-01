/* =============================================================================
 *                          ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : protocol.h
 *   Author     : MrZloHex
 *   Date       : 2025-02-01
 *
 *   Description:
 *      MLP - MonoLith's Protocol
 *
 * =============================================================================
 */

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

typedef enum
{
    MT_UNKNOWN = 0x0U,
    MT_CMD     = 0x1U,
    MT_LOG     = 0x2U,
    MT_OK      = 0x3U,
    MT_ERR     = 0x4U
} MLP_MsgType;

typedef enum
{
    MA_UNKNOWN = 0x0U,
    MA_REG     = 0x1U,
    MA_GET     = 0x2U,
    MA_SET     = 0x3U,
    MA_HRT     = 0x4U,
} MLP_MsgAction;

typedef struct
{
    MLP_MsgType   type;
    MLP_MsgAction action;
    char         *param;
} MLP_Msg;


MLP_Msg
mlp_parse_msg(char *msg);

#endif /* __PROTOCOL_H__ */

#ifdef MLP_STRINGS

const static char *MLP_MsgType_STR[] =
{
    "UNKNOWN", "CMD", "LOG", "OK", "ERR"
};

const static char *MLP_MsgAction_STR[] =
{
    "UNKNOWN", "REG", "GET", "SET", "HRT"
};

#endif /* MLP_STRINGS */
