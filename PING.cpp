# include "PING.hpp"

PING::PING() {
}

PING::PING( std::string &line ) : ACommand( line ){
	return ;
}

PING::~PING( void ) {
}


void PING::execute( std::string & line, Client & client, Server &server ) {
	(void)line;
	(void)server;
	if ( command.size() == 1 )
		RPL::ERR_NOORIGIN( client );
	else
		RPL::RPL_PING( client );
	return ;
}
