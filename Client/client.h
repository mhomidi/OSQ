// Write CPP code here
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#define SA struct sockaddr


//set of socket descriptors
fd_set readfds;
fd_set wrfds, s_ex;

//client
#define SIGNUP '1'
#define LOGIN '4'
#define REQUEST 'q'
#define ADD_EMAIL '2'
#define CHAT '3'
#define TIME_GAME '5'
#define GET_NUMBER_OF_DIAMOND '6'
#define REPLY 'r'


//server
#define START '0'
#define USERNAME '1'
#define SUCCESSFULLY '2'
#define USERNAME_REENTER '3'
#define LOGIN_REENTER '4'
#define USER_NOT_FOUND '5'
#define USER_FOUND '6'
#define WHAT_DO '7'
#define _SIGNUP '8'
#define _LOGIN '9'
#define GET_TIME 'a'
#define GET_DIAMOND 'b'
#define EMAIL 'c'
#define SHOW_NUMBER_OF_DIAMOND 'd'
#define QUESTION 'q'
#define REPLY_INCORRECT 'z'

#define TRUE 1
#define FALSE 0
#define PORT 8888
#define BUF_SIZE 1000



char globalBuffer[BUF_SIZE];


void func(int sockfd);
void printString(char buff[]);
void input(char* inputStr);
void showGetDiamond(char input[], int sockfd);
void showAndProcessChat(char buff[], int sockfd);
void errorInput();
void printConsole(char in[]);
void processWhatToDo(char buff[], int sockfd);
void processChating(char buff[], int sockfd);
void processSignup(char buff[], int sockfd);
void processLogin(char buff[], int sockfd);
void printWorks();
void processStartInput(char input[], char buff[]);
void createRequestBuffer(char* buff, char req);
void printWhatToDo();
void getTimeOfGame(int sockfd);
void processSendEmail(char input[], int sockfd);
void processGetDiamond(char input[], int sockfd);
void showClientDiamond(int sockfd);
void replyQuestion(char input[], char buff[],  int sockfd);