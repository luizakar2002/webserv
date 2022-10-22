#ifndef SERVER_PARAMS_HPP
# define SERVER_PARAMS_HPP

# include <iostream>

class server_params
{
    public:
        int domain;
        int type;
        int protocol;
        int port;
        u_long interface;
        int b_log;
    public:
        server_params(const std::string &); // was like this: unique_configs_element &host_conf_pair
        ~server_params();
};

# include "../MAIN.hpp"
#endif