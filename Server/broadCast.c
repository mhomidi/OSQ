#include "broadCast.h"



void broadCast() {
    int inGame = TRUE;
    int indexOfQuestion = 0;
    while (inGame) {
        struct timeval interval;
        char buff[BUF_SIZE] = {0};

        interval.tv_sec = 1;
        interval.tv_usec = 0;
        //clear the socket set
        FD_ZERO(&readfds);
//        FD_ZERO(&wrfds);

        FD_SET(0, &readfds);
        //add master socket to set
        FD_SET(master_socket, &readfds);
//        FD_SET(master_socket, &wrfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = clients[i].socket_id;

            //if valid socket descriptor then add to read list
            if(sd > 0) {
                FD_SET(sd, &readfds);
//                FD_SET(sd, &wrfds);
            }

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        if (checkSeconds())
        {
            sendQuestions(indexOfQuestion);
            indexOfQuestion++;
        }
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &interval);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

    }
}

int checkSeconds() {
    if (tm.tm_sec > sec + 10) {
        sec = tm.tm_sec;
        return TRUE;
    }
    return FALSE;
}

void sendQuestions(int index) {
    for (i = 0; i < max_clients; i++)
    {
        int sd = clients[i].socket_id;
        char kaka[100] = "salama? \n khobin";
        if (sd != -1 && FD_ISSET( sd , &readfds)) {
            createResponseBuffer(kaka, 'q');
            send(sd, kaka, strlen(kaka), 0);
        }

    }
}
