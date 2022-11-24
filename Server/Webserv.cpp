# include "Webserv.hpp"

Webserv::Webserv(std::vector<Config> &ref_configs)
{
	_fd_pool.resize(MAX_FD_SIZE, NULL);
	_timeout = 700000;
	configs = ref_configs;
	std::cout << "WEBSERV constructor\n";
	start_server();
}

Webserv::Webserv(const Webserv &other)
{
	*this = other;
}

Webserv::~Webserv()
{
}

Webserv	&Webserv::operator=(const Webserv &rhs)
{
	_kq = rhs._kq;
	_fd_pool = rhs._fd_pool;
	for (int i = 0; i < MAX_FD_SIZE; i++)
		_return_events[i] = rhs._return_events[i];
	return (*this);
}

static void	ft_error(const char *sms){
	std::cerr << sms << std::endl;
	exit(1);
}

void	Webserv::change_events(std::vector<struct kevent> &change_list, uintptr_t ident, int16_t filter,
	uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
		struct kevent	temp_event;

		EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
		change_list.push_back(temp_event);
}

void    Webserv::seperate_configs()
{
	std::cout << "in seperate configs" << configs.size() << std::endl;
    std::vector<Config>::iterator conf = configs.begin(); //iterator for while loop
    // unique_configs unique_config_vectors;  //result map with keys as ip:host and value as vector of not unique server configs
    unique_configs::iterator listen_dir; //to find where to store current conf
    // prop_map conf_prop_map;  //for storing return value of conf->get_map() to find value of "listen" key
    prop_map::iterator prop_map_listen_dir;
    while (conf != configs.end())
    {
        prop_map	conf_prop_map(conf->get_map());
        prop_map_listen_dir = conf_prop_map.find("listen");

        if (prop_map_listen_dir != conf_prop_map.end()) // if there is a listen directive in prop_map of config
        {
            listen_dir = unique_config_vectors.find(prop_map_listen_dir->second[0]);
            if (listen_dir != unique_config_vectors.end())
            {
                listen_dir->second.push_back(*conf);
            }
            else
            {
                std::vector<Config> unique_config_vectors_value;
                unique_config_vectors_value.push_back(*conf);
                std::pair< std::string, std::vector<Config> > p = std::make_pair(prop_map_listen_dir->second[0], unique_config_vectors_value);
                unique_config_vectors.insert(p);
            }
        }
        conf++;
    }
	std::cout << "IN SEPERATE CONFIGS correctly seperated\n";
	std::cout << unique_config_vectors.begin()->first << std::endl;
	std::cout << unique_config_vectors.rbegin()->first<< std::endl;
	std::cout << "end seperate configs"<< std::endl;

}

