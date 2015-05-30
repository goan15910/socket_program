// Program to implement Bulls & Cows client

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

void raise_error(const char* msg);
void print_result(int cnt, const char* guess, const char* result);

int main()
{
    char line[LINE_LEN]; //buffer for user input
    char send_buff[BUFF_SIZE]; //buffer for sending messages to client
    char rcv_buff[BUFF_SIZE]; //buffer for receiving messages from client

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

    //.....................................................
   
    //Connect to server to start the game
    int end_of_game = 0;
    strcpy(send_buff, "start the game");
    write(sockfd, send_buff, sizeof(send_buff));

    //Recieve the message from server
    read(sockfd, rcv_buff, sizeof(rcv_buff));
    printf("%s\n", rcv_buff);

    int cnt = 1;
    while ( !end_of_game ){
        //Prompt the user to enter the 4 digits and send it to server
        printf("Enter the 4 digits: ");
        if ( fgets(line, sizeof(line), stdin) )
            sscanf(line, "%[^\n]s", send_buff);
        write(sockfd, send_buff, sizeof(send_buff));

        //Get the reply of server and check if the game ends
        read(sockfd, rcv_buff, sizeof(rcv_buff));
        print_result(cnt, send_buff, rcv_buff);
        if (rcv_buff[0] == '4')
            end_of_game = 1;
        
        cnt++;
    }

    printf("You win!!\n\n");

    //.....................................................

    //Terminate the transfer
    close(sockfd);

}


void raise_error(const char* msg)
{
    perror(msg);
    exit(1);
}

void print_result(int cnt, const char* guess, const char* result)
{
    printf("\n");
    printf("++++++++++++++++++++++++++\n");
    printf("+ Round + Guess + Result +\n");
    printf("++++++++++++++++++++++++++\n");
    printf("+   %d   + %s  +  %s  +\n", cnt, guess, result);
    printf("++++++++++++++++++++++++++\n");
    printf("\n");
}
