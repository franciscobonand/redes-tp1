all:
	gcc -Wall -c common.c
	gcc -Wall -c utils.c
	gcc -Wall client.c common.o -o client
	gcc -Wall server.c common.o -o server

clean:
	rm common.o utils.o client server
