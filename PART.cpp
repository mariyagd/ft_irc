#include "PART.hpp"

PART::PART( void ) {

	return;
}

PART::~PART( void ) {

	return;
}

void PART::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	std::cout << Get::Time() << GREEN << " --- Processing PART command" << END << std::endl;

	std::vector< std::string > channelsName;
	std::string comment;
	Channel *channel = nullptr;

	splitMsgOnComma( command[1], channelsName );
	if ( command.size() > 2 )
		concatenate(command, 2, comment);

	for ( size_t i = 0; i < channelsName.size(); i++ )
	{
		channel = server.getChannelByName( channelsName[i] );
		if ( !channel )
		{
			std::cout << Get::Time() << RED_BOLD << " --- No such channel " << channelsName[i] << " in the network" << END << std::endl;
			RPL::ERR_NOSUCHCHANNEL( client, channelsName[i] );
		}
		else if ( !channel->clientIsInChannel( &client ) )
		{
			std::cout << Get::Time() << RED_BOLD << " --- You are not in channel " << channelsName[i] << END << std::endl;
			RPL::ERR_NOTONCHANNEL( client, channelsName[i] );
		}
		else
		{
			std::cout << Get::Time() << BOLD << " --- Client " << client.getNickname() << " is leaving channel " << channelsName[i] << END << std::endl;
			RPL::RPL_PART( client, channel->getAllClients(), channelsName[i], comment );
			channel->removeClient( client.getNickname() );
		}
	}
}