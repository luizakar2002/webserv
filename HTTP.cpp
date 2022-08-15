#include "HTTP.hpp"

HTTP::HTTP(): status_code(200), body("")
{
}

HTTP::~HTTP()
{
}

const int   &HTTP::get_status_code()
{
    return this->status_code;
}

const std::string &HTTP::get_version()
{
    return this->version;
}

const std::map<std::string, std::string> &HTTP::get_headers()
{
    return this->headers_map;
}

const std::string &HTTP::get_body()
{
    return this->body;
}

void    HTTP::set_version(const std::string &version)
{
    this->version = version;
}

void    HTTP::set_body(const std::string &body)
{
    this->body = body;
}