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
 *   File       : main.c
 *   Author     : MrZloHex
 *   Date       : 2025-01-28
 *
 * ==============================================================================
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>

#include "tma.h"
#include "util.h"


void
usage(const char *program_name)
{
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("\nOptions:\n");
    printf("  -p, --port <port>       Specify the port number to host on (1-65535).\n");
    printf("  -f, --fork              Fork process.\n");
    printf("  -v, --verbose           More verbose logs.\n");
    printf("  -h, --help              Display this help message and exit.\n");
    printf("\nExample:\n");
    printf("  %s -p 8080 -f\n", program_name);
}


int
main(int argc, char *argv[])
{
    int   port       = 0;
    bool  verbose    = false;

    static struct option long_options[] =
    {
        {"port",        required_argument,  0, 'p'},
        {"verbose",     no_argument,        0, 'v'},
        {"fork",        no_argument,        0, 'f'},
        {"help",        no_argument,        0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "p:vfh", long_options, &option_index)) != -1)
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

        case 'v':
            verbose = true;
            break;

        case 'f':
            if (fork() > 0)
            { exit(0); }

            fclose(stdout);
            fclose(stdin);
            fclose(stderr);
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

    tma(port, verbose);

    return 0;
}

