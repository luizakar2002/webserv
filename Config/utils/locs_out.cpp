#include "../Configs.hpp"

string get_locs_out(string &serv)
{
    int op = 0;
    bool has = false;
    string locs;
    string line;
    stringstream sserv(serv);

    while (getline(sserv, line))
    {
        if (!line.compare(0, 9, "location "))
        {
            has = true;
            op++;
        }
        else if (line.find("}") <= line.size())
            op--;
        if (op > 0)
            locs.append(line + "\n");
        if (op == 0 && has)
        {
            has = false;
            locs.append("}\n");
        }
    }
    return locs;
}