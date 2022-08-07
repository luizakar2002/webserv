#include "Request.hpp"

int main()
{
    std::string str = "GET /hello.htm HTTPs/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\n\r\nrandombody";
    Request r = Request(str);
    return 0;
}