#include "PingClient.h"


void error(const char *);
int main(int argc, char *argv[]) {
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
    
    
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    
    
    /* Server Info */
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length=sizeof(struct sockaddr_in);


    int size = 10;
    float RTTs[size];
    int loss = 0;
    // Send 10 ping requests to the server
    int seq = 1;

    //gettimeofday(&initial, NULL);
    time_t rawtime;
    struct tm * timeinfo;

    bzero(buffer,256);

    for (int i = 0; i < 10; i++) {
        
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        struct timeval tv_send, tv_receive;
        gettimeofday(&tv_send, NULL);
        sprintf(buffer, "PING %d", seq);
        

        int n =sendto(sock,buffer, strlen(buffer),0,(const struct sockaddr *)&server,length);
        if (n < 0) {
            perror("sendto failed");
            exit(1);
        }

        bzero(buffer,256);

        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);

        if (n < 0) {
            printf("Request timeout for seq#=%d\n", seq);
            RTTs[i] = 0;
            loss += 1;
        } else {
            gettimeofday(&tv_receive, NULL);
            float rtt_ms = ((tv_receive.tv_sec - tv_send.tv_sec)*1000000L + tv_receive.tv_usec) - tv_send.tv_usec;
            RTTs[i] = rtt_ms;
            printf("PING received from %s: seq#=%d time=%.3f ms\n", inet_ntoa(from.sin_addr), seq, rtt_ms);
        }

        bzero(buffer,256);


        // Sleep for 1 second
        sleep(1);

        // Increment sequence number
        seq++;
    }

    close(sock);

    int received = size - loss;
    int packet_loss = (loss*100) / size;
    float new_RTTs[received];
    int k = 0;
    for (int i = 0; i < size; i++) {   
        if(RTTs[i] > 0){
            new_RTTs[k] = RTTs[i];
            k = k + 1;
        }    
    }  

    float MIN = new_RTTs[0];
    float MAX = new_RTTs[0];
    float total = 0;

    for(int i = 0; i < received; i++){
        if(new_RTTs[i] < MIN){
            MIN = new_RTTs[i];
        }
        if(new_RTTs[i] > MAX){
            MAX = new_RTTs[i];
        }
        total += new_RTTs[i];
    }


    float AVG = (float)total/received;

    printf("--- %s ping statistics ---\n", argv[1]);
    
    if(received == 0){
        printf("%d packets transmitted, %d received, %d%% packet loss\n", size, received, packet_loss);
        return 0;
    }
    printf("%d packets transmitted, %d received, %d%% packet loss ", size, received, packet_loss);
    printf("rtt min/avg/max = %.3f %.3f %.3f ms\n",MIN,AVG,MAX);

    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}