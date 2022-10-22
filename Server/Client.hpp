#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
// # include "Response.hpp" // maybe we'll need this

// this's trial for now, maybe we won't need it at all
// # include "Headers.hpp"
# include "AFd.hpp"
// # include "Server.hpp"

class Server;
// class AFd;

typedef enum
{
	REQUEST_RECEIVING,
	MAKE_RESPONSE,
	FILE_READ,
	FILE_READ_DONE,
	FILE_WRITE,
	FILE_WRITE_DONE,
	MAKE_RESPONSE_DONE
}	e_client_status;

class Client : public AFd
{
	private:
		e_client_status	_status;
		Server 			*_server;
		unsigned long	_lastTime;
		// Request			_request;
	public:
		Client();
		Client(Server *server, int c_fd);
		Client(const Client &);

		virtual ~Client();

		Client	&operator=(const Client &);

		unsigned long 	getLastTime() const;
		e_client_status	&getClientStatus();
		// Request			&getRequest();
		void 			setLastTime(unsigned long);
		void			setClientStatus(e_client_status	status);

		// void	appendOrigin(string str);
		// bool	parseRequest(); // harcakan a argument kvercni te voch;

};
# include "Server.hpp"
// # include "Headers.hpp"

#endif