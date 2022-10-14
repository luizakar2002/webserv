# include "Server.hpp"

ns::Server::Server(server_params &s_params, const std::string &ip_port_str, std::vector<Config> &configs_vector): SimpleServer(s_params.domain, s_params.type, s_params.protocol, s_params.port, s_params.interface, s_params.b_log)
{
    // be careful here, no assignement operator overload in Config.hpp
    ip_port = ip_port_str;
    configs = configs_vector;
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
    Request *request = new Request(std::string(buf));
    if (configs.size() > 1)
    {

        responder(event_fd, request, configs);
    }
    responder(event_fd, request);
}

void    ns::Server::responder(int &event_fd, Request *request)
{
    Response response = Response(request);
    std::cout << response.get_c_response() << "\n\n";
    send(event_fd, response.get_c_response(), strlen(response.get_c_response()), 0);
}

void    ns::Server::responder(int &event_fd, Request *request, std::vector<Config> &configs)
{
    std::cout << "\n\nresponder overload\n\n";
    // iterate over configs to find matching server_name with request.get_headers()["Host:"], execute server with that server_params with that conf, if not found matching config, handle response with this default server
    // std::vector<Config>::iterator conf = configs.begin();
    // while (conf != configs.end())
    // {

    // }
    Response response = Response(request);
    std::cout << response.get_c_response() << "\n\n";
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