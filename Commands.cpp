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

bool	Commands::isCommand( const std::string &token, std::vector< std::string > & availableCommands ) {

	for ( size_t i = 0; i < availableCommands.size(); i++ )
	{
		if ( token == availableCommands[i] )
			return true;
	}
	return false;
}

void	Commands::initAvailableCommands( std::vector< std::string > & availableCommands ) {

	availableCommands.push_back( "PASS" );
	availableCommands.push_back( "NICK" );
	availableCommands.push_back( "USER" );
	availableCommands.push_back( "MODE" );
	availableCommands.push_back( "PING" );
	availableCommands.push_back( "WHOIS" );
	return ;
}

void	Commands::deleteCommandFromList( std::string & token, std::vector< std::string > & availableCommands ) {

	for ( size_t i = 0; i < availableCommands.size(); i++ )
	{
		if ( token == availableCommands[i] )
		{
			availableCommands.erase( availableCommands.begin() + i );
			return ;
		}
	}
	return ;
}

void	Commands::process_command( std::string & msg, int i, Server & server )
{
//	std::cout << PrintTime::printTime() << " --- Processing message" << std::endl;

	std::vector< std::string > tokens;
	std::vector< std::string > command;
	std::vector< std::string > availableCommands;

	initAvailableCommands( availableCommands );
	std::istringstream iss( msg );

	size_t pos = 0;

	while ( !iss.eof() )
	{
		std::string token;
		iss >> token;
		if ( (pos = token.find( "\n" )) != std::string::npos )
		{
			token.erase( pos, 1 );
		}
		tokens.push_back( token );
	}

	for ( size_t j = 0; j < tokens.size(); j++ )
	{
		if ( isCommand( tokens[j], availableCommands ) )
		{
			deleteCommandFromList( tokens[j], availableCommands );
			command.push_back( tokens[j] );
			for ( ++j; j < tokens.size() && !isCommand( tokens[j], availableCommands ); j++ )
			{
				command.push_back( tokens[j] );
			}
			--j;
			choose_command( command, i, server );
			command.clear();
		}
	}
	return ;
}

void Commands::choose_command( std::vector <std::string> & command, int i, Server & server ) {

	if ( command[0] == "PASS" )
	{
		PASS( command, i, server );
	}
	else if ( command[0] == "NICK" )
	{
		NICK( command, i, server );
	}
	else if ( command[0] == "USER" )
	{
		USER( command, i, server );
	}
	else if ( command[0] == "WHOIS" )
	{
		WHOIS( command, i, server );
	}
	else if ( command[0] == "PING" )
	{
		PING( command, i, server );
	}
	return ;
}


// NICK------------------------------------------------------------------------------------------------------------------

bool Commands::is_unique_nickname(  std::string & nickname, std::vector< Client > & connections )
{
	for ( int i = 0; i < MAX_CONNECTIONS; ++i )
	{
		if ( connections[i].getSocket() >= 0 && connections[i].getNickname() == nickname )
			return false;
	}
	return true;
}

void Commands::NICK( std::vector< std::string > & command, int j, Server &server ) {

//	std::cout << PrintTime::printTime() << GREEN << " --- Processing NICK command" << END << std::endl;

	std::string message ="";
	std::string nickname = command[1];
	Client & client = server.getConnections()[j];

	for ( size_t i = 0; i < nickname.length(); i++ )
	{
		if ( std::isspace(nickname[i]) || nickname[i] == '+' || nickname[i] == '-' || \
			 nickname[i] == ':' || nickname[i] == '!' || nickname[i] == '@' || nickname[i] == '#' || nickname[i] == '$' || \
			 nickname[i] == '%' || nickname[i] == '&' || nickname[i] == '*' || nickname[i] == ';' || nickname[i] == ',' || \
			 nickname[i] == '.' || nickname[i] == '/' || nickname[i] == '<' || nickname[i] == '>' || nickname[i] == '?' || \
			 nickname[i] == '=' || nickname[i] == '"' || nickname[i] == '\''  || nickname[i] == '~' )
		{
			message = RPL::ERR_ERRONEUSNICKNAME( client, server );
			send( client.getSocket(), message.c_str(), message.size(), 0 );
			std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;
			return ;
		}
	}
	client.setNickname( nickname );

	if ( !is_unique_nickname( nickname, server.getConnections() ) )
	{
		message = RPL::ERR_NICKNAMEINUSE( client, server, nickname );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;
		return;
	}

//	message = RPL::RPL_SAVENICK( client, server );
//	send( client.getSocket(), message.c_str(), message.size(), 0 );
//	std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

	message = RPL::RPL_NICK( client, server, nickname );
	send( client.getSocket(), message.c_str(), message.size(), 0 );
	std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

	std::cout << PrintTime::printTime() << " --- Nickname " << client.getNickname() << " changed to " << nickname << END << std::endl;
	client.setNickname( nickname );
	return ;
}

