// Write CPP code here
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#define SA struct sockaddr


//client
#define SIGNUP '1'
#define LOGIN '4'
#define REQUEST 'q'
#define ADD_EMAIL '2'
#define CHAT '3'


//server
#define USERNAME '1'
#define SUCCESSFULLY '2'
#define USERNAME_REENTER '3'
#define LOGIN_REENTER '4'
#define USER_NOT_FOUND '5'

#define TRUE 1
#define FALSE 0
#define PORT 8888
#define BUF_SIZE 1000


void func(int sockfd);
void printString(char buff[]);
void input(char* inputStr);
void showAndProcessGetDiamond(char buff[], int sockfd);
void showAndProcessChat(char buff[], int sockfd);
void errorInput();
void printConsole(char in[]);
void processWhatToDo(char buff[], int sockfd);