#ifndef __CURL_H__
#define __CURL_H__

#include <curl/curl.h>

typedef struct
{
    char *memory;
    size_t size;
} MemoryStruct;


typedef struct
{
    MemoryStruct mem;
    CURL *handle;
} CurlResp;


size_t
WriteMemoryCallback
(
    void *contents, size_t size, 
    size_t nmemb, void *userp
);


CurlResp
curl(const char *url);

void
curl_clean(CurlResp *resp);


#endif /* __CURL_H__ */
