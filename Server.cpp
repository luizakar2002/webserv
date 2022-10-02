// #include "Server.hpp"

// //need to throw exceptions instead of exit()-ing

// Server::Server()
// {
//     port = 8080; //should be taken from config
//     client_addr_len = sizeof(client_addr);
//     memset((char *)&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(port);
//     server_addr.sin_addr.s_addr = INADDR_ANY;
//     socket_for_listening();
//     event_loop();
// }

// Server::~Server()
// {
// }

// void    Server::socket_for_listening()
// {
//     this->server_fd = socket(this->server_addr.sin_family, SOCK_STREAM, 0);
//     if (this->server_fd < 0)
//     {
//         perror("cannot create a socket");
//         exit(1);
//     }
//     if (bind(this->server_fd, (struct sockaddr *)&this->server_addr, sizeof(server_addr)) < 0)
//     {
//         perror("cannot bind");
//         exit(2);
//     }
//     if (listen(this->server_fd, 10))
//     {
//         perror("cannot listen");
//         exit(3);
//     }
// }

// void    Server::accept_new_connection(int &event_fd)
// {
//     std::cout << "accept start" << std::endl;

//     this->accept_fd = accept(event_fd, (struct sockaddr *)&client_addr, (socklen_t *)&this->client_addr_len);
//     if (this->accept_fd < -1)
//     {
//         std::cout << "error in accept" << std::endl;
//         exit(3);
//     }
//     fcntl(this->accept_fd, F_SETFL, O_NONBLOCK); //to not block the whole program for waiting until smth is written on this fd
//     EV_SET(this->change_event, this->accept_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
//     if (kevent(this->kq, this->change_event, 1, NULL, 0, NULL) == -1)
//     {
//         std::cout << "error in kevent()" << std::endl;
//         exit(2);
//     }
//     std::cout << "accept end" << std::endl;

// }

// void    Server::read_from_socket(int &event_fd)
// {
//     char buf[1024];
//     size_t bytes_read = recv(event_fd, buf, sizeof(buf), 0);

//     if (bytes_read < 0)
//     {
//         std::cout << "error in recv()" << std::endl;
//         exit(3);
//     }
//     Request request = Request(std::string(buf));
//     printf("%s\n", buf);
//     send_response(event_fd); //, request);
// }

// void    Server::send_response(int &event_fd) //, Request &request)
// {
//     // Response response = Response(request); //for later
//     Response response = Response();
//     send(event_fd, response.get_c_response(), strlen(response.get_c_response()), 0);
// }

// void    Server::event_loop()
// {
//     int new_events;
//     int i;
//     int event_fd;

//     this->kq = kqueue();
//     EV_SET(this->change_event, this->server_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
//     if (kevent(this->kq, this->change_event, 1, NULL, 0, NULL) == -1)
//     {
//         std::cout << "error in kevent()" << std::endl;
//         exit(2);
//     }

//     while (1)
//     {
//         //checking an evvent per iteration, not registering, so 2 and 3 args are 0
//         new_events = kevent(this->kq, NULL, 0, event, 1, NULL);
//         if (new_events == -1)
//         {
//             continue;  //server should not stop in any case
//             // std::cout << "error in kevent()" << std::endl;
//             // exit(2);
//         }
//         i = -1;
//         while (++i < new_events)
//         {
//             event_fd = event[i].ident;

//             if (event[i].flags & EV_EOF)
//             {
//                 std::cout << "client disconnected" << std::endl;
//                 close(event_fd); //other actions need to be provided as this means closing a connection
//             }
//             else if (event_fd == this->server_fd)
//             {
//                 accept_new_connection(event_fd);
//             }
//             else if (event[i].filter & EVFILT_READ)
//             {
//                 read_from_socket(event_fd);
//             }
//         }
//     }
// }