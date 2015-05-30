// Program to set up UDP client process

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LINE_LEN 256
#define IP_LEN 20
#define BUFF_SIZE 100

void raise_error(const char* msg)
{
    perror(msg);
    exit(1);
}


int main()
{
    //Declare buffers for user input
    char line[LINE_LEN];

    //Initialize socket file descriptor
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        raise_error("Error opening socket");

    //Prompt user to enter the port number
    int port_num;
    printf("Enter the port number: ");
    if ( fgets(line, sizeof(line), stdin) )
        sscanf(line, "%d", &port_num);

    //Prompt user to enter the server's IP address
    char server_ip[IP_LEN];
    printf("Enter the IP address of server: ");
    if ( fgets(line, sizeof(line), stdin) )
        sscanf(line, "%[^\n]s", server_ip);

    //Set up remote server address
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port_num);
    server.sin_addr.s_addr = inet_addr(server_ip);

    //Prompt user to enter the message to server
    printf("Enter the message: \n");
    char msg[BUFF_SIZE];
    if ( fgets(line, sizeof(line), stdin) )
        sscanf(line, "%[^\n]s", msg);
    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *) &server, sizeof(server));

    //Terminate the transfer
    close(sockfd);

}
