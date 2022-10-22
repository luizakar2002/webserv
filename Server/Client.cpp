#include "Client.hpp"

Client::Client()
{
	_status = REQUEST_RECEIVING;
}

Client::Client(Server *server, int c_fd)
{
	this->_server = server;
	this->_status = REQUEST_RECEIVING;
	this->_fd_type = FD_CLIENT;
	this->_fd = c_fd;
}

Client::~Client()
{
}

Client::Client(const Client &copy)
{
	_status = copy._status;
}

Client	&Client::operator=(const Client &rhs)
{
	_status = rhs._status;
	return *this;
}

unsigned long	Client::getLastTime(void) const
{
	return _lastTime;
}

void	Client::setLastTime(unsigned long time)
{
	_lastTime = time;
}

e_client_status		&Client::getClientStatus()
{
	return _status;
}

void	Client::setClientStatus(e_client_status	status)
{
	_status = status;
}

// void	Client::appendOrigin(string str)
// {
// 	this->getRequest().getOrigin() += str;
// }

// bool	Client::parseRequest()
// {
// 	// 
// 	// parsing request... 
// }