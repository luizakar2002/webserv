#include "Cgi.hpp"
#include <string>
#include <sys/socket.h>
#include <signal.h>


CGI::CGI() {}
CGI::CGI(std::string ext, std::string cgi_path, std::string file, Config *conf, Request *req): 
    file(file), extention(ext), path(cgi_path), config(conf), request(req) 
{
    this->env = Env();
    this->reponse = "";
    find_abs_path(); // ./
    if (!find_exec_file())
        return ;
    cgi_handler();
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


std::string CGI::get_reponse()
{
    return this->reponse;
}

/** 
 *
 * 
 * find index file for uris
 * 
 * **/
int CGI::find_exec_file()
{
    std::vector<const std::string>::iterator index_iter;
    std::string temp_ext;
    std::string root = this->config->get_map().find("root")->second[0];

    if (this->file.empty() || this->file == "/")
    {
        if (this->config->get_map().count("index"))
        {
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
        else
        {
            return 0;
        }
    }
    this->file = root + "/" + this->file;
    return 1;
}

void CGI::handle_get()
{
    std::string query;

    if (request->get_uri().find_first_of('?') < request->get_uri().size())
        query = request->get_uri().substr(request->get_uri().find_first_of('?') + 1, request->get_uri().size() - request->get_uri().find_first_of('?'));
    // else
    //     query = 
    this->env.set_var("REQUEST_METHOD","GET");
    this->env.set_var("QUERY_STRING", query);
    this->env.set_var("CONTENT_LENGTH", to_string(query.size()));
    this->env.set_var("CONTENT_TYPE", (request->get_headers().count("Content-Type")) ? request->get_headers().find("Content-Type")->second : "text/plain");
    this->env.set_var("SCRIPT_FILENAME", this->file);
    this->env.set_var("PATH_INFO", this->path);
    this->env.set_var("REQUEST_URI", request->get_uri());
}

void CGI::handle_post(int fd)
{
    this->env.set_var("REQUEST_METHOD", "POST");
    this->env.set_var("QUERY_STRING", "");
    this->env.set_var("CONTENT_LENGTH", to_string(request->get_body().size()));
    this->env.set_var("CONTENT_TYPE", (request->get_headers().count("Content-Type")) ? request->get_headers().find("Content-Type")->second : "text/plain");
    this->env.set_var("SCRIPT_FILENAME", this->file);
    this->env.set_var("REQUEST_URI", request->get_uri());
    this->env.set_var("PATH_INFO", this->path);
}

void CGI::cgi_handler()
{
    pid_t   pid;
    int     saveIn;
    int     saveOut;
    char **av;
    std::string newBody;

    saveIn = dup(STDIN_FILENO);
	saveOut = dup(STDOUT_FILENO);
    FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;


    write(fdIn, request->get_body().c_str(), request->get_body().size());
    lseek(fdIn, 0, SEEK_SET);
    pid = fork();
    av = (char **)malloc(sizeof(char *) * 5);
    av[0] = (char *)this->path.c_str();
    av[1] = NULL;
    if (!pid)
    {
        if (request->get_method() == "GET")
            handle_get();
        else if (request->get_method() == "POST")
        {
            handle_post(0);
        }
        char ** argv = this->env.to_matrix();
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(av[0], av, argv);
		std::cerr << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
    }
    else 
    {
    	char	buffer[65536] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 65536);
			ret = read(fdOut, buffer, 65535);
			this->reponse += buffer;
		}
        dup2(saveIn, STDIN_FILENO);
	    dup2(saveOut, STDOUT_FILENO);
	    std::fclose(fIn);
	    std::fclose(fOut);
	    close(fdIn);
	    close(fdOut);
	    close(saveIn);
	    close(saveOut);
    }    
}




CGI::~CGI() {}