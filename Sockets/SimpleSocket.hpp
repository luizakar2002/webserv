#ifndef SIMPLE_SOCKET_HPP
# define SIMPLE_SOCKET_HPP

# include <iostream>
# include <sys/event.h>
# include <netinet/in.h>

class SimpleSocket
{
    private:
        struct sockaddr_in   address;
        int                  sock_fd; //server_fd in server
    public:
        SimpleSocket(int &, int &, int &, int &, u_long &);
        virtual ~SimpleSocket();

        virtual void    connect_to_network(int &, struct sockaddr_in &) = 0; //for user to define if she wants to bind() or connect()
        void        init_addr(int &, int &, int &, u_long &);
        void        test_connection(int &) const;
        //getters
    public:
        const struct sockaddr_in &get_address() const;
        const int   &get_socket_fd() const;
};

#endif
