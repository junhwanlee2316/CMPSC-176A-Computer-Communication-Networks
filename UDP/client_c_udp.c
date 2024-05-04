/*
filename server_ipaddress portno
argv[0] filename
argv[1] server_ipaddress
argv[2] portno
*/

#include "client_c_udp.h"

void error(const char *);
int main(int argc, char *argv[])
{
    /* Declare parameters */
    int sock, n;
    unsigned int length;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256];
    
    /* Check for the valid argv input */
    if (argc != 3) { printf("Usage: server port\n");
        exit(1);
    }

    /* Create a Client socket with UDP */
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");
    
    /* Get Host Information */
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp==0) error("Unknown host");
    
    /* Server Info */
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length=sizeof(struct sockaddr_in);

    /* Get the input */
    write(1, "Enter string: ", strlen("Enter string: "));
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    /* Send the message to the server */
    n=sendto(sock,buffer, strlen(buffer),0,(const struct sockaddr *)&server,length);
    if (n < 0) error("Sendto");

    /* Begin polling for message */
    while(1){

        /* Begin Receving */
        n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
        if (n < 0) error("recvfrom");

        /* If error */
        if (strncmp(buffer, "Sorry, cannot compute!", strlen("Sorry, cannot compute!")) == 0) {
            printf("From server: Sorry, cannot compute!\n");
            break;
        }
        
        /* Display the message from server*/
        write(1,"From server: ",strlen("From server: "));
        write(1,buffer,n);
        write(1,"\n",1);

        /* Stop receving if the message is a single digit */
        if (strlen(buffer) == 1 && isdigit(buffer[0])) {
            break; 
        }

        bzero(buffer,256);
    }

    /* Close */
    close(sock);
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}