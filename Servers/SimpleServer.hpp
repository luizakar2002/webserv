#ifndef SIMPLE_SERVER_HPP
# define SIMPLE_SERVER_HPP

// # include "../Webserv.hpp"
# include "../Sockets/Sockets.hpp"
# include "../Request.hpp"

namespace ns
{
    class SimpleServer
    {
        private:
            ListeningSocket *socket;
            virtual void accepter(int &) = 0;
            virtual void handler(int &) = 0;
            virtual void responder(int &, Request *) = 0;
        public:
            SimpleServer(int &, int &, int &, int &, u_long &, int &);
            virtual ~SimpleServer();

            virtual void launch_server() = 0; //while loop

            ListeningSocket *get_socket() const;
    };
}

#endif