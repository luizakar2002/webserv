// #ifndef SERVER_HPP
// # define SERVER_HPP

// # include <iostream>
// # include <netinet/in.h>
// // # include <sys/socket.h>
// # include <sys/event.h>
// # include <fcntl.h>
// # include <unistd.h>
// # include "Request.hpp"
// # include "Response.hpp"

// class Server
// {
//     private:
//         // int backlog; may be added
//         int                     port;
//         int                     server_fd;
//         int                     accept_fd;
//         struct  sockaddr_in     server_addr;
//         struct  sockaddr_in     client_addr;
//         int                     client_addr_len;
//         int                     kq;
//         struct  kevent          change_event[33];   //backlog + 1
//         struct  kevent          event[33];
//         // Request                 request;
//         // Response                response;
//     public:
//         Server();
//         virtual ~Server();

//         void    socket_for_listening(); //ListeningSocket
//         void    event_loop(); //serveer_launch
//         void    accept_new_connection(int &); //accepter
//         void    read_from_socket(int &); //handler
//         void    send_response(int &); //, Request &);  //responder
// };

// #endif