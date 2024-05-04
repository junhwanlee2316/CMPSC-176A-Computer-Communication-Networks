#ifndef SERVER_C_UDP_H
#define SERVER_C_UDP_H

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *);
int setup_server_socket(int portno);
void close_server_socket(int sock);
void handle_client_requests(int sock);

#endif /* SERVER_C_UDP_H */
