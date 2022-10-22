#include "Request.hpp"

Request::Request()
{
}

Request::Request(std::string raw_request): HTTP()
{
    this->raw_request = raw_request;
    this->parse_first_line(this->raw_request);
    this->parse_headers(this->raw_request);
    this->parse_body(this->raw_request);
    if (this->status_code != 200)
        std::cout << "Parsing Error with status code " << this->status_code << std::endl;
}

Request::~Request()
{
}

Request &Request::operator=(const Request   &other)
{
    raw_request = other.raw_request;
    method = other.method;
    uri = other.uri;
    return *this;
}

void    Request::parse_first_line(std::string raw_request)
{
    std::string protocol;

    std::string first_line = raw_request.substr(0, raw_request.find("\r\n"));
    this->method = first_line.substr(0, first_line.find_first_of(' '));
    if (this->method != "GET" && this->method != "POST" && this->method != "DELETE")
        this->status_code = 501;
    first_line.erase(0, this->method.size() + 1); //+1 for leading space
    this->uri = first_line.substr(0, first_line.find_first_of(' '));
    first_line.erase(0, this->uri.size() + 1);
    this->version = first_line;
    protocol = this->version.substr(this->version.find_first_of('/') + 1);
    if (this->version.substr(0, this->version.find_first_of('/')) != "HTTP")
        this->status_code = 400; //Bad raw_request
    if (protocol != "1.0" && protocol != "1.1")
        this->status_code = 403; //Forbidden
    std::cout << this->method << "\n" << this->uri << "\n" << this->version << std::endl;
}

void    Request::parse_headers(std::string raw_request)
{
    std::string header;
    std::string key;
    std::map<std::string, std::string>::iterator it;
    int host_header_count;

    std::string headers = raw_request.erase(0, raw_request.substr(0, raw_request.find("\r\n")).size() + 2);
    host_header_count = 0;
    while ((header = headers.substr(0, headers.find("\r\n"))) != "")
    {
        headers.erase(0, header.size() + 2);
        key = header.substr(0, header.find_first_of(':'));
        if (key == "Host")
            host_header_count++;
        header.erase(0, key.size() + 2); //+2 for ':' and ' '
        if (header.empty())
            host_header_count = -1;
        this->headers_map.insert(std::pair<std::string, std::string>(key, header));
    }
    if (host_header_count != 1)
        this->status_code = 400; //Bad request: many Host, no Host or Host value_field is empty (here checks if there is at least one empty value_field)
    for (it = this->headers_map.begin(); it != this->headers_map.end(); it++)
        std::cout << "{" << it->first << ": " << it->second << "}" << std::endl;
}

void    Request::parse_body(std::string raw_request)
{
    this->body = raw_request.substr(raw_request.find("\r\n\r\n") + 4);
    std::cout << "body: " << this->body << std::endl;
}

const std::string &Request::get_method()
{
    return this->method;
}

const std::string &Request::get_uri()
{
    return this->uri;
}

const std::string &Request::get_version()
{
    return this->version;
}

const std::map<std::string, std::string> &Request::get_headers()
{
    return this->headers_map;
}

const std::string &Request::get_body()
{
    return this->body;
}

void    Request::set_method(const std::string &method)
{
    this->method = method;
}

void    Request::set_uri(const std::string &uri)
{
    this->uri = uri;
}

void    Request::set_version(const std::string &version)
{
    this->version = version;
}

void    Request::set_body(const std::string &body)
{
    this->body = body;
}