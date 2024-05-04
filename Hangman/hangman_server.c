/*
filename portno
argv[0] filename
argv[1] portno
*/

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



pthread_mutex_t count_mutex;
pthread_t threads[MAX_CLIENTS];
int num_clients = 0;

void send_message(int sockfd, int msg_flag, const int word_length, int num_incorrect, const char* data);
void get_buffer(int msg_flag, int word_length, int num_incorrect, const char* data, char* buffer);
void receive_message(int sockfd, char* buffer);
void extract_message(char* buffer, char* ptr);
void* handle_client(void* arg);
void get_word(char* word);


typedef struct {
    int sockfd;
    pthread_t thread;
} ClientInfo;

ClientInfo clients[MAX_CLIENTS]; 

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


void send_message(int sockfd, int msg_flag, const int word_length, int num_incorrect, const char* data){
    char buffer[BUFFER_SIZE];
    if(msg_flag > 0){
        get_buffer(1,0,0,data, buffer);
    }else {
        int word_length = strlen(data);
        get_buffer(0, word_length, num_incorrect, data, buffer);
    }
    if(send(sockfd, buffer, strlen(buffer), 0) < 0) error("Error sending message\n");
}


void receive_message(int sockfd, char* buffer){
    bzero(buffer, BUFFER_SIZE);
    if(read(sockfd, buffer, BUFFER_SIZE)<0) error("Error reading message\n");
    extract_message(buffer, buffer);
}

void get_buffer(int msg_flag, int word_length, int num_incorrect, const char* data, char* buffer){
    if(msg_flag > 0){
        if(strlen(data) > BUFFER_SIZE - 1)error("Data exceeds the size limit");
        msg_flag = 1;
        snprintf(buffer,BUFFER_SIZE, "%d%s", msg_flag,data);
        buffer[BUFFER_SIZE-1] = '\0';
    }
    else{
        if(strlen(data)> BUFFER_SIZE - 3)error("Data exceeds the size limit");
        msg_flag = 0;
        snprintf(buffer, BUFFER_SIZE, "%d%d%d%s", msg_flag, word_length, num_incorrect, data);
        buffer[BUFFER_SIZE-1] = '\0';
    }
}

void extract_message(char* buffer, char* ptr){
    int ptrIndex = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (isalpha(buffer[i])) {
            ptr[ptrIndex] = buffer[i];
            ptrIndex++;
        }
    }
    ptr[ptrIndex] = '\0';  // Add the null terminator to the end of the extracted characters
}

void get_word(char* word){
    FILE* file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Count the number of words in the file
    int word_count = 15;
    char words[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", words) == 1) {
        word_count++;
    }

    int random_index = rand() % 15;

    rewind(file);
    int current_index = 0;
    while (fscanf(file, "%s", words) == 1) {
        if (current_index == random_index) {
            strcpy(word, words);
            break;
        }
        current_index++;
    }

    fclose(file);
}



