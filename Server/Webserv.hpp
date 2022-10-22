#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
# include "../Config/Configs.hpp" //-
# include "../Request.hpp"
# include "../Response.hpp" //-
#include "Client.hpp"
#include "Server.hpp"
#include "AFd.hpp"

# define MAX_FD_SIZE 1024
# define BUFSIZE 1025

class Server;
class Client;

typedef std::map< std::string, std::vector<Config> > unique_configs; //map: key - ip:host, value - vector of configs with same ip:host

class Webserv 
{
	private:
		int							_kq;
		struct kevent 				_return_events[1024];
		std::vector<struct kevent>	_change_list;
		std::vector<AFd *>			_fd_pool;
		unsigned long 				_timeout;
		std::vector<Config>			configs;
		unique_configs				unique_config_vectors; //map of ip::port and config

	public:
		Webserv(std::vector<Config> &);
		Webserv(const Webserv &other);

		virtual	~Webserv();
		
		Webserv	&operator=(const Webserv &other);

		void	seperate_configs();
		int		accept_client(struct kevent *);
		int		request_handler(struct kevent *);
		void	response_maker(int , Request *);
		void	response_maker(int, Request *, std::vector<Config> &);
		void	start_server();
		void	run_server();
		void	change_events(std::vector<struct kevent> &change_list, uintptr_t ident, int16_t filter,
					uint16_t flags, uint32_t fflags, intptr_t data, void *udata);

		std::vector<struct kevent>& getChangeList();
		void addFdPool(AFd* res);
		void deleteFdPool(AFd *res);
		void signalExit();
		unsigned long getTime();

};

# include "Client.hpp"

#endif