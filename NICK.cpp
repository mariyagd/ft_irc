#include "NICK.hpp"

NICK::NICK( void ) : ACommand() {
	return ;
}

NICK::~NICK( void ) {
	return ;
}

// NICK <nickname>
void NICK::execute( std::vector< std::string > & command, Client & client, Server &server ) {

//	std::cout << Get::Time() << GREEN << " --- Processing NICK command" << END << std::endl;

	std::string nickname = command[1];

	if ( client.getSocket() < 0 )
		return;
	else if ( command.size() == 1 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: NICK <nickname>" << END << std::endl;
		RPL::ERR_NONICKNAMEGIVEN( client );
	}
	else if ( nickname.find_first_of(" :+-&?!@#$%*;,./<>=\"\\~") != std::string::npos )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Erroneous nickname" << END << std::endl;
		RPL::ERR_ERRONEUSNICKNAME( client );
	}
	else if ( client.getNickname() == nickname )
	{
		std::cout << Get::Time() << " --- socket " << client.getSocket() << " Nick: " << nickname << " nickname is the same. Ignore request" << std::endl;
		return;
	}
	else if ( !is_unique_nickname( nickname, client.getSocket(), server.getConnections() ) )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Nickname is use" << END << std::endl;
		RPL::ERR_NICKNAMEINUSE( client, nickname );
	}
	else if ( !client.isRegistered() )
	{
		std::cout << Get::Time() << " --- User can register with name [" << nickname << "]" << END << std::endl;
		client.setNickname( nickname );
		RPL::RPL_NICK( client, client.getAllClientsInAllChannels(), nickname );
	}
	else
	{
		std::cout << Get::Time() << GREEN_BOLD << " --- Nickname " << client.getNickname() << " changed to " << nickname << END << std::endl;
		RPL::RPL_NICK( client, client.getAllClientsInAllChannels(), nickname );
		client.setNickname( nickname );
		client.printInfo();
	}
	return ;
}

bool NICK::is_unique_nickname(  std::string & nickname, int clientSocket, std::vector< Client > & connections )
{
	for ( int i = 0; i < MAX_CONNECTIONS; ++i )
	{
		if ( connections[i].getSocket() >= 0 && connections[i].getSocket() != clientSocket && connections[i].getNickname() == nickname )
			return false;
	}
	return true;
}