// Program to set up TCP server process

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LINE_LEN 256
#define BUFF_SIZE 100


void strrev(char* str)
{
    char *p1, *p2;
    if (str && *str)
        for (p1 = str, p2 = (str + strlen(str) - 1); p2 > p1; ++p1, --p2){
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
        }
}

void to_upper(char str[])
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
        if (islower(str[i]))
            str[i] = toupper(str[i]);
}

void raise_error(const char* msg)
{
    perror(msg);
    exit(1);
}


int main()
{
    //Declare line buffer for user input
    char line[LINE_LEN];

    //Initialize server socket file descriptor
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    //Listen for client connections on the socket
    listen(sockfd, 5);

    //Wait for the connection request from clients
    int clientfd;
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof(c_addr);
    int client_fd = accept(sockfd, (struct sockaddr*) &c_addr, &c_len);
    if (client_fd < 0)
        raise_error("Error on accept");

    //Wait for messages from client
    char buff[BUFF_SIZE];
    int read_flag = read(client_fd, buff, sizeof(buff));
    if (read_flag < 0)
        raise_error("Error reading from socket");

    //Reverse and Capitalize the message from client
    strrev(buff);
    to_upper(buff);

    //Send message back to client to confirm the receipt
    int write_flag = write(client_fd, buff, sizeof(buff));
    if (write_flag < 0)
        raise_error("Error writing to socket");

    //Terminate the transfer
    close(client_fd);
    close(sockfd);
    
    return 0;
}
