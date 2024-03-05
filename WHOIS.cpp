#include "WHOIS.hpp"

WHOIS::WHOIS( void ) {
	return;
}

WHOIS::~WHOIS( void ) {
	return;
}

/*
 * WHOIS [<target>] <nick>
 */
void WHOIS::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	if ( command.size() < 2 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: WHOIS <nick>" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "WHOIS" );
	}
	if ( command.size() == 3 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- This server handles only: WHOIS <nick>" << END << std::endl;
		return;
	}
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
	RPL::ERR_NOSUCHNICK( client, nickname );
	return ;
}