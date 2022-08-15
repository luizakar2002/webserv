#ifndef HTTP_HPP
# define HTTP_HPP

# include <iostream>
# include <map>

class HTTP
{
    protected:
        std::string version;
        int         status_code;
        std::map<std::string, std::string>  headers_map;
        std::string body;
    public:
        HTTP();
        virtual ~HTTP();

        const int   &get_status_code();
        const std::string &get_version();
        const std::map<std::string, std::string> &get_headers();
        const std::string &get_body();

        void    set_version(const std::string &);
        void    set_body(const std::string &);
};

#endif