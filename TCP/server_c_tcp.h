/* server_c_tcp.h */

#ifndef SERVER_C_TCP_H
#define SERVER_C_TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg);
int setup_server(int portno);
int accept_connection(int sockfd);
void close_connection(int sockfd);
void send_message(int sockfd, const char* message);
void receive_message(int sockfd, char* buffer);

#endif /* SERVER_C_TCP_H */
