#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros


#define REQUEST 'q'
#define ADD_USERNAME '1'
#define ADD_EMAIL '2'
#define LOGIN '4'
#define CHAT '3'



#define TRUE 1
#define FALSE 0
#define PORT 8888
#define BUF_SIZE 1000


typedef struct client {
    char name[BUF_SIZE];
    int diamond;
    int socket_id;
    int port;
    char email[BUF_SIZE];
} Client;


int opt;
int master_socket , addrlen , new_socket , max_clients , activity, i , valread , sd;
int max_sd;
struct sockaddr_in address;
char buffer[1025]; //data buffer of 1K
Client clients[10];

//set of socket descriptors
fd_set readfds;
fd_set wrfds;

int isRequestOnSocked(char buff[]);
void responseRequest(char buff[], int socketReq);
void printString(char buff[]);
void input(char* inputStr);
int isUsernameExist(char username[]);