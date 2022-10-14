#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <map>
# include "HTTP.hpp"

class Request: public HTTP
{
    private:
        std::string raw_request;
        std::string method;
        std::string uri;
        Request();
    public:
        Request(std::string raw_request);
        virtual ~Request();

        Request &operator=(const Request &);
        const std::string &get_method();
        const std::string &get_uri();
        const std::string &get_version();
        const std::map<std::string, std::string> &get_headers();
        const std::string &get_body();

        void    set_method(const std::string &);
        void    set_uri(const std::string &);
        void    set_version(const std::string &);
        void    set_body(const std::string &);

        void    parse_first_line(std::string raw_request);
        void    parse_headers(std::string raw_request);
        void    parse_body(std::string raw_request);
};

#endif

//return codes
//501 - method is not recognized or implemented by the server
//405 - method is not allowed on the requested resource
//414 - Request-URI Too Long