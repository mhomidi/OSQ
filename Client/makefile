all: Client

Client : main.o client.o
	gcc main.o client.o -o Client

client.o : client.c client.h
	gcc -c client.c

main.o : main.c client.h
	gcc -c main.c

clean:
	rm *.o Client

