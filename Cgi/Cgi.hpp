#ifndef CGI_HPP
# define CGI_HPP

# include <unistd.h>
# include <iostream>
# include <string>
# include "../Config/Configs.hpp"
# include "../Request.hpp"
# include "Env.hpp"


class CGI
{
private:
    std::string extention;
    std::string path;
    std::string response;
    std::string file;
    // std::string absolute_uri;
    Config      *config;
    Request     *request;
    Env         env;
    prop_map    props;
    std::string reponse;

    CGI();
public:
    CGI(std::string, std::string, std::string, Config *, Request *);
    ~CGI();

    prop_map        hasLocation();
    void            find_abs_path();
    int            find_exec_file();
    // void            set_env();
    void            handle_post(int fd);
    void            handle_get();

    void            cgi_handler();

    std::string     get_reponse();
};

#endif