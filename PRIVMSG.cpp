#include "PRIVMSG.hpp"

PRIVMSG::PRIVMSG() {
}

PRIVMSG::~PRIVMSG( void ) {
}

/*
 *  PRIVMSG <target>{,<target>} <text to be sent>
 */
void PRIVMSG::execute( std::vector< std::string > & command, Client & client, Server &server ) {

	std::cout << Get::Time() << GREEN << " --- Processing PRIVMSG command" << END << std::endl;

	if ( command.size() < 3 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: PRIVMSG <target>{,<target>} <text to be sent>" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "PRIVMSG" );
		return;
	}
	
	std::vector< std::string > target;
	std::string message;
	std::cout << "target = " << command[1] << std::endl;

	splitMsgOnComma( command[1], target );
	concatenate(command, 2, message );
	std::cout << "message = " << message << std::endl;
	for (size_t i = 0; i < target.size(); ++i )
	{
		std::cout << "target[" << i << "] = " << target[i] << std::endl;
	}
	for (size_t i = 0; i < target.size(); ++i )
	{
		if (target[i].find_first_of("&#+!") == 0) // if the destination is a channel
			send_in_channel( server, client,  message, target[i] );
		else
			send_to_client(server, client, message, target[i] );

	}
}

void PRIVMSG::send_in_channel( Server &server, Client & client, const std::string & message, const std::string & target ) {

	std::cout << target << std::endl;
	Channel * channel = server.getChannelByName( target);
	if ( !channel )
	{
		std::cerr << Get::Time() << RED_BOLD << " --- Channel doesn't exist " << END << std::endl;
		RPL::ERR_NOSUCHNICK( client, target);

	}
	else if ( channel->clientIsInChannel( &client ) )
	{
		std::cout << Get::Time() << BOLD << " --- Sending message to all clients in channel " << target<< END << std::endl;
		for ( size_t j = 0; j < channel->getAllClients().size(); j++ )
		{
			if ( channel->getAllClients()[j]->getSocket() != client.getSocket() )
				RPL::RPL_PRIVMSG( client, target, message, channel->getAllClients()[j]->getSocket() );
		}
	}
	else
	{
		std::cerr << Get::Time() << RED_BOLD << " --- Cannot send to channel " << target<< END << std::endl;
		RPL::ERR_CANNOTSENDTOCHAN( client, target);
	}
}

void PRIVMSG::send_to_client( Server & server, Client & client, const std::string & message, const std::string & target ) {

	std::cout << Get::Time() << GREEN_BOLD << " --- Send private message to " << target << END << std::endl;

	int	receiverSocket = server.getSocketByNickname(target);
	if (receiverSocket < 0)
		RPL::ERR_NOSUCHNICK( client, target);
	else
	{
		printf("receiverSocket = %d\n", receiverSocket);
		RPL::RPL_PRIVMSG( client, target, message, receiverSocket );
		std::cout << Get::Time() << BOLD << " --- Private message sent to " << target << END << std::endl;
	}
}