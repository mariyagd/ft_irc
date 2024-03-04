#include "USER.hpp"

USER::USER(void) : ACommand() {
	return;
}

USER::~USER( void ) {
	return;
}

// USER <username> <hostname> <servername> <realname>
void USER::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	std::cout << Get::Time() << GREEN << " --- Processing USER command" << END << std::endl;

	(void )server;

	client.setUsername( "~" + command[1] );

//	client.setHostname( command[2] );
//	client.setServname( command[3] );

	std::string realname;
	for ( size_t i = 4; i < command.size(); i++ )
	{
		realname += command[i];
		if ( i != command.size() - 1 )
			realname += " ";
	}
	client.setRealname( realname );

}
