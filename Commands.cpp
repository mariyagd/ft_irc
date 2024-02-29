#include "Commands.hpp"

// Coplien's form------------------------------------------------------------------------------------------------------

Commands::Commands( void ) {
	return ;
}

Commands::~Commands( void ) {
	return ;
}

//  Helpers ---------------------------------------------------------------------------------------------------------------------

void	Commands::splitMsgOnSpace( std::string & msg, std::vector< std::string > & tokens ) {

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
		if ( !token.empty() )
			tokens.push_back( token );
	}
	return ;
}

void Commands::splitMsgOnComma( std::string & msg, std::vector< std::string > & tokens ) {

	std::istringstream iss( msg );

	while ( !iss.eof() )
	{
		std::string token;
		getline(iss, token, ',' ); // split by comas
		if ( !token.empty() )
			tokens.push_back( token );
	}
}

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
	availableCommands.push_back( "JOIN" );
	availableCommands.push_back( "PRIVMSG" );
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



// Process command------------------------------------------------------------------------------------------------------

void	Commands::process_command( std::string & msg, Client & client, Server & server )
{
//	std::cout << Get::Time() << " --- Processing message" << std::endl;

	std::vector< std::string > tokens;
	std::vector< std::string > command;
	std::vector< std::string > availableCommands;

	initAvailableCommands( availableCommands );
	splitMsgOnSpace( msg, tokens );

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
	else if ( command[0] == "JOIN" )
	{
		JOIN( command, client, server );
	}
	else if ( command[0] == "MODE" )
	{
		MODE( command, client, server );
	}
	else if ( command[0] == "PRIVMSG" )
	{
		PRIVMSG( command, client, server );
	}
	else if ( command[0] == "KICK" )
	{
		KICK( command, client, server );
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

	// if ( nickname.find_first_of(":+-&") != std::string::npos
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
	else
	{
		std::cout << Get::Time() << " --- Nickname " << client.getNickname() << " changed to " << nickname << END << std::endl;
		client.setNickname( nickname );
		RPL::RPL_NICK( client, nickname );
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
//
//

void	Commands::JOIN( std::vector< std::string > & command, Client & client, Server & server ) {

	std::cout << Get::Time() << GREEN << " --- Processing JOIN command" << END << std::endl;

	for ( size_t i = 0; i < command.size(); i++ )
	{
		std::cout << RED_BG << "command[" << i << "] = [" << command[i] << "]" << END << std::endl;
	}


	(void)command;
	(void)client;
	(void)server;

	size_t i = 0;
	Channel * channel = NULL;
	
	std::vector< std::string > channels;
	splitMsgOnComma( command[1], channels );


	std::cout << Get::Time() << RED_BG << " --- Requested to JOIN this(these) channel(s)" << END << std::endl;
	for ( size_t i = 0; i < channels.size(); i++ )
	{
		std::cout << Get::Time() << RED_BG << " --- " << channels[i] << END << std::endl;
	}


	for ( ; i < channels.size(); i++ )
	{
		if ( !server.channelExists (channels[i]))
		{
			std::cout << Get::Time() << BOLD << " --- Channel doesn't exists. Create a new channel" << END << std::endl;
			channel = server.createChannel(channels[i]);
			// server.addOperator( client.getNicknameId() );
			server.addOperator( client.getSocket() );
			//print added operatorsocket
			int operatorSocket = server.getOperatorSocket();
			std::cout << "the operator socket is " << operatorSocket << std::endl;
			std::cout << Get::Time() << BOLD << " --- " << client.getNickname() << " created the channel " << channels[i] << END << std::endl;
			//print added operatorsocket
			std::cout << "the operator socket is " << operatorSocket << std::endl;

		}
		else
		{
			std::cout << Get::Time() << BOLD << " --- Channel already exists" << END << std::endl;
			channel = server.getChannel(channels[i]);
		}
		if ( !channel->isClientInChannel( &client ) )
		{
			std::cout << Get::Time() << BOLD << " --- Add " << client.getNickname() << " in channel " << channels[i] << END << std::endl;
			RPL::RPL_JOIN( client, channels[i]);
			RPL::RPL_NAMREPLY(client,channels[i]);
			RPL::RPL_ENDOFNAMES(client,channels[i]);
			channel->addClient( client );
			std::cout << Get::Time() << BOLD << " --- " << client.getNickname() << " joined the channel " << channels[i] << END << std::endl;
			
		}
	}
}

void	Commands::MODE( std::vector< std::string > & command, Client & client, Server & server ) {

	(void)command;
	(void)client;
	(void)server;
		

	std::cout << Get::Time() << GREEN << " --- Processing MODE command" << END << std::endl;
	
	
	if ( command.size() == 1 )
	{
		RPL::ERR_NEEDMOREPARAMS( client, "MODE" );
	}
	if ( command[1].find_first_of("&#+!") == 0 )
	{
		std::cout << Get::Time() << BOLD << " --- Requested Channel MODE " << END << std::endl;
		
		if ( !server.channelExists( command[1] ) )
		{
			RPL::ERR_NOSUCHCHANNEL( client, command[1] );
		}
		else
		{
			std::cout << Get::Time() << BOLD << " --- Channel exist" << END << std::endl;
			
			RPL::RPL_CHANNELMODEIS( client, command[1]);
			RPL::RPL_CREATIONTIME( client, command[1], server.getChannel( command[1] )->getCreationTime() );
		}
	}
	if ( command[1] == client.getNickname() && command.size() == 3)
	{
		std::cout << Get::Time() << BOLD << " --- Send User MODE " << END << std::endl;
		RPL::ERR_UMODEUNKNOWNFLAG( client );
	}
	// else is nickname

	return ;
}

void Commands::PRIVMSG(std::vector<std::string>& command, Client& client, Server& server) 
{

	std::cout << Get::Time() << GREEN << " --- Processing PRIVMSG command" << END << std::endl;
    if (command.size() < 3)
	{
        std::cerr << Get::Time() << RED << " Error: Insufficient parameters for PRIVMSG command" << END << std::endl;
        return;
    }

	std::vector< std::string > target;
	splitMsgOnComma( command[1], target );

	std::string message = "";
	
	for (size_t i = 2; i < command.size(); i++)
	{
		message += command[i];
		if ( ( i + 1 ) != command.size() )
			message += " ";
	}
	// if (message[0] == ':')
	// 	message = message.erase(0, 1);

	for (size_t i = 0; i < target.size(); ++i )
	{
		
		if (target[i].find_first_of("&#") == 0)
		{
			if ( server.channelExists( target[i] ) )
			{
				std::cout << Get::Time() << BOLD << " --- Sending message to channel " << target[i] << END << std::endl;
				Channel * channel = server.getChannel( target[i] );
				for ( size_t j = 0; j < channel->getAllClients().size(); j++ )
				{
					int receiverSocket = channel->getAllClients()[j]->getSocket();
					if ( receiverSocket != client.getSocket() )
					{
						RPL::RPL_PRIVMSG( client, target[i], message, receiverSocket );
					}
				}
				// RPL::RPL_PRIVMSG_CHANNEL( client, target[i], message );
			}
			else
			{
				std::cerr << Get::Time() << RED << " --- Error: Failed to send message to channel " << target[i] << END << std::endl;
				RPL::ERR_NOSUCHNICK( client, target[i] );

			}
		
			// RPL::RPL_PRIVMSG_CHANNEL( client, target[i], message );
			// 	std::cout << Get::Time() << BOLD << " --- Message sent to channel " << target[i] << END << std::endl;
			// }
			// else
			// {
			// 	std::cerr << Get::Time() << RED << " --- Error: Failed to send message to channel " << target[i] << END << std::endl;
			// }
		}
		else
		{
			std::cout << Get::Time() << BOLD << " --- Sending private message to " << target[i] << END << std::endl;
		
				int	receiverSocket = server.getSocketByNickname(target[i]);
				if (receiverSocket < 0)
					RPL::ERR_NOSUCHNICK( client, target[i] );
				else
				{				
					RPL::RPL_PRIVMSG( client, target[i], message, receiverSocket );
					std::cout << Get::Time() << BOLD << " --- Private message sent to " << target[i] << END << std::endl;
				}
		}
	}
}

//KICK #newchnl hkahsay_ :
// >> @time=2024-02-29T14:22:13.784Z :hkahsay__!~hkahsay@185.25.195.181 KICK #newchnl hkahsay_ :hkahsay_
void Commands::KICK( std::vector< std::string > & command, Client & client, Server &server )
{

	std::string channelName = command[1];
	std::vector< std::string > nickname;
	std::string comment = "";
	Channel *channel = nullptr;

	if ( !server.channelExists( channelName ) )
	{
		RPL::ERR_NOSUCHNICK( client, channelName );
		return;
	}

	channel = server.getChannel(channelName);
	if ( !channel->isClientInChannel( &client ) )
	{
		RPL::ERR_NOTONCHANNEL( client, channelName );
		return;
	}
	int operatorSocket = server.getOperatorSocket();
	//call int  ChannelMenager::getOperatorSocket()const in channel->isClientIsOperator()
	if ( channel->isClientIsOperator(operatorSocket) == -1 )
	{
		std::cout << "check that its not operator status for client ID: " << client.getSocket() << std::endl;
		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
		return;
	}
	std::cout << "the client with socket" << client.getSocket() << " is operator" << std::endl;
	splitMsgOnComma( command[2], nickname );

	for (size_t i = 3; i < command.size(); i++)
	{
		comment += command[i];
		if ( ( i + 1 ) != command.size() )
			comment += " ";
	}

	std::cout << Get::Time() << GREEN << " --- Processing KICK command" << END << std::endl;

	for (size_t i = 0; i < channel->getAllClients().size(); i++)
	{
		for ( size_t j = 0; j < nickname.size(); j++ )
		{
			if ( channel->getAllClients()[i]->getNickname() == nickname[j])
			{
				std::cout << "the client " << nickname[j] << " will be kicked from the channel" << std::endl;
				channel->removeClient(nickname[j]);
				RPL::RPL_KICK(client, channelName, nickname[j], comment );
				nickname.erase(nickname.begin() + j);
			}
		}
	}
	for ( size_t i = 0; i < nickname.size(); i++ )
	{
		RPL::ERR_USERNOTINCHANNEL( client, nickname[i], channelName );
	}
	return;
}
		
