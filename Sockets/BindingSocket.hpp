#ifndef BINDING_SOCKET_HPP
# define BINDING_SOCKET_HPP

# include "SimpleSocket.hpp"

class BindingSocket: public SimpleSocket
{
    private:
        int binding;
    public:
        BindingSocket(int &, int &, int &, int &, u_long &);
        virtual ~BindingSocket();

        void    connect_to_network(int &, struct sockaddr_in &);
};

#endif