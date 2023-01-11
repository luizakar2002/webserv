# include "MAIN.hpp"
# include "Server/Headers.hpp"


bool isExist(std::string path)
{
	struct stat info;
	return (stat(path.c_str(), &info) == 0);
}

bool isDirectory(std::string path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return false;
    else if (info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

int main(int ac, char **av)
{
    // std::string str = "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\n\r\nrandombody";
    // Request r = Request(str);
    
    std::vector<Config> configs;
    char buf[1025];
    getcwd(buf, 1024);
    if (ac == 2 && isExist(std::string(buf) + "/" + std::string(av[1])))
    {
        start_parse(av[1], configs);
        Webserv webserv = Webserv(configs);
    }
    else 
    {
        std::cout << "Config file doesn't exist\n";
    }
        

    // Server s = Server();
    // std::cout << "in main" << configs.size() << std::endl;
    
    
    return 0;
}