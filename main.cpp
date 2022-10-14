# include "Webserv.hpp"

using namespace ns;

int main(int ac, char **av)
{
    // std::string str = "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com\r\n\r\nrandombody";
    // Request r = Request(str);
    
    std::vector<Config> configs;
    if (ac == 2)
        start_parse(av[1], configs);

    std::vector<Config>::iterator conf = configs.begin(); //iterator for while loop
    unique_configs unique_config_vectors;  //result map with keys as ip:host and value as vector of not unique server configs
    unique_configs::iterator listen_dir; //to find where to store current conf
    // prop_map conf_prop_map;  //for storing return value of conf->get_map() to find value of "listen" key
    prop_map::iterator prop_map_listen_dir;
    while (conf != configs.end())
    {
        prop_map conf_prop_map(conf->get_map());
        prop_map_listen_dir = conf_prop_map.find("listen");
        if (prop_map_listen_dir != conf_prop_map.end()) // if there is a listen directive in prop_map of config
        {
            listen_dir = unique_config_vectors.find(prop_map_listen_dir->second[0]);
            if (listen_dir != unique_config_vectors.end())
            {
                listen_dir->second.push_back(*conf);
            }
            else
            {
                std::vector<Config> unique_config_vectors_value;
                unique_config_vectors_value.push_back(*conf);
                std::pair< std::string, std::vector<Config> > p = std::make_pair(prop_map_listen_dir->second[0], unique_config_vectors_value);
                unique_config_vectors.insert(p);
            }
        }
        conf++;
    }
    
    // HAVE TO TYPENAME AND CHANGE THE CONSTRUCTOR
    // server_params *s_params = new server_params;
    unique_configs::iterator it = unique_config_vectors.begin();
    while (it != unique_config_vectors.end())
    {
        std::cout << "IN WHILE" << std::endl;
        server_params s_params = server_params(it->first);
        ns::Server s = ns::Server(s_params, it->first, it->second);
        it++;
    }

    // std::vector<Config>::iterator conf = configs.begin();
    // while (conf != configs.end())
    // {
    //     ns::Server s = ns::Server(domain, type, protocol, port, interface, b_log, *conf);
    //     conf++;
    // }
    
    return 0;
}