
#include "client.h"



int isResponseOnSocked(char buffer[]) {
    if (buffer[0] == 'p')
        return TRUE;
    return FALSE;
}



void request(char buff[], int sockfd) {
    if (buff[1] == USERNAME_REENTER){
        int n = 2;
        printConsole("This username already exist. enter your username to signup again: ");
        buff[0] = REQUEST;
        buff[1] = SIGNUP;
        while ((buff[n++] = getchar()) != '\n');
        buff[n - 1] = '\0';
        send(sockfd, buff, BUF_SIZE, 0);
    }

    else if (buff[1] == LOGIN_REENTER) {
        int n = 2;
        printConsole("This username already doesn\'t exist. enter your username to login again: ");
        buff[0] = REQUEST;
        buff[1] = LOGIN;
        while ((buff[n++] = getchar()) != '\n');
        buff[n - 1] = '\0';
        send(sockfd, buff, BUF_SIZE, 0);
    }
    else if (buff[1] == SUCCESSFULLY) {
        int n = 0;
        printConsole("You login successfully. ");
        processWhatToDo(buff, sockfd);
    }

    else if (buff[1] == USER_NOT_FOUND) {
        printConsole("This user is not founded or online.");
        processWhatToDo(buff, sockfd);
    }

    else if (buff[1] == USER_FOUND) {
        printConsole("User is founded. Please start your chat:");
        processChating(buff, sockfd);
    }

}


void func(int sockfd)
{
    int n = 0;
    char buff[BUF_SIZE] = {0};
    printConsole("Select one of these:");
    printConsole("1.Signup");
    printConsole("2.Login");
    while(TRUE) {
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        if (buff[0] == '1') {
            int n = 2;
            printConsole("Please enter your username to signup: ");
            buff[0] = REQUEST;
            buff[1] = SIGNUP;
            while ((buff[n++] = getchar()) != '\n');
            buff[n - 1] = '\0';
            send(sockfd, buff, BUF_SIZE, 0);
            break;
        } else if (buff[0] == '2') {
            int n = 2;
            printConsole("Please enter your username to login: ");
            buff[0] = REQUEST;
            buff[1] = LOGIN;
            while ((buff[n++] = getchar()) != '\n');
            buff[n - 1] = '\0';
            send(sockfd, buff, BUF_SIZE, 0);
            break;
        } else
            errorInput();
    }
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
        FD_ZERO(&wrfds);
        FD_ZERO(&s_ex);

        //add master socket to set
        FD_SET(fileno(stdin), &readfds);
        FD_SET(sockfd, &readfds);
        FD_SET(sockfd, &wrfds);
        activity = select( sockfd + 1 , &readfds , &wrfds , &s_ex , NULL);
//        puts("salma");
        if (FD_ISSET( sockfd , &readfds) || FD_ISSET(fileno(stdin), &readfds)) {
            valread = read(sockfd, buff, BUF_SIZE);
            write(sockfd, buff, BUF_SIZE);
            if (valread) {
                puts("salam1");
                if (isResponseOnSocked(buff)) {
//                    char c;
//                    puts("salam2");
//                    c = getchar();
                    request(buff, sockfd);
                }
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

void showAndProcessGetDiamond(char buff[], int sockfd) {
    int n = 0;
    printConsole("Enter a number of way:");
    printConsole("1.Enter my email (1 diamond)");
    while (TRUE) {
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        if (buff[0] == '1' && buff[1] == '\n') {
            n = 2;
            printConsole("Please enter your email:");
            while ((buff[n++] = getchar()) != '\n');
            buff[0] = REQUEST;
            buff[1] = ADD_EMAIL;
            send(sockfd, buff, BUF_SIZE, 0);
            break;
        }
        else
            errorInput();
    }
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
    printConsole("Which work do you want to do? (Enter the Number)");
    printConsole("1. chat to another.");
    printConsole("2. Get more diamond.");
    printConsole("3. Show my diamond.");
    printConsole("4. Start competition.");
//    while (TRUE) {
//    {
//        int len;
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        buff[n - 1] = '\0';
//        if (FD_ISSET( fileno(stdin) , &readfds)) {
//            puts("kaka1");
//            getchar();
//            if (len = read(fileno(stdin), buff, 100)) {
//                buff[len] = 0;
//                printf("%s", buff);
//            }
            if (buff[0] == '2') {
                showAndProcessGetDiamond(buff, sockfd);
//                puts("kaka3");
//                break;
            }
            else if (buff[0] == '1') {
                showAndProcessChat(buff, sockfd);
//                puts("kaka4");
//                break;
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