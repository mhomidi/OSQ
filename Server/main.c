#include "server.h"



int main(int argc , char *argv[])
{
    int preGame = TRUE;
    t = time(NULL);
    tm = *localtime(&t);
    max_clients = 10;
    opt = TRUE;
    day = 5;
    hour = 22;
    min = 12;
    sec = 0;
    //a message
    char *message = "p0";
    a.tv_sec = 1;
    a.tv_usec = 0;
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        clients[i].socket_id = -1;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    printConsole("Waiting for connections ...");

    while(TRUE)
    {
        int check = TRUE;
        char buff[BUF_SIZE] = {0};
        //clear the socket set
        FD_ZERO(&readfds);
        t = time(NULL);
        tm = *localtime(&t);
//        FD_ZERO(&wrfds);

        FD_SET(0, &readfds);
        //add master socket to set
        FD_SET(master_socket, &readfds);
//        FD_SET(master_socket, &wrfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            clients[i].isInGame = TRUE;
            for (int j = 0; j < 5; ++j) {
                clients[i].answers[j] = FALSE;
            }
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
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &a);

        if ((activity < 0) && (errno!=EINTR))
        {
            printConsole("select error");
        }
        if (checkTimeOfGame() && preGame) {
            printConsole("The competition will start 10 sec later");
            for (int j = 0; j < max_clients; ++j) {
                if (clients[j].socket_id != -1) {
                    send(clients[j].socket_id, "pj", 10, 0);
                    if (clients[j].isInGame)
                        clients[j].diamond += 2;
                }
            }
            startTheGame();
            printConsole("The competition finished");
            for (int j = 0; j < max_clients; ++j) {
                if (clients[j].socket_id != -1) {
                    send(clients[j].socket_id, "pg", 10, 0);
                    if (clients[j].isInGame)
                        clients[j].diamond += 2;
                }
            }
            preGame = FALSE;
            check = FALSE;
        }
        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(master_socket, &readfds) && check)
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printConsole("New connection");

//            send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send");
            }

            printConsole("Welcome message sent successfully");

            //add new socket to array of sockets
            for (int k = 0; k < max_clients; k++)
            {
                //if position is empty
                if( clients[k].socket_id == -1 )
                {
                    clients[k].socket_id = new_socket;
                    clients[k].port = ntohs(address.sin_port);
                    printConsole("Adding to list of sockets\n");

                    break;
                }
            }
        }

        if (FD_ISSET(0, &readfds) && check)
        {
            if ((valread = read(0 , buffer, 1024)) != 0)
            {
                buffer[valread - 1] = 0;
                for (int i = 0; i < max_clients; i++) {
                    if (clients[i].socket_id != -1) {
                        send(clients[i].socket_id, buffer, strlen(buffer), 0);
                    }
                }
            }
        }
        //else its some IO operation on some other socket

        for (i = 0; i < max_clients; i++)
        {
            sd = clients[i].socket_id;
            if (sd != -1 && FD_ISSET( sd , &readfds)  && check)
            {
                //Check if it was for closing , and also read the
                //incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
                    printConsole("One of the socked disconnected");

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    clients[i].socket_id = -1;
                }

                    //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end
                    //of the data read
//                    buffer[valread] = '\0';
//                    send(sd , buffer , strlen(buffer) , 0 );
                    responseRequest(buffer, i);
                }
            }
        }
    }

    return 0;
}

