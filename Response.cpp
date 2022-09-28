#include "Response.hpp"

Response::Response()
{
    // this->status_code needs to be taken from request as well
    this->version = "HTTP/1.1 ";
    fill_phrase();
    fill_status_line();
    fill_headers();
    // fill_body();
}

Response::~Response()
{
}

char    *Response::get_c_response()
{
    return this->c_response;
}

void    Response::fill_phrase()
{
    if (this->status_code == 200)
        this->phrase = "OK";
    //consider other status codes
}

void    Response::fill_status_line()
{
    this->status_line += this->version + std::to_string(this->status_code) + " " + this->phrase + "\r\n";
    this->raw_response += this->status_line;
}

void    Response::fill_headers()
{
    this->raw_response += "Content-Type: text/html\nContent-Length: 56\n\n<html>\n<body>\n<h1>Hello from server</h1>\n</body>\n</html>";
    this->c_response = (char *)this->raw_response.c_str(); // consider casting into non-const again
}