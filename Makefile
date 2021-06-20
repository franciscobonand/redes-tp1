all:
	gcc -Wall -c utils.c
	gcc -Wall -c common.c
	gcc -Wall -c handlers.c
	gcc -Wall client.c utils.o common.o -o client -lm
	gcc -Wall server.c utils.o common.o handlers.o -o server -lm

clean:
	rm utils.o common.o handlers.o client server
