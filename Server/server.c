#include "broadCast.h"


int isRequestOnSocked(char aaaa[]) {
    if (aaaa[0] == REQUEST)
        return TRUE;
    return FALSE;
}

void responseRequest(char buff[], int client_index) {
    char subBuff[BUF_SIZE] = {0};
    if (buff[1] == ADD_USERNAME){
        if (isUsernameExist(&buff[2]) == -1) {
            strncpy(clients[client_index].name, buff + 2, BUF_SIZE);
            printWithoutEnter(clients[client_index].name);
            printConsole("sign up.");
            clients[client_index].diamond = 5;
//        printString(clients[client_index].name);
            send(clients[client_index].socket_id, "p2", 3, 0);
        }
        else {
            send(clients[client_index].socket_id, "p3", 3, 0);
        }
    }

    else if (buff[1] == LOGIN) {
        int index;
        if ((index = isUsernameExist(&buff[2])) != -1) {
            send(clients[client_index].socket_id, "p2", 3, 0);
            printWithoutEnter(clients[client_index].name);
            printConsole("login.");
            if (index != client_index) {
                strncpy(clients[index].name, clients[client_index].name, sizeof(clients[client_index].name));
                clients[index].diamond = clients[client_index].diamond;
                clients[index].socket_id = clients[client_index].socket_id;
                strncpy(clients[index].email, clients[client_index].email, sizeof(clients[client_index].email));
                clients[client_index].socket_id = -1;
            }
        }
        else
            send(clients[client_index].socket_id, "p4", 3, 0);
    }

    else if (buff[1] == ADD_EMAIL) {
        if (!clients[client_index].emailStatus) {
            printWithoutEnter(clients[client_index].name);
            printConsole("add his/her email.");
            strncpy(subBuff, &buff[2], BUF_SIZE - 2);
            strncpy(clients[client_index].email, subBuff, sizeof(subBuff));
            clients[client_index].diamond++;
            clients[client_index].emailStatus = TRUE;
            send(clients[client_index].socket_id, "p2", 3, 0);
        }
        else
            send(clients[client_index].socket_id, "pk", 3, 0);
    }

    else if (buff[1] == CHAT){
        int index;
        if ((index = isUsernameExist(&buff[2])) != -1 && clients[index].socket_id != -1){
            printWithoutEnter(clients[index].name);
            printWithoutEnter("and");
            printWithoutEnter(clients[client_index].name);
            printConsole("started to chat together");
            send(clients[index].socket_id , "p6" , 3 , 0 );
            send(clients[client_index].socket_id , "p6" , 3 , 0 );
            strncpy(clients[client_index].chatName, clients[index].name, BUF_SIZE);
            strncpy(clients[index].chatName, clients[client_index].name, BUF_SIZE);
        }
        else
            send(clients[client_index].socket_id , "p5" , 3 , 0 );
    }

    else if (buff[1] == TIME_GAME) {
        sendTimeOfGame(clients[client_index].socket_id );
    }
    else if (buff[1] == GET_NUMBER_OF_DIAMOND) {
        char number[10] = {0};
        intToString(clients[client_index].diamond, number);
        createResponseBuffer(number, 'd');
        send(clients[client_index].socket_id , number , 20 , 0 );
    }
    else if (buff[1] == REPLY) {
        printWithoutEnter(clients[client_index].name);
        printConsole("reply current question.");
        processReply(buff, client_index);
    }
    else if (buff[1] == SEND_CHAT) {
        int index;
        if ((index = isUsernameExist(clients[client_index].chatName)) != -1) {
            if(strcmp(buff + 2, "disconnect") == 0){
                send(clients[index].socket_id , "p2" , 3 , 0 );
                send(clients[client_index].socket_id , "p2" , 3 , 0 );
                strncpy(clients[client_index].chatName, "", BUF_SIZE);
                strncpy(clients[index].chatName, "", BUF_SIZE);
            }
            else {
                buff[0] = 'p';
                buff[1] = 'i';
                send(clients[index].socket_id, buff, BUF_SIZE, 0);
            }
        }

    }

}

int isUsernameExist(char username[]) {
    for (int j = 0; j < max_clients; ++j) {
        if (strcmp(username, clients[j].name) == 0)
            return j;
    }
    return -1;
}


void createResponseBuffer(char* buff, char res) {
    buff[strlen(buff) - 1] = 0;
    for (int i = strlen(buff); i > 1; --i) {
        buff[i] = buff[i - 2];
    }
    buff[0] = 'p';
    buff[1] = res;
}

void intToString(int a, char buff[]) {
    buff[2] = a / 100 + 48;
    buff[1] = (a / 10) % 10 + 48;
    buff[0] = a % 10 + 48;
    buff[3] = 0;
}

int checkTimeOfGame() {
    return (tm.tm_mday >= day) && (tm.tm_hour >= hour) && (tm.tm_min >= min) && (tm.tm_sec >= sec);
}

void startTheGame() {
    broadCast();
}


void printConsole(char in[]) {
//    puts(in);
    write(1, in, strlen(in) + 1);
    write(1, "\n", 2);
}

void sendTimeOfGame(int sockfd) {
    char c[BUF_SIZE] = {0};
    c[0] = 'p';
    c[1] = 'a';
    c[2] = hour / 10 + 48;
    c[3] = hour % 10 + 48;
    c[4] = ':';
    c[5] = min / 10 + 48;
    c[6] = min % 10 + 48;
    c[7] = ':';
    c[8] = sec / 10 + 48;
    c[9] = sec % 10 + 48;
    send(sockfd, c, BUF_SIZE, 0);

}

void printWithoutEnter(char in[]) {
    write(1, in, strlen(in) + 1);
    write(1, " ", 2);
}