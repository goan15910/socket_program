// Program to set up TCP client process to upload file

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
    char line[LINE_LEN]; //Declare buffers for user input
    char fname[BUFF_SIZE]; //file name
    char fpath[BUFF_SIZE]; //file path 
    char buff[BUFF_SIZE]; //buffer for socket IO

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

    //.............................................................................

    //Prompt user to enter the filename
    printf("Enter the filename: ");
    if ( fgets(line, sizeof(line), stdin) )
        sscanf(line, "%[^\n]s", fname);
    int write_flag = write(sockfd, fname, sizeof(fname));
    if (write_flag < 0)
        raise_error("Error writing to socket");

    //Prompt user to enter the path of file
    printf("Enter the path of the file: \n");
    if ( fgets(line, sizeof(line), stdin) )
        sscanf(line, "%[^\n]s", fpath);

    //Open the file as a string
    char* f_buff = 0;
    long len;
    FILE* f = fopen(fpath, "rb");

    if (f){
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        fseek(f, 0, SEEK_SET);
        f_buff = malloc(len);
        if (f_buff){
            fread(f_buff, 1, len, f);
        }
        fclose(f);
    }

    //Transmit the string form of the file to server
    write_flag = write(sockfd, f_buff, sizeof(f_buff));
    if (write_flag < 0)
        raise_error("Error writing to socket");

    //Wait for server's reply
    int read_flag = read(sockfd, buff, sizeof(buff));
    if (read_flag < 0)
        raise_error("Error reading from socket");
    printf("The message from server: %s\n", buff);

    //.............................................................................

    //Terminate the transfer
    close(sockfd);

}
