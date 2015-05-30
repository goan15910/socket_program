// Program to set up UDP server process

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFF_SIZE 100
#define LINE_LEN 256


void raise_error(const char* msg)
{
    perror(msg);
    exit(1);
}


int main()
{
    //Declare buffers for user input
    char line[LINE_LEN];

    //Initialize server socket file descriptor
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        raise_error("Error opening socket");
  
    //Prompt user to enter the port number
    int port_num;
    printf("Enter the port number: ");
    if ( fgets(line, sizeof(line), stdin) )
        sscanf(line, "%d", &port_num);

    //Bind IP address and port number to socket
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port_num);
    dest.sin_addr.s_addr = INADDR_ANY;
    int bind_flag = bind(sockfd, (struct sockaddr *) &dest, sizeof(dest));
    if (bind_flag < 0)
        raise_error("Error on binding");

    //Set up remote address
    struct sockaddr_in remote_addr;
    socklen_t remote_len = sizeof(remote_addr);

    //Wait for messages from client
    printf("Waiting for messages from client ...\n");
    char buff[BUFF_SIZE];
    int rcv_len = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *) &remote_addr, &remote_len);
    printf("Received %d bytes\n", rcv_len);
    if (rcv_len > 0){
        buff[rcv_len] = 0;
        printf("Received message: \"%s\"\n", buff);
    }

    //Terminate the transfer
    close(sockfd);
    
    return 0;
}
