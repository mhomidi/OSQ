#include "client.h"



int main(int argc , char *argv[])
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    int port;
    if(argc != 3) {
        printConsole("Arguments should be 3");
        return 0;
    }
    port = atoi(argv[1]);

//set of socket descriptors
    fd_set readfds;
    fd_set wrfds;
    for (int i = 0; i < BUF_SIZE; ++i) {
        globalBuffer[i] = 0;
    }
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printConsole("socket creation failed...\n");
        exit(0);
    }
    else
        printConsole("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(port);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printConsole("connection with the server failed...\n");
        exit(0);
    }
    else
        printConsole("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}

