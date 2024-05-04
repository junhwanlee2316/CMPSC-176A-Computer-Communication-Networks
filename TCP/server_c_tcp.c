/*
filename portno
argv[0] filename
argv[1] portno
*/

#include "server_c_tcp.h"


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{   
    /* Exit if port number is not provided */
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    /* Declare parameters*/
    int sockfd, newsockfd, portno, n;
    char buffer[256];

    /* Aquire internet address */
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;


    /*
    * AF_INET = Address family -> IPv4
    * SOCK_STREAM = TCP
    * 0 = default TCP protocol
    */

    /* Create a socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket.");

    /* Clear serv_addr */
    bzero((char *) &serv_addr, sizeof(serv_addr));

    /* Port number converted to int */
    portno = atoi(argv[1]);

    /* Initialize serv_addr */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    /* Begin Binding */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    /* Begin Listening */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    /* Begin Connecting */
    while (1) {

        /* Declare Parameters */
        int sum_first = 0;
        int i;
        int c = 0;

        bzero(buffer, 256);

        /* Begin Accepting */
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");

        /* Begin Reading */
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");

        /* Input validation and calculate the first sum */
        for(i = 0; buffer[i] != '\0'; i++){
            if((buffer[i] >= '0') && (buffer[i] <= '9')){
                sum_first += (buffer[i] - '0');
            }
            else c = c+ 1;
        }

        /* If the string is not a number */
        if(c > 1){
            n = write(newsockfd,"Sorry, cannot compute!",strlen("Sorry, cannot compute!"));
            if (n < 0) error("ERROR writing to socket");
        }

        /* If the string is a number */
        else{
            bzero(buffer,256);

            /* Declare the storage char array */
            char str[255];
            sprintf(str, "%d", sum_first);

            /* Begin writing the first sum */
            n = write(newsockfd,str,strlen(str));
            if (n  < 0) error("ERROR writing to socket");


            int sum = 10;
            /* Begin the digit sum loop */
            while(sum > 9){

                /* Delay */
                for(int k = 0; k < 10000000; k++){}

                sum = 0;
                for(i = 0; str[i] != '\0'; i++){
                    sum += (str[i] - '0');
                }

                /* Get the sum and write to client */
                bzero(str,255);
                sprintf(str, "%d" , sum);
                write(newsockfd,str,strlen(str));
            }
        }
    }
    
    /* Close */
    close(newsockfd);
    close(sockfd);
    return 0; 
}