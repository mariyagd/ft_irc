#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG() {
}

PRIVMSG::~PRIVMSG( void ) {
}

void PRIVMSG::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	std::cout << Get::Time() << GREEN << " --- Processing PRIVMSG command" << END << std::endl;

	std::vector< std::string > target;
	std::string message = "";

	splitMsgOnComma( command[1], target );
	concatenate(command, 2, message );

	for (size_t i = 0; i < target.size(); ++i )
	{

		if (target[i].find_first_of("&#+!") == 0) // if the destination is a channel
		{
			Channel * channel = server.getChannelByName( target[i] );
			if (! channel )
			{
				std::cerr << Get::Time() << RED << " --- Error: Failed to send message to channel. Channel doesnt't exist " << target[i] << END << std::endl;
				RPL::ERR_NOSUCHNICK( client, target[i] );

			}
			else if ( channel->clientIsInChannel( &client ) )
			{
				std::cout << Get::Time() << BOLD << " --- Sending message to all clients in channel " << target[i] << END << std::endl;
				for ( size_t j = 0; j < channel->getAllClients().size(); j++ )
				{
					if ( channel->getAllClients()[j]->getSocket() != client.getSocket() )
						RPL::RPL_PRIVMSG( client, target[i], message, channel->getAllClients()[j]->getSocket() );
				}
			}
			else
			{
				std::cerr << Get::Time() << RED << " --- Error: Failed to send message to channel. Cannot send to channel " << target[i] << END << std::endl;
				RPL::ERR_CANNOTSENDTOCHAN( client, target[i] );
			}
		}
		else
		{
			std::cout << Get::Time() << BOLD << " --- Sending private message to " << target[i] << END << std::endl;

			int	receiverSocket = server.getSocketByNickname(target[i]);
			if (receiverSocket < 0)
				RPL::ERR_NOSUCHNICK( client, target[i] );
			else
			{
				RPL::RPL_PRIVMSG( client, target[i], message, receiverSocket );
				std::cout << Get::Time() << BOLD << " --- Private message sent to " << target[i] << END << std::endl;
			}
		}
	}
}