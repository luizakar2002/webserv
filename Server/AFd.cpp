#include "AFd.hpp"

AFd::AFd(): _fd(-1)
{
}

AFd::AFd(const AFd &copy)
{
	*this = copy;
}

AFd::~AFd()
{
}

AFd	&AFd::operator=(const AFd &assign)
{
	_fd = assign.getFd();
	_fd_type = assign.getFdType();
	return *this;
}

int	AFd::getFd() const
{ 
	return _fd;
}

e_fd_type	AFd::getFdType() const
{
	return _fd_type;
}

void	AFd::setFd(int fd)
{
	_fd = fd;
}

void	AFd::setFdType(e_fd_type fd_type)
{
	_fd_type = fd_type;
}