# include "SimpleSocket.hpp"

SimpleSocket::SimpleSocket(int &domain, int &type, int &protocol, int &port, u_long &interface)
{
    int opt = 1;
    init_addr(domain, type, port, interface);
    sock_fd = socket(domain, type, protocol);
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    test_connection(sock_fd);
}

SimpleSocket::~SimpleSocket()
{
}

void    SimpleSocket::init_addr(int &domain, int &type, int &port, u_long &interface)
{
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface); // my IP address
}

void    SimpleSocket::test_connection(int &sock_fd) const //consider this later as the server should not stop at any case
{
    if (sock_fd < 0)
    {
        std::cout << "Error\n";
    }
    //else //consider this case
}

const struct sockaddr_in &SimpleSocket::get_address() const
{
    return address;
}

const int &SimpleSocket::get_socket_fd() const
{
    return sock_fd;
}
