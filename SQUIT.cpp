#include "SQUIT.hpp"

SQUIT::SQUIT( ) {

	return;
}

SQUIT::~SQUIT() {

	return;
}

void SQUIT::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	(void)client;
	(void)command;
	if ( command.size() >= 2 )
	{
		server.shutdown();
		exit(0);
	}
}