#include "Get.hpp"

// Private Constructor ----------------------------------------------------------------------------------------------------
Get::Get( void ) {

	return;
}
// Destructor -------------------------------------------------------------------------------------------------------------

Get::~Get( void ) {

	return;
}

// Get time ---------------------------------------------------------------------------------------------------------------

std::string	Get::Time(  )
{
	time_t _time;
	struct tm * timeinfo;
	const size_t size = 80;
	char buffer [size];

	time (&_time);
	timeinfo = localtime (&_time);

	strftime (buffer, size, "%FT%X", timeinfo );
	std::string time = std::string (buffer ) + " ";
	return time;
}

// Get addrinfo ----------------------------------------------------------------------------------------------------------

/*
   set the server's name to the standard host name for the current machine.
   source: https://www.ibm.com/docs/en/zos/2.3.0?topic=calls-gethostname
   hostname: Character array to be filled with the host name
   namelen: Length of name; restricted to 255 characters
   is guaranteed to be no less than {_POSIX_HOST_NAME_MAX}.
   you can also check the max value for namelen from the terminal with: getconf HOST_NAME_MAX
*/
void	Get::get_hostname( std::string & _hostname  ) {

	char hostname[ _POSIX_HOST_NAME_MAX ];
	memset( hostname, 0, _POSIX_HOST_NAME_MAX );

	int ret = gethostname( hostname, _POSIX_HOST_NAME_MAX );
	if ( ret < 0 )
	{
		std::cerr << Get::Time() << RED_BOLD << " --- gethostname() failed: " << strerror(errno) << END << std::endl;
		return;
	}
	_hostname = std::string( hostname );
	return;
}

/*
   we use getaddrinfo() to print all the IP addresses of the host
   and thus inform the clients how they can connect to our server.
   We print this information only once, when the server is bound.


   If hints.ai_flags includes the AI_CANONNAME flag,
   then the ai_canonname field of the FIRST of the addrinfo structures
   in the returned LIST is set to point to the official name of the host.
   If hostname is not NULL, then the AI_PASSIVE flag is ignored.
   -> this is why we don't use AI_PASSIVE
 */

std::string	Get::HostMachineAddrInfo()
{
	int	ret;
	struct addrinfo 	_hints;
	struct addrinfo		*_servinfo;
	std::string			hostname;

	get_hostname( hostname );

	_servinfo = NULL;
	memset( &_hints, 0, sizeof( struct addrinfo ) );

	_hints.ai_flags = AI_CANONNAME;						// fill in my IP for me
	_hints.ai_family = PF_INET;							// IPv4
	_hints.ai_socktype = SOCK_STREAM;					// TCP stream sockets
	_hints.ai_protocol = 0;								// any protocol

	ret = getaddrinfo( hostname.c_str(), NULL, &_hints, &_servinfo );
	if (ret != 0)
		std::string error_msg = Get::Time() + " Getaddrinfo error: " + std::string( gai_strerror(ret) );
	else
		printConnectionInfo( _servinfo );

	freeaddrinfo( _servinfo );
	return std::string (hostname );
}

void	Get::printConnectionInfo( struct addrinfo *& _servinfo ) {

	std::cout << Get::Time() << " ---" << GREEN_BG << BOLD << " Clients can connect to this server with: "<< END << std::endl;

	if (_servinfo->ai_canonname != nullptr)
		std::cout << Get::Time() << " ---" << GREEN_BG << BOLD  << " Cannonname: " << _servinfo->ai_canonname << " " << END << std::endl;

	struct addrinfo *ptr = _servinfo;
	for (; ptr != NULL; ptr = ptr->ai_next)
	{
		if (ptr->ai_family == AF_INET)
		{
			struct sockaddr_in *ipv4 = reinterpret_cast< struct sockaddr_in * >( ptr->ai_addr );
			char IP_adress[INET_ADDRSTRLEN];
			inet_ntop( AF_INET, &( ipv4->sin_addr ), IP_adress, INET_ADDRSTRLEN );
			std::cout << Get::Time () << " ---" << GREEN_BG << BOLD << " IPv4: " << IP_adress << " "<< END << std::endl;
		}
		else if (ptr->ai_family == AF_INET6)
		{
			struct sockaddr_in6 *ipv6 = reinterpret_cast< struct sockaddr_in6 *>( ptr->ai_addr );
			char IP_adress[INET6_ADDRSTRLEN];
			inet_ntop( AF_INET6, &( ipv6->sin6_addr ), IP_adress, INET6_ADDRSTRLEN );
			std::cout << Get::Time() << " ---" << GREEN_BG << BOLD << " IPv6: " << IP_adress << " " << END << std::endl;
		}
	}
	std::cout << Get::Time() << " ---" << DARK_GREEN_BG << BOLD << " If client is connecting from " << _servinfo->ai_canonname << " " << END << std::endl;
	std::cout << Get::Time() << " ---" << DARK_GREEN_BG << BOLD << " IPv4: 127.0.0.1 " << END << std::endl;
	std::cout << Get::Time() << " ---" << DARK_GREEN_BG << BOLD << " Canonname: localhost " << END << std::endl;
	return;
}