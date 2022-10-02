# include "ListeningSocket.hpp"

ns::ListeningSocket::ListeningSocket(int &domain, int &type, int &protocol, int &port, u_long &interface, int &b_log): BindingSocket(domain, type, protocol, port, interface)
{
    backlog = b_log;
    start_listening();
    test_connection(listening);
}

ns::ListeningSocket::~ListeningSocket()
{

}

void    ns::ListeningSocket::start_listening()
{
    listening = listen(get_socket_fd(), backlog);
}

void    ns::ListeningSocket::set_backlog(const int &b_log)
{
    backlog = b_log;
}

const int &ns::ListeningSocket::get_backlog() const
{
    return backlog;
}
