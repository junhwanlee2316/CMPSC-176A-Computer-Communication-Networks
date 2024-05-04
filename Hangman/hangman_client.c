/*
filename server_ipaddress portno
argv[0] filename
argv[1] server_ipaddress
argv[2] portno
*/

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

void print_buffer(char* buffer);
int start_game(int sockfd);
void handle_client(int sockfd);
void send_message(int sockfd, const char* input);
int letter_guess(int sockfd);
void handle_EOF(int sockfd);


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{   
    /* Exit if not enough arguments provided */
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }
    /* Declare parameters */
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    /* Port number converted to int */
    portno = atoi(argv[2]);

    /* Create a socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* Check if the server is on */
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    
    /* Begin Connecting */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");


    handle_client(sockfd);

    return 0;
}

void handle_client(int sockfd){
    char buffer [BUFFER_SIZE];
    
    // Check for overload
    if(read(sockfd, buffer, BUFFER_SIZE) < 0) error("Error receiving data\n");

    if(strstr(buffer,"overloaded") != NULL){
        //print_buffer(buffer);
        printf(">>>server-overloaded\n");
        close(sockfd);
        return;
    }

    if(start_game(sockfd) == 1) {
        close(sockfd);
        return;
    }

    while(1){

        bzero(buffer, BUFFER_SIZE);
        if(read(sockfd, buffer, BUFFER_SIZE) < 0) error("Error reading message\n");
        print_buffer(buffer);

        if(strstr(buffer + 1, "The word was") != NULL){
            bzero(buffer, BUFFER_SIZE);
            read(sockfd, buffer, BUFFER_SIZE);
            print_buffer(buffer);
            bzero(buffer,BUFFER_SIZE);
            read(sockfd, buffer, BUFFER_SIZE);
            print_buffer(buffer);
            break;
        }


        bzero(buffer,BUFFER_SIZE);
        if(read(sockfd, buffer, BUFFER_SIZE)<0) error("Error reading message\n");
        print_buffer(buffer);
        printf(">>>\n");

        int k = letter_guess(sockfd);
        if(k == 1) break;
    }

    /* Close */
    close(sockfd);
}

void print_buffer(char* buffer){
    int msg_flag = buffer[0] - '0';
    buffer[BUFFER_SIZE - 1] = '\0';
    printf(">>>");
    if(msg_flag > 0){
        if(strstr(buffer + 1, "The word was") != NULL){
            printf("%s\n", buffer+1);    // >>>The word was d o g
        }else{
            printf("%s\n", buffer+1);    // >>>Incorrect Guesses: k s
        }
    }else{
        int word_length = buffer[1] - '0';
        for (int i = 0; i < word_length; i++) {
            if (isprint(buffer[i + 3])) {
                if (i != word_length - 1) {
                    printf("%c ", buffer[i+3]);
                }else{
                    printf("%c", buffer[i + 3]);   // >>>_ _ _
                }
            } else {
                if (i != word_length - 1) {
                    printf("_ ");
                }else{
                    printf("_ ");
                }
            }
        }
        printf("\n");
    }
}


int start_game(int sockfd){
    printf(">>>Ready to start game? (y/n): ");
    char input[10];
    int result = scanf("%s", input);

    if (result == EOF) {
        handle_EOF(sockfd);
        printf("\n");
        return 1;
    }
    if(input[0] == 'y'){
        send_message(sockfd, "0");
        return 0;
    }else{
        send_message(sockfd, "n");
        //close(socket);
        printf("Disconnected from server\n");
        return 1;
    }
}

void handle_EOF(int sockfd){
    char* end_signal = "end";
    send_message(sockfd, end_signal);
    //close(sockfd);
}

int letter_guess(int sockfd) {

    char guess[BUFFER_SIZE];
    int result;
    int i = 0;
    

    while(1){
        bzero(guess,BUFFER_SIZE);
  
        printf(">>>Letter to guess: ");
        result = scanf("%s", guess);

        if (result == EOF) {
            char end_message[BUFFER_SIZE] = "end";
            end_message[BUFFER_SIZE-1] = '\0';
            send_message(sockfd, end_message);
            printf("\n");
            return 1;
        }

        if (strlen(guess) != 1 || !islower(guess[0]) || isdigit(guess[0])){
            printf(">>>Error! Please guess one letter.\n");
            continue;
        }

        getchar();
        break;
    }

    send_message(sockfd, guess);
    return 0;
}

// Send message to server
void send_message(int sockfd, const char* input) {
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    int msg_length = strlen(input);
    sprintf(buffer, "%d%s", msg_length, input);
    if(write(sockfd,buffer, BUFFER_SIZE) < 0) error("Error Sending Message\n");
}