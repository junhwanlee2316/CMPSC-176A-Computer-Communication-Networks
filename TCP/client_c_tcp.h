/* client_c_tcp.h */

#ifndef CLIENT_C_TCP_H
#define CLIENT_C_TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>

void error(const char *msg);
int setup_connection(const char *hostname, int portno);
void close_connection(int sockfd);
void send_message(int sockfd, const char* message);
void receive_message(int sockfd, char* buffer);

#endif /* CLIENT_C_TCP_H */
