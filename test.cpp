#include "Irc.hpp"

int main (void)
{
	std::string hostname = "irc.libera.chat";
	std::string     _port = "6697";
	struct addrinfo _hints;
	struct addrinfo *_servinfo;

	_hints.ai_family = PF_INET;                     // don't care IPv4 or IPv6
	_hints.ai_socktype = SOCK_STREAM;              // TCP stream sockets
	_hints.ai_flags = AI_PASSIVE;                 // fill in my IP for me
	_hints.ai_protocol = 0;                         // any protocol

	int    status = getaddrinfo( hostname.c_str(),_port.c_str(), &_hints, &_servinfo );
	if (status != 0)
	{
		std::string error_msg = "Getaddrinfo error: " + std::string( gai_strerror(status) );
		return 1;
	}

	std::cout << " Getting address info successful" << std::endl;

	std::cout << "Server is hosted on: " << hostname << std::endl;

	std::cout << " To connect to this server, use this/these adresse/s: "<< std::endl;

	for (struct addrinfo *ptr = _servinfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if (ptr->ai_family == AF_INET)
		{
			struct sockaddr_in *ipv4 = reinterpret_cast< struct sockaddr_in * >( ptr->ai_addr );
			char IP_adress[INET_ADDRSTRLEN];
			inet_ntop( AF_INET, &( ipv4->sin_addr ), IP_adress, INET_ADDRSTRLEN );
			std::cout << " IPv4: " << IP_adress << std::endl;
		}
		else if (ptr->ai_family == AF_INET6)
		{
			struct sockaddr_in6 *ipv6 = reinterpret_cast< struct sockaddr_in6 *>( ptr->ai_addr );
			char IP_adress[INET6_ADDRSTRLEN];
			inet_ntop( AF_INET6, &( ipv6->sin6_addr ), IP_adress, INET6_ADDRSTRLEN );
			std::cout << " IPv6: " << IP_adress << END << std::endl;
		}
	}
}