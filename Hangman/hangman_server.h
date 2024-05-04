/* hangman_server.h */

#ifndef HANGMAN_SERVER_H
#define HANGMAN_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#define BUFFER_SIZE 255
#define MAX_WORD_LENGTH 100
#define MAX_CLIENTS 3
#define FILE_NAME "hangman_words.txt"
#define MAX_GUESS 6
#define IS_MSG 1
#define NOT_MSG 0

typedef struct {
    int sockfd;
    pthread_t thread;
} ClientInfo;

void error(const char *msg);
void send_message(int sockfd, int msg_flag, const int word_length, int num_incorrect, const char* data);
void receive_message(int sockfd, char* buffer);
void extract_message(char* buffer, char* ptr);
void* handle_client(void* arg);
void get_word(char* word);

#endif /* HANGMAN_SERVER_H */
