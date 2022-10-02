#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"

class Response: public HTTP
{
    private:
        std::string raw_response;
        char    *   c_response;
        std::string status_line;
        std::string phrase;
    public:
        // Response(Request &); will be needed later
        Response();
        virtual ~Response();

        void    fill_phrase();
        void    fill_status_line();
        void    fill_headers();
        // void    fill_body();
        char    *get_c_response();
};

#endif