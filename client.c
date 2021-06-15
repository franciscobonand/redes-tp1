#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 504 // 4 additional chars to identify msg size

void usage(int argc, char **argv)
{
	printf("usage: %s <server IP> <server port>\n", argv[0]);
	printf("example: %s 127.0.0.1 51511\n", argv[0]);
	exit(EXIT_FAILURE);
}

// receiveResponse returns 1 if error happened. Returns 0 otherwise
int receiveResponse(int sfd)
{
	char buf[BUFSZ];
	char reader[BUFSZ];

	bzero(buf, BUFSZ); // resets buffer
	bzero(reader, BUFSZ);
	size_t count = recv(sfd, reader, BUFSZ - 1, 0);

	getWholeMsg(sfd, reader, buf, count);

	if (strcmp(buf, "< b") == 0)
	{ // multiple returns are expected
		while (1)
		{
			bzero(buf, BUFSZ);
			bzero(reader, BUFSZ);
			count = recv(sfd, buf, sizeof(buf) - 1, 0);

			getWholeMsg(sfd, reader, buf, count);

			if (strcmp(buf, "< e") == 0)
				return 0;

			printf("< %s\n", buf);

			if (strcmp(buf, "< error") == 0)
				return 1;
		}
	}
	else
	{
		printf("< %s\n", buf);
	}

	if (strcmp(buf, "< error") == 0)
		return 1;

	return 0;
}

void runClient(int sfd)
{
	char buf[BUFSZ];

	for (;;)
	{
		bzero(buf, BUFSZ); // resets buffer
		printf("> ");
		fgets(buf, BUFSZ - 1, stdin);

		// int msgSize = strlen(buf);
		// char trimmed[msgSize + strlen(buf)];
		// sprintf(trimmed, "%d-", msgSize);
		// strcat(trimmed, buf);

		// size_t count = send(sfd, trimmed, strlen(trimmed) + 1, 0);
		size_t count = send(sfd, "24-add 123 123\nadd 321 321\n", strlen("24-add 123 123\nadd 321 321\n") + 1, 0);
		if (count != strlen("24-add 123 123\nadd 321 321\n") + 1) //trimmed
		{
			logexit("error while sending message to server");
		}

		if ((strlen(buf) == 5) && // last char is '\n'
			(strcmp(buf, "exit\n") == 0 || strcmp(buf, "kill\n") == 0))
			break;

		if (receiveResponse(sfd)) // returns true (1) if error occured
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