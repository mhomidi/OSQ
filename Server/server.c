#include "broadCast.h"


int isRequestOnSocked(char buffer[]) {
    if (buffer[0] == REQUEST)
        return TRUE;
    return FALSE;
}

void responseRequest(char buff[], int client_index) {
    char subBuff[BUF_SIZE] = {0};
    if (buff[1] == ADD_USERNAME){
        if (isUsernameExist(&buff[2]) == -1) {
            strncpy(subBuff, &buff[2], BUF_SIZE - 2);
            strncpy(clients[client_index].name, subBuff, sizeof(subBuff));
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
        strncpy(subBuff, &buff[2], BUF_SIZE - 2);
        strncpy(clients[client_index].email, subBuff, sizeof(subBuff));
        clients[client_index].diamond++;
        send(clients[client_index].socket_id , "p2" , 3 , 0 );
    }

    else if (buff[1] == CHAT){
        int index;
//        printf("%d\n",isUsernameExist(&buff[2]));
        if ((index = isUsernameExist(&buff[2])) != -1 && clients[index].socket_id != 0){
            send(clients[index].socket_id , "p6" , 3 , 0 );
            send(clients[client_index].socket_id , "p6" , 3 , 0 );

        }
        else
            send(clients[client_index].socket_id , "p5" , 3 , 0 );
    }

    else if (buff[1] == TIME_GAME) {
        send(clients[client_index].socket_id , "pa1395/12/22" , 20 , 0 );
    }
    else if (buff[1] == GET_NUMBER_OF_DIAMOND) {
        char number[10];
        intToString(clients[client_index].diamond, number);
        createResponseBuffer(number, 'd');
        send(clients[client_index].socket_id , number , 20 , 0 );
    }
    else if (buff[1] == REPLY) {
        processReply(buff, client_index);
    }

}


void printString(char buff[]) {
    write(STDOUT_FILENO, buff, sizeof(buff));
    write(STDOUT_FILENO, "\n", sizeof(12));
    fflush(stdout);
}


void input(char* inputStr)
{
    int numBytes;
    do
    {
        numBytes = read(0, inputStr, sizeof(inputStr));
        inputStr[numBytes - 1] = '\0';
    }while(numBytes < 2);

}

int isUsernameExist(char username[]) {
    for (int j = 0; j < max_clients; ++j) {
        if (strcmp(username, clients[j].name) == 0)
            return i;
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
    puts(in);
}
