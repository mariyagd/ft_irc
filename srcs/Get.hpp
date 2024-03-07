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

class  Server;

class Get {

private:
	Get( void );

public:
	~Get( void );
	static std::string		Time( void );
	static std::string		HostMachineAddrInfo( Server & server );
	static void				printConnectionInfo( struct addrinfo *& _servinfo );
	static void				get_hostname( std::string & hostname );

};

#endif