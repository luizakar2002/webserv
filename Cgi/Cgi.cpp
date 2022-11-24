#include "Cgi.hpp"
#include <string>
#include <sys/socket.h>

CGI::CGI() {}
CGI::CGI(std::string ext, std::string cgi_path, std::string file, Config *conf, Request *req): 
    file(file), extention(ext), path(cgi_path), config(conf), request(req) 
{
    this->env = Env();
    find_abs_path(); // ./
    find_exec_file();
    set_env();
    cgi_handler();
}

void CGI::set_env()
{
    
}

void CGI::find_abs_path()
{
    // char cwd[256];

    // getcwd(cwd, 256);
    // this->path += std::string(cwd);
}

prop_map CGI::hasLocation()
{
    std::vector<Location>::iterator loc_iter = this->config->get_locations().begin();
    std::string plain_uri;
    prop_map props = config->get_map();

    while (loc_iter != this->config->get_locations().end())
    {
        plain_uri = this->request->get_uri().substr(this->request->get_uri().find_first_of('?'), this->request->get_uri().size() - this->request->get_uri().find_first_of('?'));
        if (plain_uri == loc_iter->get_path())
            props = loc_iter->get_props();
        ++loc_iter;
    }
    return props;
}

// std::string CGI::find_ext_file()
// {

// }

std::string CGI::full_file()
{
    std::cout << "CGI RESPONSE\n";
    std::vector<std::string> root = this->config->get_map().find("root")->second;
    std::string ffile;

    if (root.size() == 1)
    {
        if (root.at(0).at(0) == '/')
            ffile = root.at(0) + this->file;
        // else
            
    }
    else
    {
        // 500 to be returned
    }
}

void CGI::find_exec_file()
{
    std::vector<const std::string>::iterator index_iter;
    std::string temp_ext;

    if (this->file.empty() || this->file == "/")
    {
        // if ()
        // {
        index_iter = this->config->get_map().find("index")->second.begin();
        while (index_iter != this->config->get_map().find("index")->second.end())
        {
            temp_ext = index_iter->substr(index_iter->find_last_of('.') + 1, (index_iter->size() - 1 - index_iter->find_last_of('.')));
            if (temp_ext == this->extention)
            {
                this->file = *index_iter;
                break ;
            }
            ++index_iter;
        }
    }
    // this->file = ;
    std::cout << "File : " << this->file << std::endl;
}

void CGI::handle_get()
{
    std::string query;

    if (request->get_uri().find_first_of('?') < request->get_uri().size())
    {
        std::cout << request->get_uri() << std::endl;
        query = request->get_uri().substr(request->get_uri().find_first_of('?'), request->get_uri().size() - request->get_uri().find_first_of('?'));
    }
    // else
    //     query = 
    this->env.set_var("REQUEST_METHOD", "GET");
    this->env.set_var("QUERY_STRING", query);
    this->env.set_var("CONTENT_LENGTH", to_string(query.size()));
    this->env.set_var("SCRIPT_FILENAME", this->file);
}

void CGI::handle_post(int fd)
{
    this->env.set_var("REQUEST_METHOD", "POST");
    this->env.set_var("CONTENT_LENGTH", to_string(request->get_body().size()));
    write(fd, request->get_body().c_str(), request->get_body().size());
    this->env.set_var("SCRIPT_FILENAME", this->file);
}

void CGI::cgi_handler()
{
    pid_t pid;
    char **av;
    int fd[2];

    pipe(fd);    
    pid = fork();
    av = (char **)malloc(sizeof(char *) * 5);
    // av[0] = (char *)malloc(1000);
    av[0] = (char *)this->path.c_str();
    av[1] = (char *)this->file.c_str();

    // char ** env = this->env.to_matrix();
    // int i = 0;
    // while (env[i])
    // {
    //     std::cout << env[i] << std::endl;
    //     ++i;
    // }
    
    if (!pid)
    {
        if (request->get_method() == "GET")
        {
            handle_get();
        }
        else if (request->get_method() == "POST")
        {
            handle_post(fd[0]);
        }
        dup2(1, fd[1]);
        dup2(0, fd[0]);
        execve(av[0], av, this->env.to_matrix());

    }

    std::cout << "CGI RESPONSE\n";
    std::cout << "<<<<<<<<<>>>>>>>>>\n";
    char *buf;

    buf = (char *)malloc(10000);
    memset(buf, 0, 10000);
    read(fd[1], buf, 10000);

    std::cout << buf << std::endl;
}

// Breath air and see the wallpaper :D © some game devist



CGI::~CGI() {}