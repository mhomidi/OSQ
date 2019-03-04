
#include "client.h"



int isResponseOnSocked(char buffer[]) {
    if (buffer[0] == 'p')
        return TRUE;
    return FALSE;
}


void showRespose(char buff[], int sockfd) {
    if (buff[1] == USERNAME_REENTER){
        printConsole("This username already exist. enter your username to signup again: ");
    }
    else if (buff[1] == START){
        printWorks();
    }

    else if (buff[1] == LOGIN_REENTER) {
        printConsole("This username already doesn\'t exist. enter your username to login again: ");
    }
    else if (buff[1] == SUCCESSFULLY) {
        int n = 0;
        printConsole("successfully. ");
        printWhatToDo();
    }

    else if (buff[1] == USER_NOT_FOUND) {
        printConsole("This user is not founded or online.");
        printWhatToDo();
    }

    else if (buff[1] == USER_FOUND) {
        printConsole("User is founded. Please start your chat:");
    }
    else if (buff[1] == GET_TIME) {
        printConsole(&buff[2]);
        printWhatToDo();
    }
    else if (buff[1] == SHOW_NUMBER_OF_DIAMOND) {
        printConsole(&buff[2]);
        printWhatToDo();
    }
}


void request(char buff[], char input[] , int sockfd) {
    if (buff[1] == USERNAME_REENTER){
        processSignup(buff, sockfd);
    }
    else if (buff[1] == START){
        processStartInput(input, buff);
    }

    else if (buff[1] == LOGIN_REENTER) {
        processLogin(input, sockfd);
    }
    else if (buff[1] == SUCCESSFULLY || buff[1] == GET_TIME ||
        buff[1] == USER_NOT_FOUND || buff[1] == SHOW_NUMBER_OF_DIAMOND) {
        processWhatToDo(input, sockfd);
    }

    else if (buff[1] == USER_FOUND) {
        processChating(input, sockfd);
    }
    else if (buff[1] == _LOGIN) {
        processLogin(input, sockfd);
    }
    else if (buff[1] == _SIGNUP) {
        processSignup(input, sockfd);
    }
    else if (buff[1] == EMAIL) {
        processSendEmail(input, sockfd);
    }
    else if (buff[1] == GET_DIAMOND) {
        processGetDiamond(input, sockfd);
    }
}


void func(int sockfd)
{
    int n = 0;
    char input[BUF_SIZE] = {0};
//    printConsole("Select one of these:");
//    printConsole("1.Signup");
//    printConsole("2.Login");
//    while(TRUE) {
//        n = 0;
//        while ((buff[n++] = getchar()) != '\n');
//        if (buff[0] == '1') {
//            int n = 2;
//            printConsole("Please enter your username to signup: ");
//            buff[0] = REQUEST;
//            buff[1] = SIGNUP;
//            while ((buff[n++] = getchar()) != '\n');
//            buff[n - 1] = '\0';
//            send(sockfd, buff, BUF_SIZE, 0);
//            break;
//        } else if (buff[0] == '2') {
//            int n = 2;
//            printConsole("Please enter your username to login: ");
//            buff[0] = REQUEST;
//            buff[1] = LOGIN;
//            while ((buff[n++] = getchar()) != '\n');
//            buff[n - 1] = '\0';
//            send(sockfd, buff, BUF_SIZE, 0);
//            break;
//        } else
//            errorInput();
//    }
    for (;;) {
        int activity, valread;
//        int n;
//        bzero(buff, sizeof(buff));
//        printf("Enter the string : ");
//        n = 0;
//        while ((buff[n++] = getchar()) != '\n')
//            ;
//        write(sockfd, buff, sizeof(buff));
//        bzero(buff, sizeof(buff));
//        read(sockfd, buff, sizeof(buff));
//        printf("From Server : %s", buff);
//        if ((strncmp(buff, "exit", 4)) == 0) {
//            printf("Client Exit...\n");
//            break;
//        }
//        getchar()
//        read(sockfd, buff, sizeof(buff));
        FD_ZERO(&readfds);
//        FD_ZERO(&wrfds);
//        FD_ZERO(&s_ex);

        //add master socket to set
        FD_SET(fileno(stdin), &readfds);
        FD_SET(sockfd, &readfds);
//        FD_SET(sockfd, &wrfds);
        activity = select( sockfd + 1 , &readfds , NULL , NULL , NULL);
//        puts("salma");
        if (FD_ISSET( sockfd , &readfds)) {
            valread = read(sockfd, globalBuffer, BUF_SIZE);
            if (valread) {
//                puts(buff);
                if (isResponseOnSocked(globalBuffer)) {
                    showRespose(globalBuffer, sockfd);
                }
            }
        }
        if (FD_ISSET(0, &readfds))
        {
            if ((valread = read(0 , input, BUF_SIZE)) != 0)
            {
                request(globalBuffer, input, sockfd);
//                printConsole(globalBuffer);
            }
        }
    }
}



