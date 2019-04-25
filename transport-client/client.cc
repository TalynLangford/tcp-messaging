#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h> // for inet_pton
#include <unistd.h> // for read
#include <iostream>

#define PORT 5432
#define MAXLINES 1024
#define EXIT_MSG "exit"

int main(int arc, char** argv)
{
    struct sockaddr_in address;
    int s = 0, valread;
    struct sockaddr_in serv_addr;
    char const* msg = "Hello from client";
    char buf[MAXLINES] = {0};

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nsocket creation error");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/Address not supported\n");
        return -1;
    }

    if (connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    /*
    if (connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection failed\n");
        return -1;
    }
    */
    bool e_flag = false;
    while(!e_flag)
    {
        fgets(buf, MAXLINES, stdin);

        if (!strncmp(buf, EXIT_MSG, (size_t)(sizeof(EXIT_MSG) - 1)))
            e_flag = !e_flag;

        send(s, buf, strlen(buf), 0);
        // zero buffer for response
        bzero(buf, MAXLINES);
        printf("Hello message sent\n");
        valread = read(s, buf, MAXLINES);
        printf("%s\n", buf);   
    }
    return 0;
}