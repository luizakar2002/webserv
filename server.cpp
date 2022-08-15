#include <iostream>
#include <sys/socket.h>
#include <sys/event.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

const int PORT = 8080;

int socket_listen()
{
    int server_fd;
    struct sockaddr_in addr;
    int len;
    len = sizeof(addr);
    memset((char *)&addr, 0, len);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    server_fd = socket(addr.sin_family, SOCK_STREAM, 0);
    // fcntl(server_fd, F_SETFL, O_NONBLOCK);
    if (server_fd < 0)
    {
        perror("cannot create a socket");
        exit(1);
    }
    if (bind(server_fd, (struct sockaddr *)&addr, len) < 0)
    {
        perror("cannot bind");
        exit(2);
    }
    if (listen(server_fd, 10))
    {
        perror("cannot listen");
        exit(3);
    }
    return server_fd;
}

int main()
{
    int server_fd, new_fd;
    long count;

    char *msg = (char *)"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 56\n\n<html>\n<body>\n<h1>Hello from server</h1>\n</body>\n</html>";
    server_fd = socket_listen();
    int kq = kqueue();
    struct kevent ev_set;
    EV_SET(&ev_set, server_fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
    kevent(kq, &ev_set, 1, NULL, 0, NULL);
    while (1)
    {
        new_fd = accept(server_fd, (struct sockaddr *)&addr, (socklen_t *)&len);
        if (new_fd < 0)
        {
            perror("error in accept");
            exit(4);
        }
        char buffer[30000] = {0};
        count = recv(new_fd, buffer, 30000, 0);
        if (count == 0)
        {
            std::cout << "connection is closed" << std::endl;
            break ;
        }
        else if (count < 0)
            std::cout << "error in reading" << std::endl;
        printf("%s\n", buffer);
        send(new_fd, msg, strlen(msg), 0);
        close(new_fd);
    }
    return 0;
}