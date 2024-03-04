#include "WHO.hpp"

WHO::WHO( void ) {
	return;
}

WHO::~WHO( void ) {
	return;
}

void WHO::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	if (command.size() != 2 )
		return;

	Channel * channel = server.getChannelByName( command[1] );
	if (!channel)
	{
		RPL::ERR_NOSUCHCHANNEL( client, command[1] );
		return;
	}
	RPL::RPL_ENDOFWHO( client, command[1] );
	return ;
}