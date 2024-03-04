#include "INVITE.hpp"

INVITE::INVITE( void ) : ACommand() {
}

INVITE::~INVITE( void ) {
}

void INVITE::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	std::string & nickname = command[1];
	std::string & channelName = command[2];
	Channel *channel = nullptr;
	Client * invited = nullptr;

//	std::cout << Get::Time() << GREEN << " --- Processing INVITE command" << END << std::endl;

	for ( size_t i = 0; i < command.size(); i++ )
	{
		std::cout << Get::Time() << CYAN_BG << " --- Command: [" << command[i] << "]" << END << std::endl;
	}

	invited = server.getClientByNickname( nickname );
	channel = server.getChannelByName( channelName );
	if ( !invited )
	{
		std::cout << Get::Time() << RED_BOLD << " --- No such nick " << nickname << " in the network" << END << std::endl;
		RPL::ERR_NOSUCHNICK( client, nickname );
	}
	else if ( !channel )
	{
		std::cout << Get::Time() << RED_BOLD << " --- No such channel " << channelName << " in the network" << END << std::endl;
		RPL::ERR_NOSUCHCHANNEL( client, channelName );
	}
	else if ( channel->clientIsInChannel( invited ) )
	{
		std::cout << Get::Time() << RED_BOLD << " --- User " << nickname << " is already in channel " << channelName << END << std::endl;
		RPL::ERR_USERONCHANNEL( client, nickname, channelName );
	}
	else if ( channel->isClientIsOperator( client.getNicknameId() ) == -1 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- You are not an operator" << END << std::endl;
		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
	}
	else
	{
		std::cout << Get::Time() << GREEN_BOLD << " --- Inviting user " << nickname << " in channel " << channelName << END << std::endl;
		RPL::RPL_INVITING( client, *invited, channelName );
		channel->addInvited( invited->getNicknameId() );
	}
}