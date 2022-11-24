#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"
# include "Config/Configs.hpp"
# include "Cgi/Cgi_headers.hpp"
# include <unistd.h>
# include <fstream>
# include <sstream>

class Response: public HTTP
{
    private:
        std::string     raw_response;
        char            *c_response;
        std::string     status_line;
        std::string     phrase;
        std::string     content;
        Request         *request;
        Config          config;
        Response();
    public:
        Response(Request *, Config &); // will be needed later
        virtual ~Response();

        void            fill_phrase();
        void            fill_status_line();
        void            fill_headers();
        // void            fill_body();
        char            *get_c_response();
        void            fill_static_content();
        int             hasExt();
        void             serve_non_static(std::string);
        void            open_static(std::string file);
};

#endif