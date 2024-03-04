#include "WHOIS.hpp"

WHOIS::WHOIS( void ) {
	return;
}

WHOIS::~WHOIS( void ) {
	return;
}

void WHOIS::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	std::vector< Client > & connections = server.getConnections();

	std::string nickname = command[1];
	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
	{
		if ( connections[i].getSocket() >= 0 && connections[i].isRegistered() && connections[i].getNickname() == nickname )
		{
			RPL::RPL_WHOISUSER( client );
			RPL::RPL_ENDOFWHOIS( client );
			return ;
		}
	}
	return ;
}