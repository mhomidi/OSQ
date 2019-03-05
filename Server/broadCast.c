#include "broadCast.h"



void broadCast() {
    int indexOfQuestion = 0;
    createQuestion();
    while (1) {
        struct timeval interval;
        char buff[BUF_SIZE] = {0};
        t = time(NULL);
        tm = *localtime(&t);
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
        puts(".");
        if (checkSeconds())
        {
            printf("%d\t", indexOfQuestion);
            if (indexOfQuestion == 5)
                break;
            sendQuestions(indexOfQuestion);
            indexOfQuestion++;
        }
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &interval);
        if ((activity < 0) && (errno!=EINTR))
        {
            printConsole("select error");
        }

        for (i = 0; i < max_clients; i++)
        {
            sd = clients[i].socket_id;

            if (sd != -1 && FD_ISSET( sd , &readfds) && clients[i].isInGame)
            {

                //Check if it was for closing , and also read the
                //incoming message
                if ((valread = read( sd , buff, 1024)) != 0) {
                    processReply(buff, i);
                }
                else
                {

                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
                    printConsole("One of the socked disconnected");

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    clients[i].socket_id = -1;
                }
            }
            else if (!clients[i].isInGame && sd != -1 && FD_ISSET( sd , &readfds)) {
                send(sd, "pzYou can't answer the questions.", BUF_SIZE, 0);
            }
        }

    }
}

int checkSeconds() {
    if (tm.tm_sec > sec + 4) {
        sec = tm.tm_sec;
        return TRUE;
    }
    return FALSE;
}

void sendQuestions(int index) {
    for (i = 0; i < max_clients; i++)
    {
        int sd = clients[i].socket_id;
        if (sd != -1 && FD_ISSET( sd , &readfds)) {
            for (int i = 100; i > 1; --i) {
                q[index][i] = q[index][i - 2];
            }
            q[index][0] = 'p';
            q[index][1] = 'q';
            send(sd, q[index], 100, 0);
            for (int i = 0; i < 100; ++i) {
                q[index][i] = q[index][i + 2];
            }
        }

    }
}

void createQuestion() {
    char q1[100] = "1.a?";
    char q2[100] = "2.b?";
    char q3[100] = "3.c?";
    char q4[100] = "4.d?";
    char q5[100] = "5.e?";

    strncpy(q[0], q1, 100);
    strncpy(q[1], q2, 100);
    strncpy(q[2], q3, 100);
    strncpy(q[3], q4, 100);
    strncpy(q[4], q5, 100);
}


void processReply(char buff[], int client_index) {
    if (buff[2] == '1' && !clients[client_index].answers[0]) {
        clients[client_index].answers[0] = TRUE;
        if (buff[3] != '1') {
            clients[client_index].diamond--;
            if (clients[client_index].diamond == 0) {
                clients[client_index].isInGame = FALSE;
            }
        }
    }
    else if (buff[2] == '1' && clients[client_index].answers[0]) {
        send(clients[client_index].socket_id, "pf1", BUF_SIZE, 0);
    }
    else if (buff[2] == '2' && !clients[client_index].answers[1]) {
        clients[client_index].answers[1] = TRUE;
        if (buff[3] != '1') {
            clients[client_index].diamond--;
            if (clients[client_index].diamond == 0) {
                clients[client_index].isInGame = FALSE;
            }
        }
    }
    else if (buff[2] == '2' && clients[client_index].answers[1])
        send(clients[client_index].socket_id, "pf2", BUF_SIZE, 0);

    else if (buff[2] == '3' && !clients[client_index].answers[2]) {
        clients[client_index].answers[2] = TRUE;
        if (buff[3] != '1') {
            clients[client_index].diamond--;
            if (clients[client_index].diamond == 0) {
                clients[client_index].isInGame = FALSE;
            }
        }
    }
    else if (buff[2] == '3' && clients[client_index].answers[2])
        send(clients[client_index].socket_id, "pf3", BUF_SIZE, 0);

    else if (buff[2] == '4' && !clients[client_index].answers[3]) {
        clients[client_index].answers[3] = TRUE;
        if (buff[3] != '1') {
            clients[client_index].diamond--;
            if (clients[client_index].diamond == 0) {
                clients[client_index].isInGame = FALSE;
            }
        }
    }
    else if (buff[2] == '4' && clients[client_index].answers[3])
        send(clients[client_index].socket_id, "pf4", BUF_SIZE, 0);
    else if (buff[2] == '5' && !clients[client_index].answers[4]) {
        clients[client_index].answers[4] = TRUE;
        if (buff[3] != '1') {
            clients[client_index].diamond--;
            if (clients[client_index].diamond == 0) {
                clients[client_index].isInGame = FALSE;
            }
        }
    }
    else if (buff[2] == '5' && clients[client_index].answers[4])
        send(clients[client_index].socket_id, "pf5", BUF_SIZE, 0);
}