void printString(char buff[]) {
    write(STDOUT_FILENO, buff, BUF_SIZE);
    write(STDOUT_FILENO, "\n", sizeof(12));
    fflush(stdout);
}


void input(char* inputStr)
{
    int numBytes;
    do
    {
        numBytes = read(0, inputStr, BUF_SIZE);
        inputStr[numBytes - 1] = '\0';
    }while(numBytes < 2);

    for (int i = BUF_SIZE - 3; i > -1; --i) {
        inputStr[i + 2] = inputStr[i];
    }

}

void showGetDiamond(char input[], int sockfd) {
    int n = 0;
    printConsole("Enter a number of way:");
    printConsole("1.Enter my email (1 diamond)");
    globalBuffer[1] = GET_DIAMOND;
//    while (TRUE) {
//        n = 0;
//        while ((buff[n++] = getchar()) != '\n');
//        if (buff[0] == '1' && buff[1] == '\n') {
//            n = 2;
//            printConsole("Please enter your email:");
//            while ((buff[n++] = getchar()) != '\n');
//            buff[0] = REQUEST;
//            buff[1] = ADD_EMAIL;
//            send(sockfd, buff, BUF_SIZE, 0);
//            break;
//        }
//        else
//            errorInput();
//    }
}

void showAndProcessChat(char buff[], int sockfd) {
    int n;
    printConsole("Enter username that you want to talk: ");
    n = 2;
    while ((buff[n++] = getchar()) != '\n');
    buff[n - 1] = '\0';
    buff[0] = REQUEST;
    buff[1] = CHAT;
    send(sockfd, buff, BUF_SIZE, 0);
}

void errorInput() {
    printConsole("Your input isn\'t correct");
}

void printConsole(char in[]) {
    puts(in);
}

void processWhatToDo(char buff[], int sockfd) {
    int n = 0;

            if (buff[0] == '2') {
                showGetDiamond(buff, sockfd);
            }
            else if (buff[0] == '1') {
                showAndProcessChat(buff, sockfd);
            }
            else if (buff[0] == '3') {
                getTimeOfGame(sockfd);
            }
            else if (buff[0] == '4') {
                showClientDiamond(sockfd);
            }
            else
                errorInput();
//        }
//    }
}


void processChating(char buff[], int sockfd) {
    int n = 0;
    while ((buff[n++] = getchar()) != '\n');
    buff[n - 1] = '\0';
}

void processSignup(char buff[], int sockfd){
    createRequestBuffer(buff, SIGNUP);
    send(sockfd, buff, BUF_SIZE, 0);
}

void processLogin(char buff[], int sockfd){
    createRequestBuffer(buff, LOGIN);
    send(sockfd, buff, BUF_SIZE, 0);
}

void printWorks() {
    printConsole("Select one of these:");
    printConsole("1.Signup");
    printConsole("2.Login");
}

void printWhatToDo() {
    printConsole("Select one that you want to do:");
    printConsole("1. Chat to another");
    printConsole("2. Get more diamond");
    printConsole("3. See the time of game");
    printConsole("4. Show my diamond");
}

void createRequestBuffer(char* buff, char req) {
    buff[strlen(buff) - 1] = 0;
    for (int i = strlen(buff); i > 1; --i) {
        buff[i] = buff[i - 2];
    }
    buff[0] = REQUEST;
    buff[1] = req;
}

void processStartInput(char input[], char buff[]) {
    if (input[0] == '1' && input[1] == '\n'){
        printConsole("Please enter your username to signup: ");
        buff[1] = '8';
    }
    else if (input[0] == '2' && input[1] == '\n') {
        printConsole("Please enter your username to login: ");
        buff[1] = '9';
    }
    else
        errorInput();
}

void getTimeOfGame(int sockfd) {
    char buff[10] = {0};
    buff[0] = REQUEST;
    buff[1] = TIME_GAME;
    send(sockfd, buff, 10, 0);
}

void processSendEmail(char input[], int sockfd) {
    createRequestBuffer(input, ADD_EMAIL);
    send(sockfd, input, BUF_SIZE, 0);
}

void processGetDiamond(char input[], int sockfd) {
    if (input[0] == '1'){
        printConsole("Please enter your email:");
        globalBuffer[1] = 'c';
    }
}

void showClientDiamond(int sockfd) {
    char buff[10] = {0};
    buff[0] = REQUEST;
    buff[1] = GET_NUMBER_OF_DIAMOND;
    send(sockfd, buff, 10, 0);
}