#include "PASS.hpp"

PASS::PASS( void ) : ACommand() {
}

PASS::~PASS( void ) {
}

void PASS::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	std::cout << Get::Time() << GREEN << " --- Processing PASS command" << END << std::endl;

	if ( command.size() == 1 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- socket " << client.getSocket() << " gave empty password. Disconnecting" << END << std::endl;

		RPL::ERR_NEEDMOREPARAMS( client, "PASS" );
		client.closeSocket();
	}
	else if ( command[1] != server.getPassword() && !client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- socket " << client.getSocket() << " gave invalid password. Disconnecting" << END << std::endl;
		RPL::ERR_PASSWDMISMATCH( client );
		client.closeSocket();
	}
	else if ( client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- socket " << client.getSocket() << " is already registered" << END << std::endl;
		RPL::ERR_ALREADYREGISTERED( client );
	}
	else
	{
		std::cout << Get::Time() << GREEN_BOLD << " --- socket " << client.getSocket() << " gave correct password" << END << std::endl;
		client.setGavePassword( true );
	}
	return;
}