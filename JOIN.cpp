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

	size_t i = 0;
	size_t j = 0;
	Channel * channel = NULL;

	std::vector< std::string > channelsNames;
	std::vector< std::string > keys;


	if ( command.size() < 2 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Command JOIN need more params" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "JOIN" );
		return ;
	}
	splitMsgOnComma( command[1], channelsNames );
	if ( command.size() > 2 )
		splitMsgOnComma( command[2], keys );
	for ( ; i < channelsNames.size(); i++ )
	{
		channel = server.getChannelByName(channelsNames[i]);
		if ( !channel )
		{
			std::cout << Get::Time() << MAGNETA_BOLD << " --- Channel doesn't exists. Creating a new channel" << END << std::endl;
			channel = server.createChannel(channelsNames[i] );
			channel->addOperator( client.getNicknameId() );
			std::cout << Get::Time() << GREEN_BOLD << " --- " << client.getNickname() << " created the channel " << channelsNames[i] << END << std::endl;
		}
		if ( !channel->clientIsInChannel( &client ) )
		{
			if ( channel->getKeyMode() && ( keys.empty() ||  ( keys.size() > j && keys[j++] != channel->getKey() ) ) )
			{
				std::cout << Get::Time() << RED_BOLD << " --- Channel " << channel->getChannelName() << " is in key mode and you gave a wrong password" << END << std::endl;
				RPL::RPL_BADCHANNELKEY( client, channelsNames[i] );
			}
			else if ( channel->getLimitMode() && channel->getAllClients().size() >= static_cast< size_t >( channel->getLimit() ) )
			{
				std::cout << Get::Time() << RED_BOLD << " --- Channel " << channel->getChannelName() << " is in limite mode and it's already full" << END << std::endl;
				RPL::ERR_CHANNELISFULL( client, channelsNames[i] );
			}
			else if ( channel->getInviteMode() && !channel->isClientInvited( client.getNicknameId() ) )
			{
				std::cout << Get::Time() << RED_BOLD << " --- Channel " << channel->getChannelName() << " is in invite mode and you don't have an invitation" << END << std::endl;
				RPL::ERR_INVITEONLYCHAN( client, channelsNames[i] );
			}
			else
			{
				std::cout << Get::Time() << GREEN_BOLD << " --- Add client [" << client.getNickname() << "] ID [" << client.getNicknameId() << "]" << " socket [" << client.getSocket() << "]" << " in channel " << channelsNames[i] << END << std::endl;
				RPL::RPL_JOIN( client, channel->getAllClientsSockets(), channelsNames[i] );
				channel->addClient( client );

				if ( channel->isClientIsOperator( client.getNicknameId( ) ) != -1 )
				{
					RPL::RPL_MODE_OP( client, channelsNames[i], channel->getCurrentChannelModes( ) );
				}
				if ( channel->isClientInvited( client.getNicknameId() ) )
					channel->removeInvited( client.getNicknameId() );
				if ( !channel->getTopic().empty() )
				{
					RPL::RPL_TOPIC( client, channelsNames[i], channel->getTopic( ) );
					RPL::RPL_TOPICWHOTIME( client, *channel->getTopicSetter(), channelsNames[i], channel->getTopicCreationTime( ) );
				}
				RPL::RPL_NAMREPLY( client, channelsNames[i], channel->getOperatorsName( ) );
				RPL::RPL_ENDOFNAMES( client, channelsNames[i] );
			}
		}
	}
	return ;
}