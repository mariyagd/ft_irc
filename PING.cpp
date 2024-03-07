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
	if ( !client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Client not registered" << END << std::endl;
		RPL::ERR_NOTREGISTERED( client );
		return;
	}
	if ( command.size() == 1 )
		RPL::ERR_NOORIGIN( client );
	else
		RPL::RPL_PING( client );
	return ;
}
