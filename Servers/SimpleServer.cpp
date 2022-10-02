# include "SimpleServer.hpp"

ns::SimpleServer::SimpleServer(int &domain, int &type, int &protocol, int &port, u_long &interface, int &b_log)
{
    socket = new ListeningSocket(domain, type, protocol, port, interface, b_log);
}

ns::SimpleServer::~SimpleServer()
{

}

ns::ListeningSocket *ns::SimpleServer::get_socket() const
{
    return socket;
}