void	Webserv::start_server()
{
	seperate_configs();

	if ((_kq = kqueue()) == -1)
		ft_error("kqueue error");

    unique_configs::iterator it = unique_config_vectors.begin();
    while (it != unique_config_vectors.end())
    {
        server_params s_params = server_params(it->first);
        Server *s = new Server(s_params, it->first, it->second);
		std::cout << s->get_server_fd() << std::endl;
        change_events(_change_list, s->get_server_fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
        _fd_pool[s->get_server_fd()] = s;
		_fd_ip_port[s->get_server_fd()] = s->getIpPort();
        it++;
    }
	run_server();
}

int		Webserv::accept_client(struct kevent *curr_event)
{
	std::cout << "in accept_client " << curr_event->ident << std::endl;
	std::cout << "in accept_client pointer" << _fd_pool[curr_event->ident] << std::endl;
	int serv_fd = _fd_pool[curr_event->ident]->getFd();
	std::cout << "in accept_client getFd" << serv_fd << std::endl;
	// serv_fd = 4;
	int client_sock;

	if ((client_sock = accept(serv_fd, NULL, NULL)) == -1){
		std::cerr << "error: accept" << std::endl;
		return -1;
	}
	fcntl(client_sock, F_SETFL, O_NONBLOCK);
	struct timeval	tv;
	tv.tv_sec = 60;
	tv.tv_usec = 0;
	if (setsockopt(client_sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(struct timeval)) < 0)
	{
		std::cerr << "error: setsockopt timeout" << std::endl;
		return -1;
	}
	change_events(_change_list, client_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	change_events(_change_list, client_sock, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);

	Client	*client = new Client((dynamic_cast<Server *>(_fd_pool[serv_fd])), client_sock);
	_fd_pool[client_sock] = client;
	std::cout << "client_sock fd in accept_client: " << client_sock << std::endl;
	_fd_ip_port[client_sock] = _fd_ip_port[serv_fd]; //to keep track of ip_port of clients too, as server's is done, to know which port the request is sent in response_maker 
	return 0;
}

int		Webserv::request_handler(struct kevent *curr_event)
{
	std::cout << "REQUEST_HANDLER" << std::endl;
	Client *client = dynamic_cast<Client *>(_fd_pool[curr_event->ident]);
	client->setLastTime(getTime());
	char	buf[BUFSIZE];
	int 	n;
	std::string		current_ip_port;

	memset(buf, 0, BUFSIZE);
	if ((n = recv(curr_event->ident, buf, BUFSIZE - 1, 0)) == -1)
	{
		deleteFdPool(_fd_pool[curr_event->ident]);
		return -1;
	}
	if (n == 0)
		deleteFdPool(_fd_pool[curr_event->ident]);
	else if (n > 0)
	{
		buf[n] = '\0';
		// client->appendOrigin(buf);
		// if (client->getClientStatus() == REQUEST_RECEIVING && client->parseRequest())
		// {
		// 	client->setClientStatus(MAKE_RESPONSE);
		// 	// I have to make response here;
		// }
		Request *request = new Request(std::string(buf));
		if (configs.size() > 1)
		{
			std::cout << curr_event->ident << std::endl;
			current_ip_port = _fd_ip_port[curr_event->ident]; //added 14
			// current_ip_port = _fd_pool[curr_event->ident];
			std::cout << "ip_port of request: " << current_ip_port << std::endl;
			std::cout << unique_config_vectors[current_ip_port].at(0).get_map().begin()->first; //just to check that unique_config_vectors[current_ip_port] is accessable
			response_maker(_fd_pool[curr_event->ident]->getFd(), request, unique_config_vectors[current_ip_port]);
		}
		response_maker(_fd_pool[curr_event->ident]->getFd(), request);
	}
	return 0;
}

void	Webserv::response_maker(int event_fd, Request *request)
{
	Response response = Response(request, *(configs.begin())); // default server
    std::cout << response.get_c_response() << "\n\n";
    send(event_fd, response.get_c_response(), strlen(response.get_c_response()), 0);
	free(request);
}

void    Webserv::response_maker(int event_fd, Request *request, std::vector<Config> &configs)
{
	Response response = Response(request, *(configs.begin())); //default server;
    std::vector<Config>::iterator conf = configs.begin();

	//CHECKING if seperating config vectors for each requests works
	std::cout << "CHECKING\n";
	prop_map map = configs.begin()->get_map();
    const std::string prop = "listen";
    std::vector<std::string>::iterator vect = map[prop].begin();
	std::cout << request->get_raw_request() << "_____" << *vect << std::endl;

	std::cout << "config size in response_maker overload: " << configs.size();
	//end of CHECKING

    while (conf != configs.end())
    {
		std::string host;
		std::vector<std::string> server_names;

		std::vector<std::string>::iterator matching_server_name;
		std::map<std::string, std::string>::const_iterator iterator_on_host_header = request->get_headers().find("Host:"); //e.g. localhost:8080
		prop_map::const_iterator iterator_on_server_name_of_config = conf->get_map().find("server_name");

		// server_names = iterator_on_server_name_of_config->second;
		if (iterator_on_host_header != request->get_headers().end())
			host = iterator_on_host_header->second;
		if (iterator_on_server_name_of_config != conf->get_map().end())
		{
			server_names = iterator_on_server_name_of_config->second;
			matching_server_name = std::find(server_names.begin(), server_names.end(), host);
		}
		if (matching_server_name != server_names.end())
			break ;
		++conf;
    }
	if (conf != configs.end())
	{
		std::cout << "IN IF" << std::endl;
		response = Response(request, *conf);
		prop_map::const_iterator iterator_on_server_name_of_config = conf->get_map().find("index");
		std::cout << "INDEX OF CONF: " << *(iterator_on_server_name_of_config->second).begin();
	}
    std::cout << response.get_c_response() << "\n\n";
    send(event_fd, response.get_c_response(), strlen(response.get_c_response()), 0);
}

void	Webserv::run_server()
{
	int				new_events;
	struct kevent	*curr_event;

	for(;;)
	{
		new_events = kevent(_kq, &_change_list[0], _change_list.size(), _return_events, 1024, NULL);
		if (new_events == -1)
			ft_error("kevent error");
		
		_change_list.clear();
		for (int i = 0; i < new_events; i++)
		{
			curr_event = &_return_events[i];
			if (curr_event->flags & EV_ERROR)
			{
				std::cerr << "error: socket deleted" << std::endl;
				deleteFdPool(_fd_pool[curr_event->ident]);
			}
			else if (curr_event->filter == EVFILT_READ)
			{
				if (_fd_pool[curr_event->ident]->getFdType() == FD_SERVER)
				{
					if (accept_client(curr_event))
						continue ;
				}
				else if (_fd_pool[curr_event->ident]->getFdType() == FD_CLIENT)
				{
					if (request_handler(curr_event))
						continue ;
				}
				else if (_fd_pool[curr_event->ident]->getFdType() == FD_RESOURCE)
				{
					// es karoxa petq chga, bayc miguce CGI-i jamanak ogni...
				}		
			}
			else if (curr_event->filter == EVFILT_WRITE)
			{
				if (_fd_pool[curr_event->ident]->getFdType() == FD_CLIENT)
				{
					// TODO: piti write arvi
				}
				else if (_fd_pool[curr_event->ident]->getFdType() == FD_RESOURCE)
				{
					// TODO: CGI u write, kam avel a :(
				}
			}
		}
		usleep(5);
	}
}

std::vector<struct kevent>&		Webserv::getChangeList()
{

}

void	Webserv::addFdPool(AFd* res)
{
	_fd_pool[res->getFd()] = res;
}


// kisat a grac, piti nax haskananq inchi fd a, u vonc enq pakum, vor leak chunenanq
void	Webserv::deleteFdPool(AFd* instance)
{
	if (instance == NULL)
		return ;
		
	close(instance->getFd());
}

void	Webserv::signalExit(){}

unsigned long	Webserv::getTime(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
