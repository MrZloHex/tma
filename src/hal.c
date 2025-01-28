#include <stdio.h>
#include <time.h>

void
get_timestamp(char *buffer, size_t buffer_size)
{
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    if (local_time == NULL)
    {
        perror("localtime");
        buffer[0] = '\0';
        return;
    }

    // Format the time as "YYYY-MM-DD HH:MM:SS"
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", local_time);
}
