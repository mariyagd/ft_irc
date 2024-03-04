#include "JOIN.hpp"

JOIN::JOIN( void ) {
	
	return ;
}


JOIN::~JOIN( void ) {
	
	return ;
}

void JOIN::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	size_t i = 0;
	Channel * channel = NULL;

	std::vector< std::string > channelsNames;
	splitMsgOnComma( command[1], channelsNames );

	for ( ; i < channelsNames.size(); i++ )
	{
		channel = server.getChannelByName(channelsNames[i]);
		if ( !channel )
		{
			std::cout << Get::Time() << BOLD << " --- Channel doesn't exists. Create a new channel" << END << std::endl;
			channel = server.createChannel(channelsNames[i] );
			channel->addOperator( client.getNicknameId() );
			std::cout << Get::Time() << BOLD << " --- " << client.getNickname() << " created the channel " << channelsNames[i] << END << std::endl;
		}
		if ( !channel->clientIsInChannel( &client ) )
		{
			std::cout << Get::Time() << BOLD << " --- Add " << client.getNickname() << " in channel " << channelsNames[i] << END << std::endl;
			if ( channel->getInviteMode() && !channel->isClientInvited( client.getNicknameId() ) )
				RPL::ERR_INVITEONLYCHAN( client, channelsNames[i] );
			else if ( channel->getLimitMode() && channel->getAllClients().size() >= static_cast< size_t >( channel->getLimit() ) )
				RPL::ERR_CHANNELISFULL( client, channelsNames[i] );
			else
			{
				if ( channel->getKeyMode() && (command.size() != 3 || command[2] != channel->getKey()) )
				{
					RPL::RPL_BADCHANNELKEY( client, channelsNames[i] );
					return;
				}
				RPL::RPL_JOIN( client, channel->getAllClients(), channelsNames[i] );
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
				std::cout << Get::Time( ) << BOLD << " --- " << client.getNickname( ) << " joined the channel " << channelsNames[i] << END << std::endl;
			}
		}
	}
	return ;
}