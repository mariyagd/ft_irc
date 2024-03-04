#include "TOPIC.hpp"

TOPIC::TOPIC( void ) {
	return;
}

TOPIC::~TOPIC( void ) {
	return;
}

void TOPIC::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	std::string & channelName = command[1];
	std::string topic;
	concatenate(command, 2, topic );
	Channel *channel = nullptr;

	std::cout << Get::Time() << GREEN << " --- Processing TOPIC command" << END << std::endl;

	channel = server.getChannelByName( channelName );
	if ( !channel )
	{
		std::cout << Get::Time() << RED_BOLD << " --- No such channel " << channelName << " in the network" << END << std::endl;
		RPL::ERR_NOSUCHCHANNEL( client, channelName );
	}
	else if ( !channel->clientIsInChannel( &client ) )
	{
		std::cout << Get::Time() << RED_BOLD << " --- You are not in channel " << channelName << END << std::endl;
		RPL::ERR_NOTONCHANNEL( client, channelName );
	}
	else if ( command.size() == 2 )
	{
		if (channel->getTopic().empty())
		{
			std::cout << Get::Time() << " --- no channel topic " << std::endl;
			RPL::RPL_NOTOPIC(client, channelName);
		}
		else
		{
			std::cout << Get::Time() << BOLD << " --- Send channel topic " << END << std::endl;
			RPL::RPL_TOPIC(client, channelName, channel->getTopic());
			RPL::RPL_TOPICWHOTIME(client, *channel->getTopicSetter(), channelName, channel->getTopicCreationTime());
		}
	}
	else if ( !channel->getTopicMode() || channel->isClientIsOperator( client.getNicknameId() ) != -1 )
	{
		std::cout << Get::Time() << BOLD << " --- Set channel topic hrhr" << END << std::endl;
		channel->setTopic( topic, client.getNicknameId() );
		RPL::RPL_NORMAL( client, channel->getAllClients(), channelName, "TOPIC", topic );
		RPL::RPL_TOPIC( client, channelName, topic );
	}
	else
	{
		std::cout << Get::Time() << RED_BOLD << " --- You are not an operator" << END << std::endl;
		RPL::ERR_CHANOPRIVSNEEDED( client, channelName );
	}
}