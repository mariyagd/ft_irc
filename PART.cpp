#include "PART.hpp"

PART::PART( void ) {

	return;
}

PART::~PART( void ) {

	return;
}

/*
 * PART <channel>{,<channel>} [<reason>]
 */
void PART::execute( std::vector< std::string > & command, Client & client, Server & server ) {

//	std::cout << Get::Time() << GREEN << " --- Processing PART command" << END << std::endl;
	if ( !client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Client not registered" << END << std::endl;
		return;
	}
	if ( command.size() < 2 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: PART <channel>{,<channel>} [<reason>]" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "PART" );
		return;
	}

	std::vector< std::string > channelsName;
	std::string comment;
	Channel *channel = nullptr;

	splitMsgOnComma( command[1], channelsName );
	if ( command.size() > 2 )
		concatenate(command, 2, comment);
	for ( size_t i = 0; i < channelsName.size(); i++ )
	{
		if ( channelsName[i].find_first_of("&#+!") != 0 )
		{
			std::cout << Get::Time() << RED_BOLD << " --- Bad channel prefix" << END << std::endl;
			RPL::ERR_BADCHANMASK( client, channelsName[i] );
			continue;
		}
		channel = server.getChannelByName( channelsName[i] );
		if ( !channel )
		{
			std::cout << Get::Time() << RED_BOLD << " --- No such channel" << END << std::endl;
			RPL::ERR_NOSUCHCHANNEL( client, channelsName[i] );
		}
		else if ( !channel->clientIsInChannel( &client ) )
		{
			std::cout << Get::Time() << RED_BOLD << " --- You are not in the channel"<< END << std::endl;
			RPL::ERR_NOTONCHANNEL( client, channelsName[i] );
		}
		else
		{
			std::cout << Get::Time() << GREEN_BOLD << " --- Client " << client.getNickname() << " left channel " << channelsName[i] << END << std::endl;
			RPL::RPL_PART( client, channel->getAllClientsSockets(), channelsName[i], comment );
			channel->removeClient( client.getNickname() );
			if (channel->getAllClients().size() == 0 )
			{
				server.deleteChannel(channel);
				server.print_all_info();
			}
			else
				channel->print_channels_info();
		}
	}
}