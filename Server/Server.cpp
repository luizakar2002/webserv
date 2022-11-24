# include "Server.hpp"

Server::Server()
{
    _fd_type = FD_SERVER;
    _port = -1;
}

Server::Server(server_params &s_params, const std::string &ip_port_str, std::vector<Config> &configs_vector)
{
    socket = new ListeningSocket(s_params.domain, s_params.type, s_params.protocol, s_params.port, s_params.interface, s_params.b_log);
    _fd_type = FD_SERVER;
    _server_fd = socket->get_socket_fd();
	setFd(_server_fd); // to set _fd of AFd, not sure if _server_fd is needed anymore
	_ip_port = ip_port_str;
	_ip = ip_port_str.substr(0, ip_port_str.find_first_of(':'));
    _port = s_params.port;
	std::cout << "PORT ->>>" << _port << std::endl;
    // be careful here, no assignement operator overload in Config.hpp
    configs = configs_vector;
    client_addr_len = sizeof(client_addr);
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    // launch_server();
}

Server::~Server()
{
}

Server  &Server::operator=(const Server &copy)
{
	_fd = copy._fd;
	_fd_type = copy._fd_type;
	_serverName = copy.getServerName();
	_ip = copy.getIp();
	_port = copy.getPort();
	if (!this->configs.empty()){
		this->configs.clear();
	}
	this->configs.insert(configs.begin(), copy.configs.begin(), copy.configs.end());
	return *this;
}

ListeningSocket *Server::get_socket() const
{
    return socket;
}

const	int		&Server::get_server_fd() const
{
	return _server_fd;
}

void	Server::setServerName(std::string serverName)
{
	_serverName = serverName;
}

void	Server::setIp(std::string ip)
{
	_ip = ip;
}

void	Server::setPort(int port)
{
	_port = port;
}

const std::string	&Server::getServerName() const
{
	return _serverName;
}

const std::string	&Server::getIp() const
{
	return _ip;
}

const std::string	&Server::getIpPort() const
{
	return _ip_port;
}

const int	&Server::getPort() const
{
	return _port;
}

std::vector<Config>     &Server::getConfig()
{
	return configs;
}