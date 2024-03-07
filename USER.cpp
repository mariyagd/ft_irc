#include "USER.hpp"

USER::USER(void) : ACommand() {
	return;
}

USER::~USER( void ) {
	return;
}

//
/*
 * USER <username> <hostname> <servername> <realname>
 * USER <username> 0 * <realname>
 *
 *  If username lookups are enabled and a client does not have an Identity Server enabled,
 *  the username provided by the client SHOULD be prefixed by a tilde ('~', 0x7E)
 *  to show that this value is user-set.
 *
 * If a client tries to send the USER command after they have already completed
 * registration with the server, the ERR_ALREADYREGISTERED reply should be sent
 * and the attempt should fail.
 */
void USER::execute( std::vector< std::string > & command, Client & client, Server &server ) {

//	std::cout << Get::Time() << GREEN << " --- Processing USER command" << END << std::endl;

	(void )server;

	if ( command.size() < 5 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: USER <username> 0 * <realname>" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "USER" );
	}
	else if ( client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << command[1] << " already registered" << END << std::endl;
		RPL::ERR_ALREADYREGISTERED( client );
	}
	else
	{
		client.setUsername( command[1] );

		std::string realname;
		concatenate( command, 4, realname );
		if ( realname[0] != ':' )
			realname = ":" + realname;
		client.setRealname( realname );
	}
	return;
}
