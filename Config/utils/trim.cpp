#include "../Configs.hpp"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

void removeSpaces(string &str)
{
    int n = str.length();
    int i = 0, j = -1;
    bool spaceFound = false;
 
    while (++j < n && str[j] == ' ');
    

    while (j < n)
    {
        // if current characters is non-space
        if (str[j] != ' ')
        {
            // remove preceding spaces before dot,
            // comma & question mark
            
                // copy current character at index i
                // and increment both i and j
                str[i++] = str[j++];
 
            // set space flag to false when any
            // non-space character is found
            spaceFound = false;
        }
        // if current character is a space
        else if (str[j++] == ' ')
        {
            // If space is encountered for the first
            // time after a word, put one space in the
            // output and set space flag to true
            if (!spaceFound)
            {
                str[i++] = ' ';
                spaceFound = true;
            }
        }
    }
    
    // Remove trailing spaces
    if (i <= 1)
        str.erase(str.begin() + i, str.end() - ((str.c_str()[str.size() - 1] == ';') ? 0 : 1));
    else
        str.erase(str.begin() + i - 1, str.end() - ((str.c_str()[str.size() - 1] == ';') ? 0 : 1));
}
