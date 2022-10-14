# include "server_params.hpp"

server_params::server_params(const std::string &ip_port)
{
    size_t pos = ip_port.find_first_of(':');
    if (pos != std::string::npos)
        port = std::stoi(ip_port.substr(pos + 1, std::string::npos));
    else
        port = std::stoi(ip_port);

    std::cout << "PORT in server_params: " << port << std::endl;
    domain = AF_INET;
    type = SOCK_STREAM;
    protocol = 0;
    interface = INADDR_ANY;
    b_log = 32;
}

server_params::~server_params()
{
}