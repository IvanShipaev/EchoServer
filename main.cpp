#include "server.h"
#include <getopt.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    unsigned serverPort = 5000;
    struct in_addr serverAddr = {.s_addr = INADDR_ANY};

    int res;
    while ((res = getopt(argc, argv, "p:i:h")) != -1) {
        switch(res) {
        case 'p':
        {
            unsigned tmp = atoi(optarg);
            if (tmp > 0 && tmp < 0xffff)
                serverPort = tmp;
        }
        break;
        case 'i':
        {
            struct in_addr tmp;
            if (inet_aton(optarg, &tmp) != 0)
                 serverAddr = tmp;
        }
        break;
        case 'h':
            printf("Usage: OPTIONS\n"
                   " OPTIONS:\n"
                   "  -p   Port to listen, default 5000\n"
                   "  -i   IP address for interface, default \"0.0.0.0\" (INADDR_ANY)\n"
                   "  -h   Help\n");
            exit(0);
        break;
        }
    }

    try
    {
        Server server(serverAddr, serverPort);
        ev::default_loop loop;
        loop.run(0);
    }
    catch(const char* e)
    {
        printf("THROW Error - %s\n", e);
    }

    return 0;
}
