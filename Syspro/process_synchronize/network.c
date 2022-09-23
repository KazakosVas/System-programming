//
// Created by Bill on 9/19/2022.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h> /* sockets */
#include <netinet/in.h>
#include <netdb.h> /* gethostbyaddr */
#include <arpa/inet.h>

#include "../include/network.h"


/*------------------------Initialize Struct client declared in ../include/network.h------------------------*/
client * client_constructor ()
{
    client * new_client = malloc(sizeof (client));
    new_client->serverptr = (struct sockaddr*)&(new_client->server);
    return new_client;
}

/*-------------------------Initialize struct server declared in ../include/network.h------------------------*/
server * server_constructor()
{
    server * new_server = malloc(sizeof(server));
    new_server->serverptr = (struct sockaddr *)&(new_server->server);
    new_server->clientptr = (struct sockaddr *)&(new_server->client);
    new_server->clientlen = sizeof(new_server->client);

    return new_server;
}

/*-------------------------Connect to server--------------------------------------------*/
client * connect_to_server(int server_port, char * server_name)
{
    client * new_client = client_constructor();

    new_client->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(new_client->sock <0)
        perror("Error socket");

    new_client->rem = gethostbyname(server_name);

    if (new_client->rem == NULL) {
        herror("Get host by name");
        pthread_exit((void*) GET_HOST_BY_NAME_ERROR);
    }

    new_client->server.sin_family=AF_INET;
    memcpy(&new_client->server.sin_addr, new_client->rem->h_addr, new_client->rem->h_length);
    new_client->server.sin_port = htons(server_port);

    new_client->connection = connect(new_client->sock, new_client->serverptr, sizeof(new_client->server));


    if(new_client->connection<0) {
        perror("Error connection");
        pthread_exit((void *) CONNECT_ERROR);
    }
    return new_client;
}

/*-------------------------Initialize server--------------------------------------------*/

server * initialize_server(int port)
{
    server * new_server = server_constructor();

    /*Create Socket */
    new_server->sock = socket(AF_INET, SOCK_STREAM,0);
    if (new_server->sock <0)
    {
        perror("Socket");
        exit(1);
    }

    new_server->server.sin_family = AF_INET;                            //Internet Domain
    new_server->server.sin_addr.s_addr = htonl(INADDR_ANY);     //Receive requests from all IP
    new_server->server.sin_port = htons(port);                  //Server port

    /*Bind Socket to address and check if port available*/
    if (bind(new_server->sock, new_server->serverptr, sizeof(new_server->server)) < 0)
    {
        perror("bind");
        new_server->server.sin_port = htons(0);
        if (bind(new_server->sock, new_server->serverptr, sizeof(new_server->server)) < 0)
            perror("bind");
        else
        {
            struct sockaddr_in  sa;
            socklen_t sa_len;
            sa_len = sizeof(sa);

            if (getsockname(new_server->sock, (struct sockaddr * )&sa, &sa_len) == -1) {
                perror("getsockname() failed");
            }

            printf("Port %d was not available. System gave you port %d\n", port, ntohs(sa.sin_port));
            //printf("IP: %s\n", inet_ntoa(sa.sin_addr));
        }
    }

    /*Listen for Connections */
    if (listen(new_server->sock,2) <0)
        perror("listen");

    return new_server;
}

void destroy_server(void * new_server)
{
    close(((server * )new_server)->sock);
    free(new_server);
    printf("Server destroyed \n");

}