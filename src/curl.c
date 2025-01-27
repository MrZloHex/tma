#include "curl.h"

#include <stdlib.h>

size_t
WriteMemoryCallback
(
    void *contents, size_t size, 
    size_t nmemb, void *userp
)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL)
    {
        printf("error: not enough memory\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


CurlResp
curl(const char *url)
{
    CurlResp resp = { 0 };
    CURLcode res;

    resp.mem.memory = malloc(1);
    resp.mem.size = 0;

    resp.handle = curl_easy_init();
    if (!resp.handle) {
        return resp;
    }
    curl_easy_setopt(resp.handle, CURLOPT_URL, url);
    curl_easy_setopt(resp.handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(resp.handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(resp.handle, CURLOPT_WRITEDATA, (void *)&resp.mem);
    curl_easy_setopt(resp.handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(resp.handle);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
        curl_clean(&resp);
    }

    return resp;
}

void
curl_clean(CurlResp *resp)
{
    curl_easy_cleanup(resp->handle);
    free(resp->mem.memory);
    resp->handle     = NULL;
    resp->mem.memory = NULL;
    resp->mem.size   = 0;
}
