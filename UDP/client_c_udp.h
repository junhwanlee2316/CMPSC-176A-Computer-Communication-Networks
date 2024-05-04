#ifndef CLIENT_C_UDP_H
#define CLIENT_C_UDP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(const char *);
int setup_client_socket(const char *hostname, int portno);
void close_client_socket(int sock);
void communicate_with_server(int sock, const char *hostname, int portno);

#endif
