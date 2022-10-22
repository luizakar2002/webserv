#ifndef AFD_HPP
# define AFD_HPP


typedef enum
{
	FD_SERVER,
	FD_CLIENT,
	FD_RESOURCE // pipe or an open file
}	e_fd_type;

class AFd
{
	protected:
		int 		_fd;
		e_fd_type	_fd_type;
	public:
		AFd();
		AFd(const AFd &copy);

		virtual ~AFd();
		
		AFd	&operator=(const AFd &assign);
		
		int 		getFd() const;
		e_fd_type 	getFdType() const;
		void 		setFd(int);
		void 		setFdType(e_fd_type);
};

#endif