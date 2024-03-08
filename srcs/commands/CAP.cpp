#include "CAP.hpp"

CAP::CAP( void ) {
	return;
}

CAP::~CAP( void ) {
	return;
}

void CAP::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	(void)command;
	(void)server;

	if ( client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Already registered" << END << std::endl;
		RPL::ERR_UNKNOWNCOMMAND( client, command[0]);
		return;
	}
	if (command[1] == "END")
	{
		std::cout << Get::Time() << GREEN_BOLD << " --- Capabilities negotiation end. Client can proceed with registration" << END << std::endl;
		return;
	}
	if (command[1] == "LS")
	{
		std::cout << Get::Time() << BOLD << " --- Capabilities negotiation starts." << END << std::endl;
		RPL::RPL_CAP(client);
	}
	else
	{
		std::cout << Get::Time() << RED_BOLD << " --- Unknown CAP subcommand" << END << std::endl;
		RPL::ERR_INVALIDCAPCMD(client, command[1] );
	}
	return;
}