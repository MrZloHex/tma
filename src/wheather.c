#include "wheather.h"

#include <mjson.h>
#include <curl.h>


const static char *wttr_url = "wttr.in/Dolgoprudny?format=j1";

Wheather
curr_wheather()
{
    CurlResp resp = curl(wttr_url);
    int n = 0;

    char feel_temp[5];
    n = mjson_get_string(resp.mem.memory, resp.mem.size, "$.current_condition[0].FeelsLikeC", feel_temp, 5);
    if (n < 0)
    { printf("FAILED\n"); }

    char temp[5];
    n = mjson_get_string(resp.mem.memory, resp.mem.size, "$.current_condition[0].temp_C", temp, 5);
    if (n < 0)
    { printf("FAILED\n"); }

    char humidity[5];
    n = mjson_get_string(resp.mem.memory, resp.mem.size, "$.current_condition[0].humidity", humidity, 5);
    if (n < 0)
    { printf("FAILED\n"); }

    char code[5];
    n = mjson_get_string(resp.mem.memory, resp.mem.size, "$.current_condition[0].weatherCode", code, 5);
    if (n < 0)
    { printf("FAILED\n"); }

    curl_clean(&resp);


    return (Wheather)
    {
        .feels_temp = atoi(feel_temp),
        .temp       = atoi(temp),
        .humidity   = atoi(humidity),
        .code       = atoi(code)
    };
}
