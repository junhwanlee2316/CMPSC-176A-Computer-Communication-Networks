/* hangman_client.h */

#ifndef HANGMAN_CLIENT_H
#define HANGMAN_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 255

void error(const char *msg);
void handle_client(int sockfd);
int start_game(int sockfd);
void handle_EOF(int sockfd);
int letter_guess(int sockfd);
void print_buffer(char* buffer);
void send_message(int sockfd, const char* input);

#endif /* HANGMAN_CLIENT_H */
