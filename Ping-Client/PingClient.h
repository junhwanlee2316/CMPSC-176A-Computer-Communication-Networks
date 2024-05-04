#ifndef PINGCLIENT_H
#define PINGCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

void error(const char *);
int setup_socket(const char *hostname, int portno);
void close_socket(int sock);
void send_ping_requests(int sock, const char *hostname, int portno, int num_requests);
float* receive_ping_responses(int sock, int num_requests, int* received_count, int* loss_count);
void print_ping_statistics(const char *hostname, int num_requests, int received_count, int loss_count, float *RTTs);

#endif /* PINGCLIENT_H */
