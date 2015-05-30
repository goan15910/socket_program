// Program to set up TCP server process for uploading file

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


void raise_error(const char* msg);
void insert_str(char strA[], char strB[], char strC[], int pos);


int main()
{
    char line[LINE_LEN]; //line buffer for user input
    char buff[BUFF_SIZE]; //buffer to receive content from client

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

    //.......................................................

    //Wait for messages from client to get the filename
    int read_flag = read(client_fd, buff, sizeof(buff));
    if (read_flag < 0)
        raise_error("Error reading from socket");
    char fname[BUFF_SIZE];
    char* append_string = "-b99501038";
    int insert_pos = (int)(strchr(buff, '.') - buff);
    insert_str(buff, append_string, fname, insert_pos);
    printf("Original filename: %s\n", buff);
    printf("New filename: %s\n", fname);

    //Wait for the file transmit by client
    char* f_buff;
    read_flag = read(client_fd, f_buff, sizeof(f_buff));
    if (read_flag < 0)
        raise_error("Error reading from socket");

    //Store the content in f_buff into a file
    FILE* f = fopen(fname, "ab");
    if (f != NULL){
        fputs(f_buff, f);
        fclose(f);
    }

    //.......................................................

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

void insert_str(char strA[], char strB[], char strC[], int pos)
{
    strncpy(strC, strA, pos);
    strC[pos] = '\0';
    strcat(strC, strB);
    strcat(strC, strA + pos);
}
