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
        std::string     mime;
        Request         *request;
        Config          config;
        Location        location;
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
        int             hasExt(std::string &, std::string &);
        int             is_non_static(std::string);
        void            serve_non_static(std::string);
        void            open_static(std::string file);
        int             check_autoindex();
        void            create_autoindex();
        void            finder(std::string &, std::string &);
        int             makeFileList(std::string &);
        void            find_correct_location(std::string &uri);
        void            error(int status);
        void            ext_mime(std::string &, std::string &);
        void            open_file(std::string);
};

#endif