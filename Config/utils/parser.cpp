#include "../Configs.hpp"

int check_conf(string cont)
{
    size_t match_s = cont.find("{", 0);
    size_t match_e = cont.find("}", 0);
    int count = 0;

    if (match_e < match_s)
        return (1);
    while (1)
    {
        match_s = cont.find("{", match_s + 1);
        if (match_s < cont.size())
            ++count;
        if (match_s > cont.size())
            break;
    }
    while (1)
    {
        match_e = cont.find("}", match_e + 1);
        if (match_e < cont.size())
            --count;
        if (match_e > cont.size())
            break;
    }
    if (count != 0)
        return 1;
    return 0;
}

void uncomment(string &cont)
{
    size_t start = 0, end = 0;
    while (1)
    {
        start = cont.find("#", 0);
        end = cont.find("\n", start);
        if (start <= cont.size() && end <= cont.size())
            cont.erase(start, end - start);
        else 
            break ;
    }
} 

size_t find_server_end(string &cont)
{
    int op = 0, cl = 0;
    istringstream cont_stream(cont);
    string line;
    size_t pos = 0;

    // if (cont.find("server {", 0, 8) > 0)
    //     ft_error(127, "Use of undefined property!");
    while (std::getline(cont_stream, line))
    {
        if (line.find("{") <= line.size())
            op++;
        if (line.find("}") <= line.size())
            cl++;
        if (cl > 0 && cl == op)
            break;
    }
    while (cl > 0)
    {
        pos = cont.find("}", pos + 1);
        cl--;
    }
    // std::cout << "POS IS " << pos << std::endl;
    return pos;
}

int split_servers(string &cont, vector<Config> &servers)
{
    size_t i = 0;
    string ret;
    string locations;

    uncomment(cont);
    removeSpaces(cont);
    if (check_conf(cont))
        ft_error(127, "Bad quotes!");
    while ((i = find_server_end(cont)) < cont.size())
    {
        ret = trim(cont.substr(0, i + 1));

        locations = get_locs_out(ret);
        Config s(ret, locations);
        s.string_to_map();
        
        if (cont.length() - i != 1)
            cont.erase(0, cont.find("server {", i, 8));
        else
            cont.erase(0, i);
        servers.push_back(s);
    }
    return 0;
}

int start_parse(char *confpath, vector<Config> &servers)
{
    string filepath = (string)confpath;
    string temp;
    string config;
    ifstream conf;
    conf.open(filepath, ios::in);

    while (getline(conf, temp))
    {
        config = config.append(trim(temp));
        config.append("\n");
    }
   
    while (split_servers(config, servers))
        continue ;
    return 0;
}