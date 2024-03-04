#include "Commands.hpp"

std::map< std::string, ACommand * > availableCommands;

// Coplien's form------------------------------------------------------------------------------------------------------

Commands::Commands( void ) {


	return ;
}

Commands::~Commands( void ) {
	return ;
}

//  Helpers ---------------------------------------------------------------------------------------------------------------------

void Commands::concatenate( std::vector< std::string > & tokens, size_t i, std::string & msg ) {

	for ( ; i < tokens.size(); i++ )
	{
		msg += tokens[i];
		if ( ( i + 1 ) != tokens.size() )
			msg += " ";
	}
}

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

//bool	Commands::isCommand( const std::string &token/*, std::vector< std::string > & availableCommands */) {
//
//	std::map< std::string, void ( * )( std::vector< std::string > & , Client &, Server & ) >::iterator it = Commands::_commands.find( token );
//	if ( it != _commands.end() )
//		return true;
//	return false;
//}

void	Commands::initAvailableCommands( ) {


	availableCommands.insert( std::make_pair ("PASS", new PASS ) );
//	availableCommands.insert( std::make_pair ("NICK", new NICK ) );
//	availableCommands.insert( std::make_pair ("USER", new USER ) );
	return ;
}


// Process command------------------------------------------------------------------------------------------------------

void	Commands::process_command( std::string & msg, Client & client, Server & server )
{
//	std::cout << Get::Time() << " --- Processing message" << std::endl;

	std::istringstream iss(msg);
	initAvailableCommands();

	while ( !iss.eof() )
	{
		std::vector< std::string > command;
		std::vector< std::string > tokens;
		std::string line;

		getline(iss, line, '\n' ); // split by new line
//		splitMsgOnSpace( line, tokens );

		for ( size_t i = 0; i < tokens.size( ); i++ )
		{
			std::map< std::string, ACommand * >::iterator it = availableCommands.find( line );
			if ( it != availableCommands.end() )
				it->second->execute( line, client, server );
		}
	}
	return ;

}
//
//void Commands::choose_command( std::vector <std::string> & command, Client & client, Server & server ) {
////
////	if ( command[0] == "PASS" )
////	{
////		PASS( command, client, server );
////	}
////	else if ( command[0] == "NICK" )
////	{
////		NICK( command, client, server );
////	}
////	else if ( command[0] == "USER" )
////	{
////		USER( command, client, server );
////	}
////	else if ( command[0] == "WHOIS" )
////	{
////		WHOIS( command, client, server );
////	}
////	else if ( command[0] == "PING" )
////	{
////		PING( command, client, server );
////	}
////	else if ( command[0] == "JOIN" )
////	{
////		JOIN( command, client, server );
////	}
////	else if ( command[0] == "MODE" )
////	{
////		MODE( command, client, server );
////	}
////	else if ( command[0] == "PRIVMSG" )
////	{
////		PRIVMSG( command, client, server );
////	}
////	else if ( command[0] == "KICK" )
////	{
////		KICK( command, client, server );
////	}
////	else if ( command[0] == "INVITE" )
////	{
////		INVITE( command, client, server );
////	}
////	else if ( command[0] == "TOPIC" )
////	{
////		TOPIC( command, client, server );
////	}
////	else if ( command[0] == "PART" )
////	{
////		PART( command, client, server );
////	}
////	else if ( command[0] == "WHO" )
////	{
////		WHO( command, client, server );
////	}
//	static std::map< std::string, void ( * )( std::vector< std::string > & , Client &, Server & ) >::iterator it = Commands::_commands.find( command[0] );
//	if ( it != Commands::_commands.end() )
//		it->second( command, client, server );
//
//
//	return ;
//}

