// Program to implement Bulls & Cows server


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LINE_LEN 256
#define BUFF_SIZE 100


void raise_error(const char* msg);


int main()
{
    char line[LINE_LEN]; //buffer for user input
    char send_buff[BUFF_SIZE]; //buffer for sending messages to client
    char rcv_buff[BUFF_SIZE]; //buffer for receiving messages from client

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

    //........................................................

    //Wait for client to start the game
    read(client_fd, rcv_buff, sizeof(rcv_buff));

    //Generate 4 different digits
    char answer[] = "1234";

    //Prompt client to guess the 4 digits
    int end_of_game = 0;
    strcpy(send_buff, "The game starts! Guess the 4 digits!");
    write(client_fd, send_buff, sizeof(send_buff));

    while ( !end_of_game ){
        int a = 0;
        int b = 0;

        //Receive client's answer and check for Bulls & Cows
        read(client_fd, rcv_buff, sizeof(rcv_buff));

        for(int i = 0; i < 4; i++)
            if (rcv_buff[i] == answer[i])
                a++;

        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if ( rcv_buff[i] == answer[j] && i != j ){
                    b++;
                    break;
                }
        
        //Reply the client and check if the game ends
        sprintf(send_buff, "%dA%dB", a, b);
        write(client_fd, send_buff, sizeof(send_buff));
        if (a == 4)
            end_of_game = 1;
    }

    //........................................................

    //Terminate the transfer
    close(client_fd);
    close(sockfd);
    
    return 0;
}


void raise_error(const char* msg)
{
    perror(msg);
    exit(1);
}
