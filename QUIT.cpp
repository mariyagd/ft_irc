#include "QUIT.hpp"

QUIT::QUIT( void ) {
	return;
}

QUIT::~QUIT( void ) {
	return;
}

/*
 *  QUIT [<reason>]
 */
void QUIT::execute(std::vector<std::string> & command, Client & client, Server & server) {

	std::string reason;

	if ( command.size() > 1 )
		concatenate( command, 1, reason );
	RPL::QUIT( client, client.getAllClientsInAllChannels(), reason );

	for ( size_t i = 0; i < client.getAllChannels().size(); i++ )
	{
		Channel *channel = client.getAllChannels()[i];

		channel->removeClient( client.getNickname() );
		if (channel->getAllClients().size() == 0 )
		{
			server.deleteChannel(channel);
			server.print_all_info();
		}
		else
			channel->print_channels_info();
	}
	client.closeSocket();
	return;
}