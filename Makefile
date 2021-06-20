all:
	gcc -Wall -c utils.c
	gcc -Wall -c common.c
	gcc -Wall -c handlers.c
	gcc -Wall client.c utils.o common.o -o cliente -lm
	gcc -Wall server.c utils.o common.o handlers.o -o servidor -lm

clean:
	rm utils.o common.o handlers.o cliente servidor
