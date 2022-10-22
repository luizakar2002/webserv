# include "BindingSocket.hpp"

BindingSocket::BindingSocket(int &domain, int &type, int &protocol, int &port, u_long &interface): SimpleSocket(domain, type, protocol, port, interface)
{
    int sock_fd = get_socket_fd();
    struct sockaddr_in address = get_address();
    connect_to_network(sock_fd, address);
    test_connection(binding);
}

BindingSocket::~BindingSocket()
{
    
}

void    BindingSocket::connect_to_network(int &sock_fd, struct sockaddr_in &address)
{
    binding = bind(sock_fd, (struct sockaddr *)&address, sizeof(address));
}