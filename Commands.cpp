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
	availableCommands.push_back( "KICK" );

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
	else if ( command[0] == "KICK" )
	{
		std::cout << "Process KICK command" << std::endl;
		if (command.size() >= 3) { // Ensure enough parameters for KICK
			KICK(command[1], command[2], server);
		} else {
			std::cerr << "Insufficient parameters for KICK command" << std::endl;
		}
		
	}
	return ;
}


// NICK------------------------------------------------------------------------------------------------------------------

bool Commands::is_unique_nickname(  std::string & nickname, int clientSocket, std::vector< Client > & connections )
{
	for ( int i = 0; i < MAX_CONNECTIONS; ++i )
	{
		if ( connections[i].getSocket() >= 0 && connections[i].getSocket() != clientSocket && connections[i].getNickname() == nickname )
			return false;
	}
	return true;
}

void Commands::NICK( std::vector< std::string > & command, int j, Server &server ) {

//	std::cout << PrintTime::printTime() << GREEN << " --- Processing NICK command" << END << std::endl;

	std::string message ="";
	std::string nickname = command[1];
	Client & client = server.getConnections()[j];

	if ( client.getSocket() < 0 )
		return;

	if ( command.size() == 1 )
	{
		std::cout << PrintTime::printTime() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname <<  " no nickname given" << END << std::endl;

		message = RPL::ERR_NONICKNAMEGIVEN( server );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;
		return;
	}

	for ( size_t i = 0; i < nickname.length(); i++ )
	{
		if ( std::isspace(nickname[i]) || nickname[i] == '+' || nickname[i] == '-' || \
			 nickname[i] == ':' || nickname[i] == '!' || nickname[i] == '@' || nickname[i] == '#' || nickname[i] == '$' || \
			 nickname[i] == '%' || nickname[i] == '&' || nickname[i] == '*' || nickname[i] == ';' || nickname[i] == ',' || \
			 nickname[i] == '.' || nickname[i] == '/' || nickname[i] == '<' || nickname[i] == '>' || nickname[i] == '?' || \
			 nickname[i] == '=' || nickname[i] == '"' || nickname[i] == '\''  || nickname[i] == '~' )
		{
			std::cout << PrintTime::printTime() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname << " erroneus nickname" << END << std::endl;

			message = RPL::ERR_ERRONEUSNICKNAME( client, server );
			send( client.getSocket(), message.c_str(), message.size(), 0 );
			std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

			client.getNickname().clear();

			return ;
		}
	}

	if ( !is_unique_nickname( nickname, client.getSocket(), server.getConnections() ) )
	{
		std::cout << PrintTime::printTime() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname << " nickname is use" << END << std::endl;

		message = RPL::ERR_NICKNAMEINUSE( client, server, nickname );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;
		return;
	}
	else if ( !client.isRegistered() )
	{
		client.setNickname( nickname );
		return;
	}

	message = RPL::RPL_SAVENICK( client, server );
	send( client.getSocket(), message.c_str(), message.size(), 0 );
	std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

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

	if ( client.getSocket() < 0 )
		return;

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
}

// PASS------------------------------------------------------------------------------------------------------------------

void Commands::PASS( std::vector< std::string > & command, int i, Server & server ) {

	Client & client = server.getConnections()[i];
	std::string message;

//	std::cout << PrintTime::printTime() << GREEN << " --- Processing PASS command" << END << std::endl;

	if ( command.size() == 1 )
	{
		std::cout << PrintTime::printTime() << BOLD << " --- socket " << client.getSocket() << " gave empty password. Disconnecting" << END << std::endl;

		message = RPL::ERR_NEEDMOREPARAMS( client, server, "PASS" );
		send( client.getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

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

// WHOIS-----------------------------------------------------------------------------------------------------------------
void Commands::WHOIS( std::vector< std::string > & command, int i, Server & server ) {

	Client & client = server.getConnections()[i];
	std::string message;

	if ( client.getSocket() < 0 )
		return;

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
	std::string message = "";

	if ( server.getConnections()[i].getSocket() < 0 )
		return;

	if ( command.size() == 1 )
	{
		message = RPL::ERR_NOORIGIN( server );
		send( server.getConnections()[i].getSocket(), message.c_str(), message.size(), 0 );
		std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << server.getConnections()[i].getSocket() << "] " << message << END;
	}
	message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " PONG " + server.getServerName() + " :" + server.getServerName() +"\r\n";
	send( server.getConnections()[i].getSocket(), message.c_str(), message.size(), 0 );
	std::cout << PrintTime::printTime() << YELLOW_BOLD << " --- Send msg to [socket " << server.getConnections()[i].getSocket() << "] " << message << END;

	return ;
}


void Commands::KICK( std::string &channel, std::string &nickname, Server &server)
{
    // Check if the channel exists and the user is available in the channel
    Channel* ch = server.getChannel(channel);
    if (ch && ch->isClientInChannel(nickname)) {
		std::cout<<"the client "<<nickname<< " is join to channel."<<std::endl;
        // Check if the user has operator status in the channel
        // if (ch->isOperator(nickname)) {
        //     // Remove the user from the channel
        //     ch->removeUser(nickname);
        //     // Send message to all users in the channel about the kick
        //     std::string kickMessage = "KICK " + channel + " " + nickname;
        //     server.sendToChannel(kickMessage, channel);
        // } else {
        //     // User does not have operator status
        //     // Send ERR_CHANOPRIVSNEEDED message
        //     server.sendErrorToUser(nickname, "ERR_CHANOPRIVSNEEDED", "You do not have operator status in the channel");
        // }
    } 
	else {
			std::cout<<"the client "<<nickname<< " is not joined to channel."<<std::endl;

        // Either channel doesn't exist or user is not available in the channel
        // Send appropriate error messages
        // if (!ch) {
        //     // Channel does not exist
        //     server.sendErrorToUser(nickname, "ERR_NOSUCHCHANNEL", "No such channel");
        // } else {
        //     // User is not available in the channel
        //     server.sendErrorToUser(nickname, "ERR_USERNOTINCHANNEL", "You are not in that channel");
        }
    }

