# include "Server.hpp"

ns::Server::Server(int &domain, int &type, int &protocol, int &port, u_long &interface, int &b_log): SimpleServer(domain, type, protocol, port, interface, b_log)
{
    client_addr_len = sizeof(client_addr);
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    launch_server();
}

ns::Server::~Server()
{

}

void    ns::Server::accepter(int &event_fd)
{
    std::cout << "accept start" << std::endl;

    accept_fd = accept(event_fd, (struct sockaddr *)&client_addr, (socklen_t *)&this->client_addr_len);
    get_socket()->test_connection(accept_fd);
    fcntl(this->accept_fd, F_SETFL, O_NONBLOCK); //to not block the whole program for waiting until smth is written on this fd
    EV_SET(this->change_event, this->accept_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(this->kq, this->change_event, 1, NULL, 0, NULL) == -1)
    {
        std::cout << "error in kevent()" << std::endl;
        exit(2);
    }
    std::cout << "accept end" << std::endl;
}

void    ns::Server::handler(int &event_fd)
{
    char buf[1024];
    size_t bytes_read = recv(event_fd, buf, sizeof(buf), 0);

    if (bytes_read < 0)
    {
        std::cout << "error in recv()" << std::endl;
        exit(3);
    }
    Request request = Request(std::string(buf));
    printf("%s\n", buf);
    responder(event_fd); //, request);
}

void    ns::Server::responder(int &event_fd)
{
    Response response = Response();
    send(event_fd, response.get_c_response(), strlen(response.get_c_response()), 0);
}

void    ns::Server::launch_server()
{
    int new_events;
    int event_fd;
    int i;

    this->kq = kqueue();
    EV_SET(this->change_event, get_socket()->get_socket_fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    if (kevent(this->kq, this->change_event, 1, NULL, 0, NULL) == -1)
    {
        std::cout << "error in kevent()" << std::endl;
        exit(2);
    }

    while (1)
    {
        //checking an evvent per iteration, not registering, so 2 and 3 args are 0
        new_events = kevent(this->kq, NULL, 0, event, 1, NULL);
        if (new_events == -1)
        {
            continue;  //server should not stop in any case
            // std::cout << "error in kevent()" << std::endl;
            // exit(2);
        }
        i = -1;
        while (++i < new_events)
        {
            event_fd = event[i].ident;

            if (event[i].flags & EV_EOF)
            {
                std::cout << "client disconnected" << std::endl;
                close(event_fd); //other actions need to be provided as this means closing a connection
            }
            else if (event_fd == this->get_socket()->get_socket_fd())
            {
                accepter(event_fd);
            }
            else if (event[i].filter & EVFILT_READ)
            {
                handler(event_fd);
            }
        }
    }
}