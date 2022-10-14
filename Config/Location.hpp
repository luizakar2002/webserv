#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::map;

typedef std::map<int, std::string> error_page;
typedef std::map<std::string, std::vector<std::string> > prop_map; 

class Location
{
    private:
        int depth;
        string path;
        prop_map properties;
        error_page error_pages;
        //std::vector<struct error_page> error_pages;
    
    public:
        Location();
        Location(string &loc);
        Location(const Location &);
        Location &operator=(const Location &);
        string get_path() const;
        void set_path(string);
        prop_map get_props() const;
        void set_props(prop_map);
        int get_depth()const;
        void set_depth(int);
        error_page get_errors()const;
        void set_errors(error_page);

        ~Location();
};