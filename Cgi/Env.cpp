#include "Env.hpp"

Env::Env() {
    this->vars["SERVER_SOFTWARE"] = "WeBsErV/1.0";
    this->vars["SERVER_NAME"] = ""; // host 
    this->vars["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->vars["SERVER_PROTOCOL"] = "HTTP/1.1";
    this->vars["SERVER_PORT"] = ""; // port
    this->vars["REQUEST_METHOD"] = ""; // get / post / delete
    this->vars["PATH_INFO"] = ""; // ???
    this->vars["PATH_TRANSLATED"] = ""; // ??
    this->vars["SCRIPT_NAME"] = ""; // cgi path
    this->vars["REDIRECT_STATUS"] = "200"; 
    // this->vars["REMOTE_HOST"] = "";
    // this->vars["REMOTE_ADDR"] = "";
    this->vars["AUTH_TYPE"] = ""; // ??
    this->vars["REMOTE_USER"] = ""; // herders Authorization
    this->vars["REMOTE_IDENT"] = ""; // herders Authorization
    this->vars["CONTENT_TYPE"] = ""; // headers Content-Type
    this->vars["CONTENT_LENGTH"] = ""; // client request size ???????
    // this->vars["HTTP_ACCEPT"] = "";
    // this->vars["HTTP_USER_AGENT"] = "";

}

void Env::set_var(std::string key, std::string val)
{
    this->vars[key] = val;
}

char **Env::to_matrix()
{
    char **envp;
    std::map<std::string, std::string>::iterator iter = this->vars.begin();
    int i = 0;

    envp = new char *[this->vars.size()];
    while (iter != this->vars.end())
    {
        envp[i] = const_cast<char *>((iter->first + iter->second).c_str());
        ++i;
        ++iter;
    }
    return envp;
}

Env::~Env() {}