#include "Commands.hpp"

// Coplien's form------------------------------------------------------------------------------------------------------

Commands::Commands( void ) {
	return ;
}

Commands::Commands( Commands const & src ) {

	( void )src;
	return ;
}

Commands & Commands::operator=( Commands const & rhs ) {

	( void )rhs;
	return *this;
}

Commands::~Commands( void ) {
	return ;
}

// ---------------------------------------------------------------------------------------------------------------------

void	Commands::process_command( std::string & msg, int i, Server & server )
{
	std::vector< std::string > tokens;
	std::istringstream iss( msg );

	while ( iss.eof() == false )
	{
		std::string token;
		iss >> token;
		tokens.push_back( token );
	}

	if ( tokens[0] == "NICK" )
	{
		std::cout << "Process NICK command" << std::endl;
		NICK( tokens[1], i, server );
	}
	return ;
}

void Commands::NICK( std::string &nickname, int j, Server &server ) {

	for ( size_t i = 0; i < nickname.length(); i++ )
	{
		if ( std::isspace(nickname[i]) || nickname[i] == '+' || nickname[i] == '-' || \
			 nickname[i] == ':' || nickname[i] == '!' || nickname[i] == '@' || nickname[i] == '#' || nickname[i] == '$' || \
			 nickname[i] == '%' || nickname[i] == '&' || nickname[i] == '*' || nickname[i] == ';' || nickname[i] == ',' || \
			 nickname[i] == '.' || nickname[i] == '/' || nickname[i] == '<' || nickname[i] == '>' || nickname[i] == '?' || \
			 nickname[i] == '=' || nickname[i] == '"' || nickname[i] == '\''  || nickname[i] == '~' )
		{
			char message[MSG_MAX_SIZE];
			snprintf( message, MSG_MAX_SIZE, ":%s 432 %s Erroneous nickname\r\n", server.getServerName(), nickname.c_str() );
			send( server.getConnections()[j].getSocket(), message, strlen(message), 0 );
			return ;
		}
	}
	if ( server.is_unique_nickname( nickname ) == false )
	{
		char message[MSG_MAX_SIZE];
		snprintf( message, MSG_MAX_SIZE, ":%s 433 %s %s\r\n", server.getServerName(), server.getConnections()[j].getNickname().c_str() ,nickname.c_str() );
		send( server.getConnections()[j].getSocket(), message, strlen(message), 0 );
		return;
	}
//	for ( int i = 0; i < MAX_CONNECTIONS; ++i )
//	{
//		if ( i != j &&  server.getConnections()[i].getSocket() >= 0 && server.getConnections()[i].getNickname() == nickname )
//		{
//			char message[MSG_MAX_SIZE];
//			snprintf( message, MSG_MAX_SIZE, ":%s 433 %s Nickname is already in use\r\n", server.getServerName(), nickname.c_str() );
//			send( server.getConnections()[j].getSocket(), message, strlen(message), 0 );
//			return;
//		}
//	}

	char message[MSG_MAX_SIZE];
	std::string time = PrintTime::printTime();
	snprintf( message, MSG_MAX_SIZE, "@time=%s:%s!%s@%s NICK %s\r\n", time.c_str(), server.getConnections()[j].getNickname().c_str(), server.getConnections()[j].getUsername().c_str(), server.getConnections()[j].getHostname().c_str(), nickname.c_str() );
	send( server.getConnections()[j].getSocket(), message, strlen(message), 0 );

	server.getConnections()[j].setNickname( nickname );
	server.getConnections()[j].printInfo();
	return ;
}
