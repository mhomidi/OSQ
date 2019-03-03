#include "server.h"


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
        if (index = isUsernameExist(&buff[2]) != -1) {
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