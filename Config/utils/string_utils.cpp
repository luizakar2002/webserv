#include "../Configs.hpp"

int find_char_occ(const string &str, char ch)
{
    return std::count(str.begin(), str.end(), ch);
}

void print_vector(vector<string> v)
{
    vector<string>::iterator uf = v.begin();
    while(uf != v.end())
    {
        std::cout << *uf << " ";
        ++uf;
    }
    std::cout << "\n";
}

void print_map(const prop_map &props)
{
    prop_map::const_iterator begin = props.cbegin();
    while(begin != props.end())
    {
        std::cout << "KEY: " << begin->first << " ==== ";
        print_vector(begin->second);
        ++begin;
    }
}
