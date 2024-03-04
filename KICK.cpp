#include "KICK.hpp"

KICK::KICK( void ) {

	return;
}

KICK::~KICK( void ) {

	return;
}

void KICK::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	std::cout << Get::Time() << GREEN << " --- Processing KICK command" << END << std::endl;

	for ( size_t i = 0; i < command.size(); i++ )
	{
		std::cout << Get::Time() << CYAN_BG << " --- Command: [" << command[i] << "]" << END << std::endl;
	}

	std::string & channelName = command[1];
	std::string & nickname = command[2];
	std::string comment = "";

	Channel *channel = nullptr;
	Client *kicked = nullptr;

	kicked = server.getClientByNickname( nickname );
	channel = server.getChannelByName( channelName );
	if ( !channel )
		RPL::ERR_NOSUCHCHANNEL( client, channelName );
	else if ( !kicked )
		RPL::ERR_NOSUCHNICK( client, nickname );
	else if ( !channel->clientIsInChannel( kicked ) )
		RPL::ERR_USERNOTINCHANNEL( client, nickname, channelName );
	else if ( !channel->clientIsInChannel( &client ) )
		RPL::ERR_NOTONCHANNEL( client, channelName );
	else if ( channel->isClientIsOperator(client.getNicknameId()) == -1 )
	{
		std::cout << Get::Time() << RED << " --- Client " << client.getNickname() << " [socket "<< client.getSocket() << "] is NOT operator" << END << std::endl;
		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
	}
	else
	{
		std::cout << Get::Time( ) << GREEN << " --- Client " << client.getNickname( ) << " [socket "
				  << client.getSocket( ) << "] is operator" << END << std::endl;

		for ( size_t i = 3; i < command.size( ); i++ )
		{
			comment += command[i];
			if ( ( i + 1 ) != command.size( ) )
				comment += " ";
		}
		if ( comment[0] == ':' )
			comment.erase( 0, 1 );

		for ( size_t i = 0; i < channel->getAllClients( ).size( ); i++ )
		{
			if ( channel->getAllClients( )[i]->getNickname( ) == nickname )
			{
				std::cout << Get::Time( ) << BOLD << " --- Client " << nickname << " is kicked from the channel" << END << std::endl;
				RPL::RPL_KICK( client, channelName, nickname, comment, channel->getAllClients( ) );
				channel->removeClient( nickname );
			}
		}
	}
	return;
}