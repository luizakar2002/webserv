#ifndef ENV_HPP
# define ENV_HPP

# include "../Config/Configs.hpp"


class Env
{
private:
/*  SERVER_SOFTWARE
    SERVER_NAME
    GATEWAY_INTERFACE
    SERVER_PROTOCOL
    SERVER_PORT
    REQUEST_METHOD
    PATH_INFO
    PATH_TRANSLATED
    SCRIPT_NAME
    AUTH_TYPE
    REMOTE_USER
    REMOTE_IDENT
    CONTENT_TYPE
    CONTENT_LENGTH

https://www6.uniovi.es/~antonio/ncsa_httpd/cgi/env.html
*/

std::map<std::string, std::string> vars;

public:
    Env();
    // Env(Config *); may be needed later 

    void            set_var(std::string, std::string);
    // std::string     get_var(std::string); // may use later 
    char **         to_matrix();
    ~Env();
};


#endif