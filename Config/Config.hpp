#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Location.hpp"

using std::string;
using std::vector;
using std::map;

// using std::string;

struct location_str {
    string loc;
    int level;
    // int checked;
    // int checked = 0;
};
// tyepdef location_elem map<string, prop_map>
// tyepdef error_page std::map<int, std::string>

class Config{
    private:
        std::vector<Location> locations;
        vector<location_str> loc_strs;
        error_page error_pages;
        prop_map properties;
        string string_to_trim;
        
    public:
        Config();
        Config(std::string arg, string locations);
        
        const prop_map          &get_map()const;
        std::vector<Location>   get_locations()const;
        error_page              get_errors()const;
        void                    string_to_map();
        void                    add_error(string key, string arg);
        void                    parse_locations(string locations, string arg);
        void                    fill_location_paths(vector<string> &loc_list);
        void                    fill_location_map(vector<string> loc_list);
        void                    fill_locs_arr1(vector<string> loc_list, string locs);
        void                    fill_locs_arr2(string location, int loc_index);
        void                    useLocations();
        void                    fillaret(std::vector<location_str>::iterator iter, Location &temp_loc);
        void                    splitndestroy(string loc, string temp1);
        Location                jdi_menya(string s);
        void                    add_error_to_location(string key, string arg, Location &loc_temp);
        

        ~Config();

};