# include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int &domain, int &type, int &protocol, int &port, u_long &interface, int &b_log): BindingSocket(domain, type, protocol, port, interface)
{
    backlog = b_log;
    start_listening();
    test_connection(listening);
}

ListeningSocket::~ListeningSocket()
{

}

void    ListeningSocket::start_listening()
{
    listening = listen(get_socket_fd(), backlog);
}

void    ListeningSocket::set_backlog(const int &b_log)
{
    backlog = b_log;
}

const int &ListeningSocket::get_backlog() const
{
    return backlog;
}
