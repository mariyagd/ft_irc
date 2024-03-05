#include "KICK.hpp"

KICK::KICK( void ) {

	return;
}

KICK::~KICK( void ) {

	return;
}

/*
 * KICK <channel> <user> *( "," <user> ) [<comment>]
 */

void KICK::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	std::cout << Get::Time() << GREEN << " --- Processing KICK command" << END << std::endl;

	if ( command.size() < 3 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: KICK <channel> <user>" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "KICK" );
		return;
	}

	printVector(command);
	Channel *channel = nullptr;
	Client *kicked = nullptr;
	std::string & channelName = command[1];
	std::string & nickname = command[2];
	std::string comment = "";


	kicked = server.getClientByNickname( nickname );
	channel = server.getChannelByName( channelName );

	if ( !channel )
	{
		std::cout << Get::Time() << RED_BOLD << " --- No such channel: " << command[1] << END << std::endl;
		RPL::ERR_NOSUCHCHANNEL( client, channelName );
	}
	else if ( !kicked )
	{
		std::cout << Get::Time() << RED_BOLD << " --- No such nick: " << command[2] << END << std::endl;
		RPL::ERR_NOSUCHNICK( client, nickname );
	}
	else if ( !channel->clientIsInChannel( kicked ) )
	{
		std::cout << Get::Time() << RED_BOLD << " --- User to be kicked is not on channel" << END << std::endl;
		RPL::ERR_USERNOTINCHANNEL( client, nickname, channelName );
	}
	else if ( !channel->clientIsInChannel( &client ) )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Kicker is not on the channel" << END << std::endl;
		RPL::ERR_NOTONCHANNEL( client, channelName );
	}
	else if ( channel->isClientIsOperator(client.getNicknameId()) == -1 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Kicker is NOT operator" << END << std::endl;
		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
	}
	else
	{
		std::cout << Get::Time( ) << BOLD << " --- Kicker is operator" << END << std::endl;

		concatenate( command, 3, comment );

		for ( size_t i = 0; i < channel->getAllClients( ).size( ); i++ )
		{
			if ( channel->getAllClients( )[i]->getNickname( ) == nickname )
			{
				std::cout << Get::Time( ) << BOLD << " --- Client " << nickname << " is kicked from the channel" << END << std::endl;
				RPL::RPL_KICK( client, channelName, nickname, comment, channel->getAllClientsSockets() );
				channel->removeClient( nickname );
			}
		}
	}
	return;
}