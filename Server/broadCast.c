#include "broadCast.h"



void broadCast() {
    char **q = createQuestion();
    int inGame = TRUE;
    int indexOfQuestion = 0;
    while (inGame) {
        struct timeval interval;
        char buff[BUF_SIZE] = {0};
        t = time(NULL);
        tm = *localtime(&t);
        interval.tv_sec = 1;
        interval.tv_usec = 0;
        //clear the socket set
        FD_ZERO(&readfds);
//        FD_ZERO(&wrfds);
        puts("111111");
        puts(q[indexOfQuestion]);
        puts("222222");
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
            puts("karim benzema");
            sendQuestions(indexOfQuestion, q);
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

void sendQuestions(int index, char *quest[]) {
    for (i = 0; i < max_clients; i++)
    {
        int sd = clients[i].socket_id;
//        char kaka[100] = "salam? \n khobin";
        if (sd != -1 && FD_ISSET( sd , &readfds)) {
            createResponseBuffer(quest[index], 'q');
            send(sd, quest[index], strlen(quest[index]), 0);
        }

    }
}

char** createQuestion() {
    char q1[100] = "a?";
    char q2[100] = "b?";
    char q3[100] = "c?";
    char q4[100] = "d?";
    char q5[100] = "e?";

    strcpy(q[0], q1, 100);
    strcpy(q[1], q2, 100);
    strcpy(q[2], q3, 100);
    strcpy(q[3], q4, 100);
    strcpy(q[4], q5, 100);

    return q;
}