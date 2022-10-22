#ifndef LISTENING_SOCKET_HPP
# define LISTENING_SOCKET_HPP

# include "BindingSocket.hpp"

class ListeningSocket: public BindingSocket
{
    private:
        int backlog;
        int listening; //return value of listen
    public:
        ListeningSocket(int &, int &, int &, int &, u_long &, int &);
        virtual ~ListeningSocket();

        void    start_listening();

        void        set_backlog(const int &);
        const int &get_backlog() const;
};

#endif