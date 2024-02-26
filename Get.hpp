#ifndef GET_HPP
# define GET_HPP

# include <time.h>
# include <string>
# include <limits.h>
# include "Colors.h"
# include <iostream>
# include <sys/types.h>			// for getaddrinfo, gai_strerror
# include <netdb.h>				// for getaddrinfo, gai_strerror
# include <arpa/inet.h>			// for inet_ntop
# include <unistd.h>				//for gehostname


class Get {

private:
	Get( void );

public:
	~Get( void );
	static std::string		Time( void );
	static int				Addrinfo( void );
	static void				printConnectionInfo( struct addrinfo *& _servinfo );
	static int			get_hostname( char **hostname );

};

#endif