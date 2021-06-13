#include "common.h"
#include "handlers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#define BUFSZ 500

struct Locations locs;

void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int listenToClient(int csock)
{
    char buf[BUFSZ];
    int kill = 0;
    const char *cmdReturn;

    for (;;)
    {
        bzero(buf, BUFSZ);
        size_t count = recv(csock, buf, BUFSZ - 1, 0);
        printf("[log] received: %s\n", buf);

        if (buf == NULL ||
            ((strlen(buf) == 5) && (strcmp(buf, "exit\n") == 0))) // last char is '\n'
            break;

        if ((strlen(buf) == 5) && (strcmp(buf, "kill\n") == 0)) // last char is '\n'
        {
            kill = 1;
            break;
        }

        cmdReturn = handleCommand(buf, &locs);
        // TODO: handle invalid command

        sprintf(buf, "< %s", cmdReturn);
        count = send(csock, buf, strlen(buf) + 1, 0);
        if (count != strlen(buf) + 1)
        {
            logexit("send");
        }
    }

    return kill;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }

    int serversock = socket(storage.ss_family, SOCK_STREAM, 0);
    if (serversock == -1)
    {
        logexit("socket");
    }

    int enable = 1;
    if (0 != setsockopt(serversock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        logexit("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(serversock, addr, sizeof(storage)))
    {
        logexit("bind");
    }

    if (0 != listen(serversock, 10))
    {
        logexit("listen");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("bound to %s, waiting connections\n", addrstr);

    // sets the current list of locations to length 0
    locs.currOccupancy = 0;
    // TODO: initialize all array elements with -1 -1

    while (1)
    {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        int csock = accept(serversock, caddr, &caddrlen);
        if (csock == -1)
        {
            logexit("accept");
        }

        char caddrstr[BUFSZ];
        addrtostr(caddr, caddrstr, BUFSZ);
        printf("[log] connection from %s\n", caddrstr);

        if (listenToClient(csock))
            break;

        close(csock);
    }

    printf("Shutting down server...\n");
    close(serversock);
    exit(EXIT_SUCCESS);
}