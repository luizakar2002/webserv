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
# include "../Config/Configs.hpp"
# include "../Utils/Utils.hpp"
// # include "../Webserv.hpp"


namespace ns
{
    class Server: SimpleServer
    {
        private:
            std::string             ip_port; // not needed, may be removed cause port is in server_params
            std::vector<Config>     configs; // vector of configs with same ip_port
            struct  sockaddr_in     client_addr; //?
            int                     client_addr_len;
            int                     accept_fd;
            int                     kq;
            struct  kevent          change_event[33];   //backlog + 1
            struct  kevent          event[33];
            void                    accepter(int &);
            void                    handler(int &);
            void                    responder(int &, Request *);
            void                    responder(int &, Request *, std::vector<Config> &); //in case of not unique configs
        public:
            Server(server_params &, const std::string &, std::vector<Config> &);
            virtual ~Server();

            void    launch_server();
    };
}

#endif