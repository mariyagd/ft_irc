#include "INVITE.hpp"

INVITE::INVITE( void ) : ACommand() {
}

INVITE::~INVITE( void ) {
}

/*
 * INVITE <nickname> <channel>
 */
void INVITE::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	if ( command.size() < 3 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: INVITE <nickname> <channel>" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "INVITE");
	}

	std::string & nickname = command[1];
	std::string & channelName = command[2];
	Channel *channel = nullptr;
	Client * invited = nullptr;

//	std::cout << Get::Time() << GREEN << " --- Processing INVITE command" << END << std::endl;

	invited = server.getClientByNickname( nickname );
	channel = server.getChannelByName( channelName );
	if ( !invited )
	{
		std::cout << Get::Time() << RED_BOLD << " --- No such nick" << END << std::endl;
		RPL::ERR_NOSUCHNICK( client, nickname );
	}
	else if ( channelName.find_first_of("&#+!") != 0 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Bad channel prefix" << END << std::endl;
		RPL::ERR_BADCHANMASK( client, channelName );
	}
	else if ( !channel )
	{
		std::cout << Get::Time() << RED_BOLD << " --- No such channel" << END << std::endl;
		RPL::ERR_NOSUCHCHANNEL( client, channelName );
	}
	else if ( channel->clientIsInChannel( invited ) )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Invited user is already in channel " << channelName << END << std::endl;
		RPL::ERR_USERONCHANNEL( client, nickname, channelName );
	}
	else if ( channel->isClientIsOperator( client.getNicknameId() ) == -1 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- You are not an operator" << END << std::endl;
		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
	}
	else
	{
		std::cout << Get::Time() << GREEN_BOLD << " --- Send invitation to " << nickname << END << std::endl;
		RPL::RPL_INVITING( client, *invited, channelName );
		channel->addInvited( invited->getNicknameId() );
	}
}