// USER <username> <hostname> <servername> <realname>--------------------------------------------------------------------

void Commands::USER( std::vector <std::string> & command, int i, Server & server ) {

//	std::cout << PrintTime::printTime() << GREEN << " --- Processing USER command" << END << std::endl;

	std::string 	message;
	Client & 		client = server.getConnections()[i];

	client.setUsername( command[1] );
	client.setHostname( command[2] );
	client.setServname( command[3] );

	std::string realname;
	for ( size_t j = 4; j < command.size(); j++ )
	{
		realname += command[j];
		if ( j != command.size() - 1 )
			realname += " ";
	}
	client.setRealname( realname );
	if ( client.getGavePassword() )
	{
		client.printInfo();

		std::cout << PrintTime::printTime() << BOLD << " --- socket " << client.getSocket() << " "
																							   "is now registered" << END << std::endl;

		message = RPL::RPL_WELCOME( client, server );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD  << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

		message = RPL::RPL_YOURHOST( client, server );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD  << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

		message = RPL::RPL_CREATED( client, server );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD  << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

		message = RPL::RPL_MYINFO( client, server );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD  << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;
	}

}

void Commands::PASS( std::vector< std::string > & command, int i, Server & server ) {

	Client & client = server.getConnections()[i];

//	std::cout << PrintTime::printTime() << GREEN << " --- Processing PASS command" << END << std::endl;

	if ( command.size() == 1 )
	{
		std::cout << PrintTime::printTime() << BOLD << " --- socket " << client.getSocket() << " gave empty password. Disconnecting" << END << std::endl;
		client.closeSocket();
		return ;
	}
	else if ( command[1] != server.getPassword() )
	{
		std::cout << PrintTime::printTime() << BOLD << " --- socket " << client.getSocket() << " gave invalid password. Disconnecting" << END << std::endl;
		client.closeSocket();
		return ;
	}
	else
	{
		std::cout << PrintTime::printTime() << BOLD << " --- socket " << client.getSocket() << " gave correct password" << END << std::endl;
		server.getConnections()[i].setGavePassword( true );
		return ;
	}
}


void Commands::WHOIS( std::vector< std::string > & command, int i, Server & server ) {

	Client & client = server.getConnections()[i];
	std::string message;

	std::string nickname = command[1];
	for ( int j = 0; j < MAX_CONNECTIONS; j++ )
	{
		if ( server.getConnections()[j].getSocket() >= 0 && server.getConnections()[j].getNickname() == nickname )
		{
			message = RPL::RPL_WHOISUSER( client, server );
			send( client.getSocket(), message.c_str(), message.size(), 0 );
			std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;
			return ;
		}
	}
	return ;
}

void Commands::PING( std::vector< std::string > & command, int i, Server & server ) {

	( void )command;
	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " PONG " + server.getServerName() + " :" + server.getServerName() +"\r\n";
	send( server.getConnections()[i].getSocket(), message.c_str(), message.size(), 0 );
	std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << server.getConnections()[i].getSocket() << "] " << message << END;

	return ;
}