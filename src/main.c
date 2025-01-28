#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

#include "tma.h"

bool
is_valid_port(const char *port_str)
{
    for (int i = 0; port_str[i] != 0; ++i)
    {
        if (!isdigit(port_str[i]))
        { return 0; }
    }

    int port = atoi(port_str);
    return port > 0 && port <= 65535;
}

void
usage(const char *program_name)
{
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("\nOptions:\n");
    printf("  -p, --port <port>       Specify the port number to host on (1-65535).\n");
    printf("  -l, --log <logfile>     Specify the file to log all messages.\n");
    printf("  -f, --foreground        Run without forking.\n");
    printf("  -h, --help              Display this help message and exit.\n");
    printf("\nExample:\n");
    printf("  %s -p 8080 -l server.log\n", program_name);
}



int
main(int argc, char *argv[])
{
    int   port       = 0;
    bool  foreground = false;

    static struct option long_options[] = {
        {"port", required_argument, 0, 'p'},
        {"help", no_argument, 0, 'h'},
        {"foreground", no_argument, 0, 'f'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "p:h:f", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 'p':
            if (!is_valid_port(optarg))
            {
                fprintf(stderr, "Error: Invalid port number '%s'.\n", optarg);
                usage(argv[0]);
                return 1;
            }
            port = atoi(optarg);
            break;

        case 'f':
            foreground = true;
            break;

        case 'h':
            usage(argv[0]);
            return 0;

        default:
            fprintf(stderr, "Error: Unknown argument.\n");
            usage(argv[0]);
            return 1;
        }
    }

    if (port == 0)
    {
        fprintf(stderr, "Error: Missing required arguments.\n");
        usage(argv[0]);
        return 1;
    }

    if (foreground)
    {
        tma(port);
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Error to fork");
        return 1;
    }
    
    if (pid == 0)
    {
        tma(port);
    }

    return 0;
}

