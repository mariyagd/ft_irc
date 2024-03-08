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

	size_t nb_channels = client.getAllChannels().size();
	for ( size_t i = 0; i < nb_channels; i++ )
	{
		for ( size_t j = 0; j < client.getAllChannels().size(); j++ )
		{
			Channel *channel = client.getAllChannels()[j];
			channel->removeClient(client.getNickname());
			if (channel->getAllClients().size() == 0)
				server.deleteChannel(channel);
		}
	}
	client.closeSocket();
	server.print_all_info();
	return;
}