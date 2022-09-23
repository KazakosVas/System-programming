//
// Created by Bill on 9/19/2022.
//

#ifndef SYSPRO_NETWORK_H
#define SYSPRO_NETWORK_H

typedef struct client {

    struct sockaddr_in server;
    struct sockaddr *serverptr;
    struct hostent *rem;
    int sock, connection;

} client;


typedef struct server {
    struct sockaddr_in server, client;
    struct sockaddr *serverptr;
    struct sockaddr *clientptr;
    socklen_t clientlen;

    struct hostent *rem;
    int sock;
    int newsock;
} server;

client *client_constructor();
client *connect_to_server(int server_port, char *server_name);
server *server_constructor();
server *initialize_server(int port);
void destroy_server(void *new_server);

#define GET_HOST_BY_NAME_ERROR 1
#define CONNECT_ERROR 2

#endif //SYSPRO_NETWORK_H
