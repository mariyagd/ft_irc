#include "Commands.hpp"

// Coplien's form------------------------------------------------------------------------------------------------------

Commands::Commands( void ) {
	return ;
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

void	Commands::process_command( std::string & msg, Client & client, Server & server )
{
//	std::cout << Get::Time() << " --- Processing message" << std::endl;

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

	for ( size_t i = 0; i < tokens.size(); i++ )
	{
		if ( isCommand( tokens[i], availableCommands ) )
		{
			deleteCommandFromList( tokens[i], availableCommands );
			command.push_back( tokens[i] );
			for ( ++i; i < tokens.size() && !isCommand( tokens[i], availableCommands ); i++ )
			{
				command.push_back( tokens[i] );
			}
			--i;
			choose_command( command, client, server );
			command.clear();
		}
	}
	return ;
}

void Commands::choose_command( std::vector <std::string> & command, Client & client, Server & server ) {

	if ( command[0] == "PASS" )
	{
		PASS( command, client, server );
	}
	else if ( command[0] == "NICK" )
	{
		NICK( command, client, server );
	}
	else if ( command[0] == "USER" )
	{
		USER( command, client, server );
	}
	else if ( command[0] == "WHOIS" )
	{
		WHOIS( command, client, server );
	}
	else if ( command[0] == "PING" )
	{
		PING( command, client, server );
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

// PASS------------------------------------------------------------------------------------------------------------------

void Commands::PASS( std::vector< std::string > & command, Client & client, Server & server ) {

//	std::cout << Get::Time() << GREEN << " --- Processing PASS command" << END << std::endl;

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

void Commands::NICK( std::vector< std::string > & command, Client & client, Server &server ) {

//	std::cout << Get::Time() << GREEN << " --- Processing NICK command" << END << std::endl;

	std::string nickname = command[1];

	if ( client.getSocket() < 0 )
		return;

	if ( command.size() == 1 )
	{
		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname <<  " no nickname given" << END << std::endl;

		RPL::ERR_NONICKNAMEGIVEN( client );
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
			std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname << " erroneous nickname" << END << std::endl;

			RPL::ERR_ERRONEUSNICKNAME( client );
			return ;
		}
	}

	if ( !is_unique_nickname( nickname, client.getSocket(), server.getConnections() ) )
	{
		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname << " nickname is use" << END << std::endl;
		RPL::ERR_NICKNAMEINUSE( client, nickname );
	}
	else if ( !client.isRegistered() )
	{
		client.setNickname( nickname );
	}
	else
	{
		RPL::RPL_NICK( client, nickname );
		std::cout << Get::Time() << " --- Nickname " << client.getNickname() << " changed to " << nickname << END << std::endl;
		client.setNickname( nickname );
	}
	return ;
}

// USER <username> <hostname> <servername> <realname>--------------------------------------------------------------------

void Commands::USER( std::vector <std::string> & command, Client & client, Server & server ) {

//	std::cout << Get::Time() << GREEN << " --- Processing USER command" << END << std::endl;

	(void )server;

	client.setUsername( command[1] );
	client.setHostname( command[2] );
	client.setServname( command[3] );

	std::string realname;
	for ( size_t i = 4; i < command.size(); i++ )
	{
		realname += command[i];
		if ( i != command.size() - 1 )
			realname += " ";
	}
	client.setRealname( realname );
}



// WHOIS-----------------------------------------------------------------------------------------------------------------
void Commands::WHOIS( std::vector< std::string > & command, Client & client, Server & server ) {

	std::vector< Client > & connections = server.getConnections();

	std::string nickname = command[1];
	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
	{
		if ( connections[i].getSocket() >= 0 && connections[i].isRegistered() && connections[i].getNickname() == nickname )
		{
			RPL::RPL_WHOISUSER( client );
			return ;
		}
	}
	return ;
}

void Commands::PING( std::vector< std::string > & command, Client & client, Server & server ) {

	(void)server;
	( void )command;
	std::string message = "";

	if ( command.size() == 1 )
	{
		RPL::ERR_NOORIGIN( client );
	}
	message = "@time=" + Get::Time() + ":" + client.getServname() + " PONG " + client.getServname() + " :" + client.getServname() +"\r\n";
	send( client.getSocket(), message.c_str(), message.size(), 0 );
	std::cout << Get::Time() << YELLOW_BOLD << " --- Send msg to [socket " << client.getSocket() << "] " << message << END;

	return ;
}


void Commands::KICK( std::string &channel, std::string &nickname, Server &server)
{
    // Check if the channel exists and the user is available in the channel
    Channel* ch = server.getChannel(channel);
    if (ch && ch->isClientInChannel(nickname)) {
		std::cout<<"the client "<<nickname<< " is join to channel."<<std::endl;
        // Check if the user has operator status in the channel
        if (ch->isClientIsOperator(nickname)) {
            // Remove the user from the channel
            std::cout<<"found operator"<<std::endl;
            ch->removeUser(nickname);
            // Send message to all users in the channel about the kick
            std::string kickMessage = "KICK " + channel + " " + nickname;
            server.sendToChannel(kickMessage, channel);
        }
		//  else {
        // //     // User does not have operator status
        // //     // Send ERR_CHANOPRIVSNEEDED message
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

