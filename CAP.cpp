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

	if (command[1] == "END")
		return;
	else
		RPL::RPL_CAP(client);
	return;
}