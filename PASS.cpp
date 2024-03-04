#include "PASS.hpp"

PASS::PASS( void ) : ACommand() {
}

PASS::~PASS( void ) {
}

void PASS::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	std::cout << Get::Time() << GREEN << " --- Processing PASS command" << END << std::endl;

	if ( command.size() == 1 )
	{
		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " gave empty password. Disconnecting" << END << std::endl;

		RPL::ERR_NEEDMOREPARAMS( client, "PASS" );
		client.closeSocket();
	}
	else if ( command[1] != server.getPassword() )
	{
		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " gave invalid password. Disconnecting" << END << std::endl;
		client.closeSocket();
	}
	else
	{
		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " gave correct password" << END << std::endl;
		client.setGavePassword( true );
	}
	return;
}