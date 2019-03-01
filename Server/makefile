all: Server

Server : main.o server.o
	gcc main.o server.o -o Server

server.o : server.c server.h
	gcc -c server.c

main.o : main.c server.h
	gcc -c main.c

clean:
	rm *.o Server

