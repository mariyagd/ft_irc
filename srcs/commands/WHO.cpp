#include "WHO.hpp"

WHO::WHO( void ) {
	return;
}

WHO::~WHO( void ) {
	return;
}

/*
 * WHO <mask>
 */
void WHO::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	if ( !client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Client not registered" << END << std::endl;
		RPL::ERR_NOTREGISTERED( client );
		return;
	}

	if ( command.size() < 2 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: WHO <channel>" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "WHOIS" );
	}

	Channel * channel = server.getChannelByName( command[1] );
	if (!channel)
	{
		RPL::ERR_NOSUCHCHANNEL( client, command[1] );
		return;
	}
	RPL::RPL_WHOREPLY( client, command[1] );
	RPL::RPL_ENDOFWHO( client, command[1] );
	return ;
}