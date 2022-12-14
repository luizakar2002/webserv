#include "Response.hpp"

Response::Response(Request *request, Config &conf)
{
    // this->status_code needs to be taken from request as well
    this->version = "HTTP/1.1 ";
    this->request = request;
    this->config = conf;
    prop_map map = this->config.get_map();
    const std::string prop = "listen";
    std::vector<std::string>::iterator vect = map[prop].begin();

    std::cout << "in RESPONSE CTOR, listen directive of current config: " << *vect << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;

    fill_phrase();
    fill_status_line();
    hasExt();
    // check_static(); //read and store content then fill headers
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
    std::cout << "zibilik" << std::endl;
    std::cout << "->" << raw_response << "<- "<<  std::endl;
    this->raw_response += "Content-Type: text/html\nContent-Length: ";//<html>\n<body>\n<h1>Hello from server</h1>\n</body>\n</html>";
    this->raw_response += std::to_string(this->content.size()) + "\n\n";
    this->raw_response += this->content;
    std::cout << this->raw_response << std::endl;
    this->c_response = (char *)this->raw_response.c_str(); // consider casting into non-const again
}

// void    Response::fill_body()
// {

// }

void    Response::serve_non_static(std::string file_uri)
{

    std::vector<std::string> cgi_pass = config.get_map().find("cgi_pass")->second;

    std::cout << "CGI STUFF\n";
    std::string cgi_ext = "";
    std::string cgi_path = "";

    
    if (cgi_pass.size() == 2)
    {
        cgi_ext = cgi_pass.at(0);
        cgi_path = cgi_pass.at(1);
    }
    else
    {
        // 500 to be returned
    } 
    CGI cgi(cgi_ext, cgi_path, file_uri, &config, request);
}

void   Response::open_static(std::string file)
{
    std::cout << "\n\nIN_OPEN\n";
    std::string line;
    std::string path = "./www" + file;
    std::cout << "PATH: " << path << "\n";
    std::ifstream static_file(path);

    if (static_file.is_open())
    {
        while (static_file)
        {
            std::getline(static_file, line);
            this->content += line;
        }
        // while (static_file.good())
        //     static_file >> static_file_content;
        // std::cout << "CONTENT: " << static_file_content << "\n";
        static_file.close();
    }
    else
    {
        this->status_code = 404;
        std::cout << "COULDNT OPEN FILE\n";
    }
}


int Response::hasExt()
{
    std::string temp, uri = this->request->get_uri();
    size_t match = 0;

    std::cout << "URI: " << uri << "\n";
    std::cout << "\n\nCHECK_STATIC\n" << this->request->get_uri().size() << "\n\n";
    if (((match = uri.find_first_of("?", 0)) < uri.size()))
        uri.erase(match, uri.size() - match);
    if ((match = uri.find_last_of(".")) > uri.find_last_of("/"))
    {
        temp = uri.substr(match + 1, uri.size() - match);
        if (temp == "html" || temp == "htm")
        {
            std::cout << "URI: " << uri << "\n";
            open_static(uri);
            return (1);
        }
    }
    serve_non_static(uri);
    return 0;
}