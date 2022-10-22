#ifndef MAIN_HPP
# define MAIN_HPP

# include "Config/Configs.hpp"
# include "Sockets/Sockets.hpp"
# include "Server/Headers.hpp"
# include "Utils/Utils.hpp"



typedef std::map< std::string, std::vector<Config> > unique_configs; //map: key - ip:host, value - vector of configs with same ip:host
// typedef std::pair< const std::string, std::vector<Config> > unique_configs_element; //element of above map is not needed yet

    

#endif