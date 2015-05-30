// Program to set up TCP client process

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
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    //Connect to the remote server
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port_num);
    server.sin_addr.s_addr = inet_addr(server_ip);
    int connect_flag = connect(sockfd, (struct sockaddr*) &server, sizeof(server));
    if (connect_flag < 0)
        raise_error("Error connecting to server");

    //Prompt user to enter the message to server
    printf("Enter the message: \n");
    char msg[BUFF_SIZE];
    if ( fgets(line, sizeof(line), stdin) )
        sscanf(line, "%[^\n]s", msg);
    int write_flag = write(sockfd, msg, sizeof(msg));
    if (write_flag < 0)
        raise_error("Error writing to socket");

    //Wait for server's reply
    char buff[BUFF_SIZE];
    int read_flag = read(sockfd, buff, sizeof(buff));
    if (read_flag < 0)
        raise_error("Error reading from socket");
    printf("The message from server: %s\n", buff);

    //Terminate the transfer
    close(sockfd);

}
