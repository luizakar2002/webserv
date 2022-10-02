#ifndef SERVER_HPP
# define SERVER_HPP

# include "SimpleServer.hpp"
# include <netinet/in.h>
# include <iostream>
# include <netinet/in.h>
// # include <sys/socket.h>
# include <sys/event.h>
# include <fcntl.h>
# include <unistd.h>
# include "../Request.hpp"
# include "../Response.hpp"


namespace ns
{
    class Server: SimpleServer
    {
        private:
            struct  sockaddr_in     client_addr; //?
            int                     client_addr_len;
            int                     accept_fd;
            int                     kq;
            struct  kevent          change_event[33];   //backlog + 1
            struct  kevent          event[33];
            void                    accepter(int &);
            void                    handler(int &);
            void                    responder(int &);
        public:
            Server(int &, int &, int &, int &, u_long &, int &);
            virtual ~Server();

            void    launch_server();
    };
}

#endif