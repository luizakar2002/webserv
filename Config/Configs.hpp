#ifndef CONFIGS_HPP
# define CONFIGS_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include <sstream>
# include <fstream>
# include <algorithm>
# include "Config.hpp"

using namespace std;

void	ft_error(int , string );
int		start_parse(char *, vector<Config> &);
void	removeSpaces(string &);
string	trim(const string &);
int		find_char_occ(const string &, char);
void	print_vector(vector<string>);
void    print_map(const prop_map &);
string  get_locs_out(string &serv);

#endif