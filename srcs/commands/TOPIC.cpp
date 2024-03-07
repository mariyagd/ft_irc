#include "TOPIC.hpp"

TOPIC::TOPIC( void ) {
	return;
}

TOPIC::~TOPIC( void ) {
	return;
}

/*
 * TOPIC <channel> [<topic>]
 */

void TOPIC::execute( std::vector< std::string > & command, Client & client, Server & server ) {

//	std::cout << Get::Time() << GREEN << " --- Processing TOPIC command" << END << std::endl;

	if ( !client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Client not registered" << END << std::endl;
		RPL::ERR_NOTREGISTERED( client );
		return;
	}
	if ( command.size() < 2 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: TOPIC <channel> [<topic>]" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "TOPIC" );
		return;
	}

	Channel *channel = nullptr;
	std::string & channelName = command[1];
	std::string topic;

	if ( command.size() > 2 )
		concatenate(command, 2, topic );
	if ( channelName.find_first_of("&#+!") != 0 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Bad channel prefix" << END << std::endl;
		RPL::ERR_BADCHANMASK( client, channelName );
		return;
	}
	channel = server.getChannelByName( channelName );
	if ( !channel )
	{
		std::cout << Get::Time() << RED_BOLD << " --- No such channel" << END << std::endl;
		RPL::ERR_NOSUCHCHANNEL( client, channelName );
	}
	else if ( !channel->clientIsInChannel( &client ) )
	{
		std::cout << Get::Time() << RED_BOLD << " --- You are not on the channel " << END << std::endl;
		RPL::ERR_NOTONCHANNEL( client, channelName );
	}
	else if ( command.size() == 2 )
	{
		if (channel->getTopic().empty())
		{
			std::cout << Get::Time() << BOLD << " --- No channel topic " << END << std::endl;
			RPL::RPL_NOTOPIC(client, channelName);
		}
		else
		{
			std::cout << Get::Time() << GREEN_BOLD << " --- Send channel topic " << END << std::endl;
			RPL::RPL_TOPIC(client, channelName, channel->getTopic());
			RPL::RPL_TOPICWHOTIME(client, *channel->getTopicSetter(), channelName, channel->getTopicCreationTime());
		}
	}
	else if ( !channel->getTopicMode() || channel->isClientIsOperator( client.getNicknameId() ) != -1 )
	{
		std::cout << Get::Time() << GREEN_BOLD << " --- Set new channel topic" << END << std::endl;
		channel->setTopic( topic, client.getNicknameId() );
		RPL::RPL_NORMAL( client, channel->getAllClients(), channelName, "TOPIC", channel->getTopic() );
		RPL::RPL_TOPIC( client, channelName, channel->getTopic() );
	}
	else
	{
		std::cout << Get::Time() << RED_BOLD << " --- You are not an operator" << END << std::endl;
		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
	}
}