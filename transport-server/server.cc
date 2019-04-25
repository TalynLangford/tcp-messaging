#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

#define PORT 5432
#define BACKLOG 10
#define MAXLINES 1024
#define EXIT_MSG "exit"

int main(int argc, char** argv)
{
    int fd, new_s, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buf[MAXLINES] = {0};
    char const *msg = "Hello from the server"; // fails otherwise?

    int ticket;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("socket failed");
        exit(1);
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind error");
        exit(1);
    }

    if (listen(fd, BACKLOG) < 0)
    {
        perror("listen");
        exit(1);
    }

    std::cout << "listening..." << std::endl;

    if ((new_s = accept(fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(1);
    }

    std::cout << "accepted..." << new_s << std::endl;

    while(1)
    {
        // truncates long messages. Use recv in while to capture.
        valread = read(new_s, buf, MAXLINES);

        std::cout << "read : " << valread << std::endl;
        if (valread < 0)
        {
            perror("valread");
        }
        printf("%s\n",buf);
        send(new_s, msg, strlen(msg), 0);
        printf("Hello message sent\n");

        if (!strncmp(buf, EXIT_MSG, (size_t)(sizeof(EXIT_MSG) - 1)))
           break;

        bzero(buf, MAXLINES);
    }
    return 0;
}