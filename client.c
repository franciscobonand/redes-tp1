#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 500

void usage(int argc, char **argv)
{
	printf("usage: %s <server IP> <server port>\n", argv[0]);
	printf("example: %s 127.0.0.1 51511\n", argv[0]);
	exit(EXIT_FAILURE);
}

void runClient(int sfd)
{
	char buf[BUFSZ];

	for (;;)
	{
		bzero(buf, BUFSZ); // resets buffer
		printf("> ");
		fgets(buf, BUFSZ - 1, stdin);

		char trimmed[strlen(buf)];
		strcpy(trimmed, buf);

		size_t count = send(sfd, trimmed, strlen(trimmed) + 1, 0);
		if (count != strlen(trimmed) + 1)
		{
			logexit("error while sending message to server");
		}

		if ((strlen(trimmed) == 5) && // last char is '\n'
			(strcmp(trimmed, "exit\n") == 0 || strcmp(trimmed, "kill\n") == 0))
			break;

		bzero(buf, BUFSZ); // resets buffer
		read(sfd, buf, sizeof(buf));
		printf("%s\n", buf);

		if (strcmp(buf, "< error") == 0)
			break;
	};
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		usage(argc, argv);
	}

	struct sockaddr_storage storage;
	if (0 != addrparse(argv[1], argv[2], &storage))
	{
		usage(argc, argv);
	}

	int sockfd = socket(storage.ss_family, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		logexit("socket");
	}
	struct sockaddr *addr = (struct sockaddr *)(&storage);
	if (0 != connect(sockfd, addr, sizeof(storage)))
	{
		logexit("connect");
	}

	char addrstr[BUFSZ];
	addrtostr(addr, addrstr, BUFSZ);

	printf("connected to %s\n", addrstr);
	printf("to close the client type 'exit'\n");
	printf("to kill the server type 'kill'\n\n");

	runClient(sockfd);

	printf("Closing client...\n");
	close(sockfd);
	exit(EXIT_SUCCESS);
}