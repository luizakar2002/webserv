#include "../Configs.hpp"

void ft_error(int status, string error)
{
    cout << "Error " << status <<  " : " << error << std::endl;
    exit(status);
}