void* handle_client(void* arg) {
    int newsockfd = *(int*)arg;

    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    const char* start_message = "hi";

    send_message(newsockfd,1,0,0,start_message);
    
    receive_message(newsockfd, buffer);


    printf("Recevied from client: %s\n", buffer);
    if(strcmp(buffer, "n") == 0 || strcmp(buffer, "end") == 0){
        printf("Disconnected from client\n");
        close(newsockfd);

        pthread_mutex_lock(&count_mutex);
        num_clients--;
        pthread_mutex_unlock(&count_mutex);

        pthread_exit(NULL);
    }


    char word[MAX_WORD_LENGTH];
    get_word(word);
    word[MAX_WORD_LENGTH-1] = '\0';
    const int word_length = strlen(word);
    int incorrect = 0;
    int correct = 0;
    char guess;
    printf("%s\n", buffer);

    char modified_word[MAX_WORD_LENGTH];
    for (size_t i = 0; i < word_length; i++) {
        modified_word[i] = '_';
    }
    modified_word[MAX_WORD_LENGTH-1] = '\0';

    char incorrect_message[BUFFER_SIZE] = "Incorrect Guesses:";
    incorrect_message[BUFFER_SIZE-1] = '\0';
    char dummy_message[BUFFER_SIZE] = "Incorrect Guesses: ";
    dummy_message[BUFFER_SIZE-1] = '\0';
    int i = 0;
    while(1){ 

        if(incorrect >= MAX_GUESS || correct == word_length){
            char ending_message[BUFFER_SIZE] = "The word was ";
            ending_message[BUFFER_SIZE-1] = '\0';
            strcat(ending_message, word);
            send_message(newsockfd,1,0,0,ending_message);
            sleep(1);
            
            if(incorrect >= MAX_GUESS) {
                char message[BUFFER_SIZE] = "You Lose!";
                message[BUFFER_SIZE-1] = '\0';
                send_message(newsockfd, 1, 0, 0, message);
            }else {
                char message[BUFFER_SIZE] = "You Win!";
                message[BUFFER_SIZE-1]='\0';
                send_message(newsockfd, 1, 0, 0, message);
            }
            sleep(1);
            char over_message[BUFFER_SIZE] = "Game Over!";
            over_message[BUFFER_SIZE-1] = '\0';
            send_message(newsockfd, 1, 0, 0, over_message);

            printf("Disconnected\n");
            break;
        }else{
            send_message(newsockfd, 0, word_length, incorrect, modified_word);
            printf("Modified word: %s\n", modified_word);
            sleep(1);
            printf("%s\n", incorrect_message);
            if(i == 0){
                send_message(newsockfd, 1,  0, 0, dummy_message);
            }else{
                send_message(newsockfd, 1,  0, 0, incorrect_message);
            }

            receive_message(newsockfd, buffer);
            if(strcmp(buffer,"end") == 0){
                printf("Disconnected\n");
                close(newsockfd);

                pthread_mutex_lock(&count_mutex);
                num_clients--;
                pthread_mutex_unlock(&count_mutex);

                pthread_exit(NULL);
            }
            
            guess = buffer[0];
            int found = 0;

            for (int i = 0; i < word_length; i++) {
                if (word[i] == guess) {
                    modified_word[i] = guess;
                    correct++;
                    found = 1;
                }
            }


            if (!found) {
                strcat(incorrect_message, " ");
                strcat(incorrect_message, buffer);
                incorrect_message[BUFFER_SIZE-1] = '\0';
                incorrect++;
                i = 1;
            }
        }

    }

    close(newsockfd);

    pthread_mutex_lock(&count_mutex);
    num_clients--;
    pthread_mutex_unlock(&count_mutex);
    
    pthread_exit(NULL);
}



void *client_handler(void *arg) {
    while (1) {
        int newsockfd;
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);

        newsockfd = accept(*((int *)arg), (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error accepting");
            continue;
        }

        pthread_mutex_lock(&count_mutex);
        if (num_clients >= MAX_CLIENTS) {
   
            char overloaded_message[BUFFER_SIZE] = "server-overloaded";
            overloaded_message[BUFFER_SIZE-1] = '\0';
            send_message(newsockfd, 1,0,0,overloaded_message);
    
            close(newsockfd);
            printf("Rejected new connection. Number of clients: %d\n", num_clients);
            pthread_mutex_unlock(&count_mutex);
            continue;
        }

        printf("Connection accepted %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        num_clients++;
        printf("Number of clients: %d\n", num_clients);

        clients[num_clients].sockfd = newsockfd;
        if (pthread_create(&threads[num_clients], NULL, handle_client, &newsockfd) != 0) {
            perror("Error creating thread");
            close(newsockfd);
            pthread_mutex_unlock(&count_mutex);
            continue;
        }
        //num_clients++;
        pthread_mutex_unlock(&count_mutex);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        error("No port provided\n");
    }

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    if (listen(sockfd, MAX_CLIENTS) == 0) {
        printf("[+] Listening....\n");
    } else {
        error("Server overloaded\n");
    }

    pthread_mutex_init(&count_mutex, NULL);

    pthread_t handler_thread;
    if (pthread_create(&handler_thread, NULL, client_handler, &sockfd) != 0) {
        error("Error creating client handler thread");
    }

    pthread_join(handler_thread, NULL);

    pthread_mutex_destroy(&count_mutex);

    /* Close */
    close(sockfd);
    return 0;
}