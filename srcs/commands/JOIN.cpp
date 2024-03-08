#include "JOIN.hpp"

JOIN::JOIN( void ) {
	
	return ;
}


JOIN::~JOIN( void ) {
	
	return ;
}

/*
 *  JOIN  <channel>{,<channel>} [<key>{,<key>}]
 */

void JOIN::execute( std::vector< std::string > & command, Client & client, Server &server ) {

//	std::cout << Get::Time() << GREEN << " --- Processing JOIN command" << END << std::endl;

	size_t i = 0;
	size_t j = 0;
	Channel * channel = NULL;

	std::vector< std::string > channelsNames;
	std::vector< std::string > keys;

	if ( !client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Client not registered" << END << std::endl;
		RPL::ERR_NOTREGISTERED( client );
		return;
	}
	if ( command.size() < 2 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: JOIN  <channel>{,<channel>} [<key>{,<key>}]" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "JOIN" );
		return ;
	}
	splitMsgOnComma( command[1], channelsNames );
	if ( command.size() > 2 )
		splitMsgOnComma( command[2], keys );
	for ( ; i < channelsNames.size(); i++ )
	{
		if ( channelsNames[i].find_first_of("&#+!") != 0 )
		{
			channelsNames[i] = "#" + channelsNames[i];
		}
		if ( channelsNames[i].size() > MAXCHANNELLEN )
		{
			std::cout << Get::Time() << RED_BOLD << " --- Channel name too long" << END << std::endl;
			RPL::ERR_BADCHANNAME( client, channelsNames[i] );
			continue;
		}
		if ( client.getNbChannels() >= CHANLIMIT )
		{
			std::cout << Get::Time() << RED_BOLD << " --- Client is in too many channels. Channel is not created" << END << std::endl;
			RPL::ERR_TOOMANYCHANNELS( client, channelsNames[i] );
			return;
		}
		channel = server.getChannelByName(channelsNames[i]);
		if ( !channel )
		{
			std::cout << Get::Time() << MAGNETA_BOLD << " --- Channel doesn't exists" << END << std::endl;
			channel = server.createChannel(channelsNames[i]);
			std::cout << Get::Time() << GREEN_BOLD << " --- " << client.getNickname() << " created channel [" << channelsNames[i] << "]" << END << std::endl;
			channel->addOperator(client.getNicknameId());
			std::cout << Get::Time() << GREEN_BOLD << " --- " << client.getNickname() << " is now an operator" << std::endl;
		}
		if ( !channel->clientIsInChannel( &client ) ) // if client is not in the channel
		{
			if ( modeRequirementsOK(channelsNames[i], keys, j, channel, client ) )
			{
				join_and_send(channel, client, channelsNames[i] );
			}
		}
	}
	return ;
}

void JOIN::join_and_send( Channel * channel, Client & client, const std::string & channelName ) {

	std::cout << Get::Time() << GREEN_BOLD << " --- " << client.getNickname() << " joined in channel [" << channelName << "]" << END << std::endl;
	channel->addClient( client );
	RPL::RPL_JOIN( client, channel->getAllClientsSockets(), channelName );

	if ( channel->isClientIsOperator( client.getNicknameId( ) ) != -1 )
	{
		RPL::RPL_MODE_OP( client, channelName, channel->getCurrentChannelModes( ) );
	}
	if ( channel->isClientInvited( client.getNicknameId() ) )
		channel->removeInvited( client.getNicknameId() );
	if ( !channel->getTopic().empty() )
	{
		RPL::RPL_TOPIC( client, channelName, channel->getTopic( ) );
		RPL::RPL_TOPICWHOTIME( client, *channel->getTopicSetter(), channelName, channel->getTopicCreationTime( ) );
	}
	RPL::RPL_NAMREPLY( client, channelName, channel->getOperatorsName( ), channel->getMembersName( ));
	RPL::RPL_ENDOFNAMES( client, channelName );
	channel->print_channels_info();
}


bool JOIN::modeRequirementsOK( const std::string & channelName, const std::vector< std::string > & keys, size_t & j, Channel * channel, Client & client ) {

//	std::cout << CYAN_BG << "mode = " << ( channel->getLimitMode() == true ? "true" : "false" ) << END << std::endl;
//	std::cout << CYAN_BG << "limit = " << channel->getLimit() << END << std::endl;
//	std::cout << CYAN_BG << "current = " << channel->getAllClients().size() << END << std::endl;

	if ( channel->getKeyMode() && ( keys.empty() ||  ( keys.size() > j && keys[j++] != channel->getKey() ) ) )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Key mode: wrong password" << END << std::endl;
		RPL::RPL_BADCHANNELKEY( client, channelName );
		return false;
	}
	else if ( channel->getLimitMode() && channel->getAllClients().size() >= static_cast< size_t >( channel->getLimit() ) && !channel->isClientInvited( client.getNicknameId() ))
	{
		std::cout << Get::Time() << RED_BOLD << " --- Limit mode: channel is full" << END << std::endl;
		RPL::ERR_CHANNELISFULL( client, channelName );
		return false;
	}
	else if ( channel->getInviteMode() && !channel->isClientInvited( client.getNicknameId() ) )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Invite mode: need an invitation" << END << std::endl;
		RPL::ERR_INVITEONLYCHAN( client, channelName );
		return false;
	}
//	else if ( channel->getLimitMode() && !channel->isClientInvited( client.getNicknameId() ) )
//	{
//		std::cout << Get::Time() << RED_BOLD << " --- Limit mode: channel is full1" << END << std::endl;
//		RPL::ERR_CHANNELISFULL( client, channelName );
//		return false;
//	}


	return true;
}
