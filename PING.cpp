# include "PING.hpp"

PING::PING() {
}

PING::~PING( void ) {
}

/*
 * PING <token>
 */
void PING::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	(void)server;

	if ( command.size() == 1 )
		RPL::ERR_NOORIGIN( client );
	else
		RPL::RPL_PING( client );
	return ;
}
