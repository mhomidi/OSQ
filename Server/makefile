all: Server

Server : main.o server.o broadCast.o
	gcc main.o server.o broadCast.o -o Server

server.o : server.c server.h
	gcc -c server.c

main.o : main.c server.h
	gcc -c main.c

broadCast.o : broadCast.c broadCast.h
	gcc -c broadCast.c

clean:
	rm *.o Server