// PASS------------------------------------------------------------------------------------------------------------------
//
//void Commands::PASS( std::vector< std::string > & command, Client & client, Server & server ) {
//
////	std::cout << Get::Time() << GREEN << " --- Processing PASS command" << END << std::endl;
//
//	if ( command.size() == 1 )
//	{
//		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " gave empty password. Disconnecting" << END << std::endl;
//
//		RPL::ERR_NEEDMOREPARAMS( client, "PASS" );
//		client.closeSocket();
//	}
//	else if ( command[1] != server.getPassword() )
//	{
//		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " gave invalid password. Disconnecting" << END << std::endl;
//		client.closeSocket();
//	}
//	else
//	{
//		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " gave correct password" << END << std::endl;
//		client.setGavePassword( true );
//	}
//	return;
//}
//
//// NICK------------------------------------------------------------------------------------------------------------------
//
//bool Commands::is_unique_nickname(  std::string & nickname, int clientSocket, std::vector< Client > & connections )
//{
//	for ( int i = 0; i < MAX_CONNECTIONS; ++i )
//	{
//		if ( connections[i].getSocket() >= 0 && connections[i].getSocket() != clientSocket && connections[i].getNickname() == nickname )
//			return false;
//	}
//	return true;
//}
//
//void Commands::NICK( std::vector< std::string > & command, Client & client, Server &server ) {
//
////	std::cout << Get::Time() << GREEN << " --- Processing NICK command" << END << std::endl;
//
//	std::string nickname = command[1];
//
//	if ( client.getSocket() < 0 )
//		return;
//
//	if ( command.size() == 1 )
//	{
//		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname <<  " no nickname given" << END << std::endl;
//
//		RPL::ERR_NONICKNAMEGIVEN( client );
//		return;
//	}
//
//	for ( size_t i = 0; i < nickname.length(); i++ )
//	{
//		if ( nickname.find_first_of(" :+-&?!@#$%*;,./<>=\"\\~") != std::string::npos )
//		{
//			std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname << " erroneous nickname" << END << std::endl;
//
//			RPL::ERR_ERRONEUSNICKNAME( client );
//			return ;
//		}
//	}
//
//	if ( !is_unique_nickname( nickname, client.getSocket(), server.getConnections() ) )
//	{
//		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname << " nickname is use" << END << std::endl;
//		RPL::ERR_NICKNAMEINUSE( client, nickname );
//	}
//	else if ( !client.isRegistered() )
//	{
//		std::cout << Get::Time() << " --- User [socket " << client.getSocket() << "] can register with name [" << nickname << "]" << END << std::endl;
//		client.setNickname( nickname );
//		RPL::RPL_NICK( client, nickname );
//	}
//	else
//	{
//		std::cout << Get::Time() << " --- Nickname " << client.getNickname() << " changed to " << nickname << END << std::endl;
//		RPL::RPL_NICK( client, nickname );
//		client.setNickname( nickname );
//	}
//	client.printInfo();
//	return ;
//}
//
//// USER <username> <hostname> <servername> <realname>--------------------------------------------------------------------
//
//void Commands::USER( std::vector <std::string> & command, Client & client, Server & server ) {
//
////	std::cout << Get::Time() << GREEN << " --- Processing USER command" << END << std::endl;
//
//	(void )server;
//
//	client.setUsername( "~" + command[1] );
////	client.setHostname( command[2] );
////	client.setServname( command[3] );
//
//	std::string realname;
//	for ( size_t i = 4; i < command.size(); i++ )
//	{
//		realname += command[i];
//		if ( i != command.size() - 1 )
//			realname += " ";
//	}
//	client.setRealname( realname );
//}
//
//
//
//// WHOIS-----------------------------------------------------------------------------------------------------------------
//void Commands::WHOIS( std::vector< std::string > & command, Client & client, Server & server ) {
//
//	std::vector< Client > & connections = server.getConnections();
//
//	std::string nickname = command[1];
//	for ( int i = 0; i < MAX_CONNECTIONS; i++ )
//	{
//		if ( connections[i].getSocket() >= 0 && connections[i].isRegistered() && connections[i].getNickname() == nickname )
//		{
//			RPL::RPL_WHOISUSER( client );
//			RPL::RPL_ENDOFWHOIS( client );
//			return ;
//		}
//	}
//	return ;
//}
//
//void Commands::WHO( std::vector< std::string > & command, Client & client, Server & server ) {
//
//	if (command.size() != 2 )
//		return;
//
//	Channel * channel = server.getChannelByName( command[1] );
//	if (!channel)
//	{
//		RPL::ERR_NOSUCHCHANNEL( client, command[1] );
//		return;
//	}
//	RPL::RPL_ENDOFWHO( client, command[1] );
//	return ;
//}
//
//void Commands::PING( std::vector< std::string > & command, Client & client, Server & server ) {
//
//	(void)server;
//
//	if ( command.size() == 1 )
//		RPL::ERR_NOORIGIN( client );
//	else
//		RPL::RPL_PING( client );
//	return ;
//}
//
//void	Commands::JOIN( std::vector< std::string > & command, Client & client, Server & server ) {
//
//	size_t i = 0;
//	Channel * channel = NULL;
//
//	std::vector< std::string > channelsNames;
//	splitMsgOnComma( command[1], channelsNames );
//
//	for ( ; i < channelsNames.size(); i++ )
//	{
//		channel = server.getChannelByName(channelsNames[i]);
//		if ( !channel )
//		{
//			std::cout << Get::Time() << BOLD << " --- Channel doesn't exists. Create a new channel" << END << std::endl;
//			channel = server.createChannel(channelsNames[i]);
//			channel->addOperator( client.getNicknameId() );
//			std::cout << Get::Time() << BOLD << " --- " << client.getNickname() << " created the channel " << channelsNames[i] << END << std::endl;
//		}
//		if ( !channel->clientIsInChannel( &client ) )
//		{
//			std::cout << Get::Time() << BOLD << " --- Add " << client.getNickname() << " in channel " << channelsNames[i] << END << std::endl;
//			if ( channel->getInviteMode() && !channel->isClientInvited( client.getNicknameId() ) )
//				RPL::ERR_INVITEONLYCHAN( client, channelsNames[i] );
//			else if ( channel->getLimitMode() && channel->getAllClients().size() >= static_cast< size_t >( channel->getLimit() ) )
//				RPL::ERR_CHANNELISFULL( client, channelsNames[i] );
//			else
//			{
//				if ( channel->getKeyMode() && (command.size() != 3 || command[2] != channel->getKey()) )
//				{
//					RPL::RPL_BADCHANNELKEY( client, channelsNames[i] );
//					return;
//				}
//				RPL::RPL_JOIN( client, channel->getAllClients(), channelsNames[i] );
//				channel->addClient( client );
//
//				if ( channel->isClientIsOperator( client.getNicknameId( ) ) != -1 )
//				{
//					RPL::RPL_MODE_OP( client, channelsNames[i], channel->getCurrentChannelModes( ) );
//				}
//				if ( channel->isClientInvited( client.getNicknameId() ) )
//					channel->removeInvited( client.getNicknameId() );
//				if ( !channel->getTopic().empty() )
//				{
//					RPL::RPL_TOPIC( client, channelsNames[i], channel->getTopic( ) );
//					RPL::RPL_TOPICWHOTIME( client, *channel->getTopicSetter(), channelsNames[i], channel->getTopicCreationTime( ) );
//				}
//				RPL::RPL_NAMREPLY( client, channelsNames[i], channel->getOperatorsName( ) );
//				RPL::RPL_ENDOFNAMES( client, channelsNames[i] );
//				std::cout << Get::Time( ) << BOLD << " --- " << client.getNickname( ) << " joined the channel " << channelsNames[i] << END << std::endl;
//			}
//		}
//	}
//}
//
//bool mode_already_set( Channel * channel, char mode, bool status )
//{
//	if ( mode == 't' && channel->getTopicMode() == status )
//		return true;
//	else if ( mode == 'l' && channel->getLimitMode() == status )
//		return true;
//	else if ( mode == 'i' && channel->getInviteMode() == status )
//		return true;
//	else if ( mode == 'k' && channel->getKeyMode() == status )
//		return true;
//	return false;
//}
//
//void Commands::setChannelMode( Client & client, std::vector< std::string > & command, Channel * channel ) {
//
//	bool status;
//	std::string mode = command[0];
//	size_t i = 0;
//	size_t j = 1;
//
//	 for ( ; i < mode.size() ; ++i)
//	 {
//		 if ( mode[i] == '+' )
//			 status = true;
//		 else if ( mode[i] == '-' )
//			 status = false;
//		 else
//		 {
//			 switch ( mode[i] )
//			 {
//				 case 't':
//				 {
//					 if ( mode_already_set(channel, 't', status) )
//						 command[0].erase( i, 1 );
//					 else
//					 	channel->setTopicMode( status );
//					 break;
//				 }
//				 case 'l':
//				 {
//					 if ( status && std::atoi( command[j].c_str() ) > 0 )
//						 channel->setLimitMode( status, std::stoi( command[j++] ) );
//					 else if ( !status && !mode_already_set( channel, 'l', status ) )
//						 channel->setLimitMode( status, 0 );
//					 else
//					 {
//						 command.erase( command.begin() + j );
//						 command[0].erase( i, 1 );
//					 }
//					 break;
//				 }
//				 case 'i':
//				 {
//					 if ( mode_already_set(channel, 'i', status) )
//						 command[0].erase( i, 1 );
//					 else
//					 	channel->setInviteMode( status );
//					 break;
//				 }
//				 case 'k':
//				 {
//					 if ( status )
//						 channel->setKeyMode( status, command[j++] );
//					 else if ( !status && mode_already_set( channel, 'k', status ) )
//						 command[0].erase( i, 1 );
//					 else
//					 {
//						 channel->setKeyMode( status, "" );
//						 command[j++] = "*";
//					 }
//					 break;
//				 }
//				 case 'o':
//				 {
//					 Client * other = channel->getClientByNickname( command[j] );
//					 if ( other )
//					 {
//						 if ( status && channel->isClientIsOperator( other->getNicknameId() ) == -1 )
//							 channel->addOperator( other->getNicknameId( ) );
//						 else if ( !status )
//							 channel->removeOperator( other->getNicknameId( ) );
//						 ++j;
//					 }
//					 else
//					 {
//						 RPL::ERR_NOSUCHNICK( client, command[j] );
//						 command.erase( command.begin( ) + j );
//						 command[0].erase( i, 1 );
//					 }
//					 break;
//				 }
//				 default:
//				 {
//					 RPL::ERR_UNKNOWNMODE( client, mode[i] );
//					 break;
//				 }
//			 }
//		 }
//	 }
//	if (command[0].find_first_not_of("+-") == std::string::npos )
//	 command[0].clear();
//}
//
//bool Commands::errorsInModeCommand( std::vector< std::string > & _command, Client & client, Channel * channel )
//{
//	(void)channel;
//	bool status;
//	std::vector< std::string > command = _command;
//
//	if ( channel->isClientIsOperator(client.getNicknameId()) == -1 )
//	{
//		RPL::ERR_CHANOPRIVSNEEDED( client, channel->getChannelName() );
//		return true;
//	}
//	if ( command[0].find_first_not_of("ltiko+-" ) != std::string::npos )
//	{
//		RPL::ERR_UNKNOWNMODE(client, command[0][command[0].find_first_not_of("ltiko")]);
//		return true;
//	}
//	for ( size_t i = 0; i < command[0].size(); ++i )
//	{
//		if (command[0][i] == '+')
//			status = true;
//		else if (command[0][i] == '-')
//			status = false;
//		else
//		{
//			if ( ( command[0][i] == 'l' || command[0][i] == 'o' || command[0][i] == 'k') && status )
//			{
//				if (command.size() <= 1 )
//				{
//					RPL::ERR_NEEDMOREPARAMS( client, "MODE" );
//					return true;
//				}
//				else
//					command.erase( command.begin() + 1 );
//			}
//		}
//	}
//	return false;
//}
//
//void	Commands::MODE( std::vector< std::string > & command, Client & client, Server & server ) {
//
//	( void ) command;
//	( void ) client;
//	( void ) server;
//
//
////	std::cout << Get::Time( ) << GREEN << " --- Processing MODE command" << END << std::endl;
//
//	if ( command.size( ) == 1 )
//	{
//		RPL::ERR_NEEDMOREPARAMS( client, "MODE" );
//	}
//	else if ( command[1].find_first_of( "&#+!" ) == 0 )  // if channel mode is requested
//	{
////		std::cout << Get::Time( ) << GREEN << " --- Processing MODE command for channels" << END << std::endl;
//
//		Channel * channel = server.getChannelByName( command[1] );
//		if ( !channel )
//			RPL::ERR_NOSUCHCHANNEL( client, command[1] );
//		else if ( command.size( ) == 2 )
//		{
//			RPL::RPL_CHANNELMODEIS( client, command[1], channel->getCurrentChannelModes( ) );
//			RPL::RPL_CREATIONTIME( client, command[1], server.getChannelByName( command[1] )->getCreationTime( ) );
////			std::cout << Get::Time( ) << BOLD << " --- Send channel mode " << END << std::endl;
//			return;
//		}
//		else
//		{
//			command.erase( command.begin( ), command.begin( ) + 2 );
//			if ( errorsInModeCommand( command, client, channel ) )
//				return ;
//			else
//			{
//				setChannelMode( client, command, channel );
//				if (!command[0].empty())
//					RPL::INFORM_CHANNELMODE( client, channel->getChannelName(), command, channel->getAllClients() );
//			}
//		}
//	}
//	else if ( command[1] == client.getNickname() && command.size() == 3)
//	{
////		std::cout << Get::Time() << BOLD << " --- Send User MODE " << END << std::endl;
//		RPL::ERR_UMODEUNKNOWNFLAG( client );
//	}
//	return ;
//}
//
//void Commands::PRIVMSG(std::vector<std::string>& command, Client& client, Server& server)
//{
//
//	std::cout << Get::Time() << GREEN << " --- Processing PRIVMSG command" << END << std::endl;
//
//	std::vector< std::string > target;
//	std::string message = "";
//
//	splitMsgOnComma( command[1], target );
//	concatenate(command, 2, message );
//
//	for (size_t i = 0; i < target.size(); ++i )
//	{
//
//		if (target[i].find_first_of("&#+!") == 0) // if the destination is a channel
//		{
//			Channel * channel = server.getChannelByName( target[i] );
//			if (! channel )
//			{
//				std::cerr << Get::Time() << RED << " --- Error: Failed to send message to channel. Channel doesnt't exist " << target[i] << END << std::endl;
//				RPL::ERR_NOSUCHNICK( client, target[i] );
//
//			}
//			else if ( channel->clientIsInChannel( &client ) )
//			{
//				std::cout << Get::Time() << BOLD << " --- Sending message to all clients in channel " << target[i] << END << std::endl;
//				for ( size_t j = 0; j < channel->getAllClients().size(); j++ )
//				{
//					if ( channel->getAllClients()[j]->getSocket() != client.getSocket() )
//						RPL::RPL_PRIVMSG( client, target[i], message, channel->getAllClients()[j]->getSocket() );
//				}
//			}
//			else
//			{
//				std::cerr << Get::Time() << RED << " --- Error: Failed to send message to channel. Cannot send to channel " << target[i] << END << std::endl;
//				RPL::ERR_CANNOTSENDTOCHAN( client, target[i] );
//			}
//		}
//		else
//		{
//			std::cout << Get::Time() << BOLD << " --- Sending private message to " << target[i] << END << std::endl;
//
//				int	receiverSocket = server.getSocketByNickname(target[i]);
//				if (receiverSocket < 0)
//					RPL::ERR_NOSUCHNICK( client, target[i] );
//				else
//				{
//					RPL::RPL_PRIVMSG( client, target[i], message, receiverSocket );
//					std::cout << Get::Time() << BOLD << " --- Private message sent to " << target[i] << END << std::endl;
//				}
//		}
//	}
//}
//
////KICK #newchnl hkahsay_ :
//// >> @time=2024-02-29T14:22:13.784Z :hkahsay__!~hkahsay@185.25.195.181 KICK #newchnl hkahsay_ :hkahsay_
//void Commands::KICK( std::vector< std::string > & command, Client & client, Server &server )
//{
//	std::cout << Get::Time() << GREEN << " --- Processing KICK command" << END << std::endl;
//
//	for ( size_t i = 0; i < command.size(); i++ )
//	{
//		std::cout << Get::Time() << CYAN_BG << " --- Command: [" << command[i] << "]" << END << std::endl;
//	}
//
//	std::string & channelName = command[1];
//	std::string & nickname = command[2];
//	std::string comment = "";
//
//	Channel *channel = nullptr;
//	Client *kicked = nullptr;
//
//	kicked = server.getClientByNickname( nickname );
//	channel = server.getChannelByName( channelName );
//	if ( !channel )
//		RPL::ERR_NOSUCHCHANNEL( client, channelName );
//	else if ( !kicked )
//		RPL::ERR_NOSUCHNICK( client, nickname );
//	else if ( !channel->clientIsInChannel( kicked ) )
//		RPL::ERR_USERNOTINCHANNEL( client, nickname, channelName );
//	else if ( !channel->clientIsInChannel( &client ) )
//		RPL::ERR_NOTONCHANNEL( client, channelName );
//	else if ( channel->isClientIsOperator(client.getNicknameId()) == -1 )
//	{
//		std::cout << Get::Time() << RED << " --- Client " << client.getNickname() << " [socket "<< client.getSocket() << "] is NOT operator" << END << std::endl;
//		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
//	}
//	else
//	{
//		std::cout << Get::Time( ) << GREEN << " --- Client " << client.getNickname( ) << " [socket "
//				  << client.getSocket( ) << "] is operator" << END << std::endl;
//
//		for ( size_t i = 3; i < command.size( ); i++ )
//		{
//			comment += command[i];
//			if ( ( i + 1 ) != command.size( ) )
//				comment += " ";
//		}
//		if ( comment[0] == ':' )
//			comment.erase( 0, 1 );
//
//		for ( size_t i = 0; i < channel->getAllClients( ).size( ); i++ )
//		{
//			if ( channel->getAllClients( )[i]->getNickname( ) == nickname )
//			{
//				std::cout << Get::Time( ) << BOLD << " --- Client " << nickname << " is kicked from the channel" << END << std::endl;
//				RPL::RPL_KICK( client, channelName, nickname, comment, channel->getAllClients( ) );
//				channel->removeClient( nickname );
//			}
//		}
//	}
//	return;
//}
//
//// INVITE
//
//void 	Commands::INVITE( std::vector< std::string > & command, Client & client, Server & server ) {
//
//	std::string & nickname = command[1];
//	std::string & channelName = command[2];
//	Channel *channel = nullptr;
//	Client * invited = nullptr;
//
//	std::cout << Get::Time() << GREEN << " --- Processing INVITE command" << END << std::endl;
//
//	for ( size_t i = 0; i < command.size(); i++ )
//	{
//		std::cout << Get::Time() << CYAN_BG << " --- Command: [" << command[i] << "]" << END << std::endl;
//	}
//
//	invited = server.getClientByNickname( nickname );
//	channel = server.getChannelByName( channelName );
//	if ( !invited )
//	{
//		std::cout << Get::Time() << RED_BOLD << " --- No such nick " << nickname << " in the network" << END << std::endl;
//		RPL::ERR_NOSUCHNICK( client, nickname );
//	}
//	else if ( !channel )
//	{
//		std::cout << Get::Time() << RED_BOLD << " --- No such channel " << channelName << " in the network" << END << std::endl;
//		RPL::ERR_NOSUCHCHANNEL( client, channelName );
//	}
//	else if ( channel->clientIsInChannel( invited ) )
//	{
//		std::cout << Get::Time() << RED_BOLD << " --- User " << nickname << " is already in channel " << channelName << END << std::endl;
//		RPL::ERR_USERONCHANNEL( client, nickname, channelName );
//	}
//	else if ( channel->isClientIsOperator( client.getNicknameId() ) == -1 )
//	{
//		std::cout << Get::Time() << RED_BOLD << " --- You are not an operator" << END << std::endl;
//		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
//	}
//	else
//	{
//		std::cout << Get::Time() << GREEN_BOLD << " --- Inviting user " << nickname << " in channel " << channelName << END << std::endl;
//		RPL::RPL_INVITING( client, *invited, channelName );
//		channel->addInvited( invited->getNicknameId() );
//	}
//}
//
//
//void Commands::TOPIC( std::vector< std::string > & command, Client & client, Server & server ) {
//
//	std::string & channelName = command[1];
//	std::string topic;
//	concatenate(command, 2, topic );
//	Channel *channel = nullptr;
//
//	std::cout << Get::Time() << GREEN << " --- Processing TOPIC command" << END << std::endl;
//
//	channel = server.getChannelByName( channelName );
//	if ( !channel )
//	{
//		std::cout << Get::Time() << RED_BOLD << " --- No such channel " << channelName << " in the network" << END << std::endl;
//		RPL::ERR_NOSUCHCHANNEL( client, channelName );
//	}
//	else if ( !channel->clientIsInChannel( &client ) )
//	{
//		std::cout << Get::Time() << RED_BOLD << " --- You are not in channel " << channelName << END << std::endl;
//		RPL::ERR_NOTONCHANNEL( client, channelName );
//	}
//	else if ( command.size() == 2 )
//	{
//		if (channel->getTopic().empty())
//		{
//			std::cout << Get::Time() << " --- no channel topic " << std::endl;
//			RPL::RPL_NOTOPIC(client, channelName);
//		}
//		else
//		{
//			std::cout << Get::Time() << BOLD << " --- Send channel topic " << END << std::endl;
//			RPL::RPL_TOPIC(client, channelName, channel->getTopic());
//			RPL::RPL_TOPICWHOTIME(client, *channel->getTopicSetter(), channelName, channel->getTopicCreationTime());
//		}
//	}
//	else if ( !channel->getTopicMode() || channel->isClientIsOperator( client.getNicknameId() ) != -1 )
//	{
//		std::cout << Get::Time() << BOLD << " --- Set channel topic hrhr" << END << std::endl;
//		channel->setTopic( topic, client.getNicknameId() );
//		RPL::RPL_NORMAL( client, channel->getAllClients(), channelName, "TOPIC", topic );
//		RPL::RPL_TOPIC( client, channelName, topic );
//	}
//	else
//	{
//		std::cout << Get::Time() << RED_BOLD << " --- You are not an operator" << END << std::endl;
//		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
//	}
//}
//
//void Commands::PART( std::vector< std::string > & command, Client & client, Server & server ) {
//
//	std::cout << Get::Time() << GREEN << " --- Processing PART command" << END << std::endl;
//
//	std::vector< std::string > channelsName;
//	std::string comment;
//	Channel *channel = nullptr;
//
//	splitMsgOnComma( command[1], channelsName );
//	if ( command.size() > 2 )
//		concatenate(command, 2, comment);
//
//	for ( size_t i = 0; i < channelsName.size(); i++ )
//	{
//		channel = server.getChannelByName( channelsName[i] );
//		if ( !channel )
//		{
//			std::cout << Get::Time() << RED_BOLD << " --- No such channel " << channelsName[i] << " in the network" << END << std::endl;
//			RPL::ERR_NOSUCHCHANNEL( client, channelsName[i] );
//		}
//		else if ( !channel->clientIsInChannel( &client ) )
//		{
//			std::cout << Get::Time() << RED_BOLD << " --- You are not in channel " << channelsName[i] << END << std::endl;
//			RPL::ERR_NOTONCHANNEL( client, channelsName[i] );
//		}
//		else
//		{
//			std::cout << Get::Time() << BOLD << " --- Client " << client.getNickname() << " is leaving channel " << channelsName[i] << END << std::endl;
//			RPL::RPL_PART( client, channel->getAllClients(), channelsName[i], comment );
//			channel->removeClient( client.getNickname() );
//		}
//	}
//}