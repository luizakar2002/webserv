#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <iostream>
# include <netinet/in.h>
// # include <sys/socket.h>
# include <sys/event.h>
// # include <fcntl.h> //CONSIDER
# include <unistd.h>
# include "AFd.hpp"
# include "../Utils/Utils.hpp"

//*** means it's merged from Leo's Server class

class Server: public AFd
{
    private:
        ListeningSocket         *socket;
        int                     _server_fd;
        std::string             _serverName; //***
        std::string             _ip; // not needed, may be removed cause port is in server_params
        int                     _port;
        std::vector<Config>     configs; // vector of configs with same ip_port, no need to keep _locationMap
        struct  sockaddr_in     client_addr; //?
        int                     client_addr_len;
    public:
        Server();
        Server(server_params &, const std::string &, std::vector<Config> &);
        
        virtual ~Server();

        Server  &operator=(const Server &);

        ListeningSocket *get_socket() const;

        //***
        const std::string 				&getServerName() const;
        const int                       &get_server_fd() const;
        const std::string 				&getIp() const;
        const int        				&getPort() const;
        std::vector<Config>             &getConfig();
        void							setServerName(std::string);
        void							setIp(std::string);
        void							setPort(int);
        
        
        Location	&getLastLocation();
        Location	&getPerfectLocation(std::string &);
};

#endif