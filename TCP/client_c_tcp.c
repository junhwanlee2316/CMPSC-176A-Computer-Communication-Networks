/*
filename server_ipaddress portno
argv[0] filename
argv[1] server_ipaddress
argv[2] portno
*/

#include "client_c_tcp.h"

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
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct hostent *server;

    /* Port number converted to int */
    portno = atoi(argv[2]);

    /*
    * AF_INET = Address family -> IPv4
    * SOCK_STREAM = TCP
    * 0 = default TCP protocol
    */

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
    
    
    /* Get the string */
    bzero(buffer, 256);
    printf("Enter string: ");
    fgets(buffer, 255, stdin);

    /* Begin Writing */
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) error("ERROR writing to socket");
    bzero(buffer,256);

    /* Begin Reading */
    while(1){

        /* Read the message and store in buffer*/
        n = read(sockfd,buffer,255);
        if (n < 0) 
            error("ERROR reading from socket");

        /* If returned error */
        if (strncmp(buffer, "Sorry, cannot compute!", strlen("Sorry, cannot compute!")) == 0) {
            printf("From server: Sorry, cannot compute!\n");

            /* Close */
            close(sockfd);
            return 0;
        }

        /* If successfully caculated */
        else{
            printf("From server: %s\n", buffer);
            
            /* Close when a single digit is received */
            if(strlen(buffer) <= 1){
                close(sockfd);
                return 0;
            }
        }
        bzero(buffer,256);
    }

    /* Close */
    close(sockfd);
    return 0;
}