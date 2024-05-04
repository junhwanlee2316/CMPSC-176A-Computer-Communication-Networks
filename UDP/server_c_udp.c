/*
filename portno
argv[0] filename
argv[1] portno
*/

#include "server_c_udp.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    /* Declare parameters */
    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[256];

    /* Check for the valid argv input */
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }

    /* Create the server socket UDP */
    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("Opening socket");
    length = sizeof(server);
    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(atoi(argv[1]));

    /* Begin Binding */
    if (bind(sock,(struct sockaddr *)&server,length)<0) 
        error("binding");
    fromlen = sizeof(struct sockaddr_in);

    /* Runs forever */
    while (1) {

        /* Declare parameters */
        int sum_first = 0;
        int i;
        int c = 0;
        bzero(buf, 256);

        /* Begin Receving */
        n = recvfrom(sock,buf,255,0,(struct sockaddr *)&from,&fromlen);
        if (n < 0) error("recvfrom");

        /* Input validation and calculate the first sum */
        for(i = 0; buf[i] != '\0'; i++){
            if((buf[i] >= '0') && (buf[i] <= '9')){
                sum_first += (buf[i] - '0');
            }
            else c = c+ 1;
        }

        /* If the message is not a number */
        if(c > 1){n = sendto(sock,"Sorry, cannot compute!",strlen("Sorry, cannot compute!"), 0,(struct sockaddr *)&from,fromlen);}

        /* If the message is a number */
        else{
            
            /* Create a storage char array */
            char str[sum_first];
            sprintf(str, "%d", sum_first);

            /* Begin Sending the first sum to client */
            n = sendto(sock,str,strlen(str), 0,(struct sockaddr *)&from,fromlen);
            if (n  < 0) error("sendto");

            /* Compute the sum */
            if(sum_first > 9){
                int sum;
                while(strlen(str) > 1){
                    sum = 0;
                    for(i = 0; str[i] != '\0'; i++){
                        sum += (str[i] - '0');
                    }
                    sprintf(str, "%d" , sum);

                    /* Send the sum to Client */
                    n = sendto(sock,str,strlen(str), 0,(struct sockaddr *)&from,fromlen);
                    if(n <0) error("error");
                }
            }
        }
    }

    /* Close */
    return 0;
}
