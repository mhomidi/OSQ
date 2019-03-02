#include "server.h"



int main(int argc , char *argv[])
{
    max_clients = 10;
    opt = TRUE;
    //a message
    struct timeval a;
    char *message = "";
    a.tv_sec = 0;
    a.tv_usec = 500;
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        clients[i].socket_id = 0;
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
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    while(TRUE)
    {
        char buff[BUF_SIZE] = {0};
        //clear the socket set
        FD_ZERO(&readfds);
        FD_ZERO(&wrfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        FD_SET(master_socket, &wrfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = clients[i].socket_id;

            //if valid socket descriptor then add to read list
            if(sd > 0) {
                FD_SET(sd, &readfds);
                FD_SET(sd, &wrfds);
            }

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }
//
//        for (int j = 0; j < max_clients; ++j) {
//            printf("%d \t", clients[j].socket_id);
//            fflush(stdout);
//            if(clients[j].socket_id != 0 ) {
//                read(clients[j].socket_id, buff, sizeof(buff));
//                printf("\nis req: %d\n", isRequestOnSocked(buff));
//                if (isRequestOnSocked(buff)) {
//                    responseRequest(buff, j);
//                }
//                bzero(buff, BUF_SIZE);
//            }
//        }


        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket ,
                    inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

//            send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send");
            }

            puts("Welcome message sent successfully");

            //add new socket to array of sockets
            for (int k = 0; k < max_clients; k++)
            {
                //if position is empty
                if( clients[k].socket_id == 0 )
                {
                    clients[k].socket_id = new_socket;
                    clients[k].port = ntohs(address.sin_port);
                    printf("Adding to list of sockets as %d\n" , k);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)
        {
            sd = clients[i].socket_id;

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the
                //incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" ,
                           inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    clients[i].socket_id = 0;
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

