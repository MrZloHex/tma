/* =============================================================================
 *                          ░▒▓█ _TMA_ █▓▒░
 *
 *   File       : weather.c
 *   Author     : MrZloHex
 *   Date       : 2024-12-11
 *
 *   Description:
 *      
 *
 * =============================================================================
 */

#include "weather.h"

#include "mjson.h"
#include "curl.h"
#include "trace.h"


const static char *wttr_url = "wttr.in/Dolgoprudny?format=j1";

Weather
curr_weather()
{
    CurlResp resp = curl(wttr_url);
    int n = 0;

    char feel_temp[5];
    n = mjson_get_string(resp.mem.memory, resp.mem.size, "$.current_condition[0].FeelsLikeC", feel_temp, 5);
    if (n < 0)
    { TRACE_ERROR("FAILED TO PARSE FEELS"); }

    char temp[5];
    n = mjson_get_string(resp.mem.memory, resp.mem.size, "$.current_condition[0].temp_C", temp, 5);
    if (n < 0)
    { TRACE_ERROR("FAILED TO PARSE TEMP"); }

    char humidity[5];
    n = mjson_get_string(resp.mem.memory, resp.mem.size, "$.current_condition[0].humidity", humidity, 5);
    if (n < 0)
    { TRACE_ERROR("FAILED TO PARSE HUM"); }

    char code[5];
    n = mjson_get_string(resp.mem.memory, resp.mem.size, "$.current_condition[0].weatherCode", code, 5);
    if (n < 0)
    { TRACE_ERROR("FAILED TO PARSE CODE"); }

    curl_clean(&resp);


    return (Weather)
    {
        .feels_temp = atoi(feel_temp),
        .temp       = atoi(temp),
        .humidity   = atoi(humidity),
        .code       = atoi(code)
    };
}
