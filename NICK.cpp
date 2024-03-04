#include "NICK.hpp"

NICK::NICK( void ) : ACommand() {
	return ;
}

NICK::NICK( std::string & line ) : ACommand( line ) {
	return ;
}

NICK::~NICK( void ) {
	return ;
}

void NICK::execute( std::string & line, Client & client, Server &server ) {

//	std::cout << Get::Time() << GREEN << " --- Processing NICK command" << END << std::endl;

	splitMsgOnSpace( line, command );
	std::string nickname = command[1];

	if ( client.getSocket() < 0 )
		return;

	if ( command.size() == 1 )
	{
		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname <<  " no nickname given" << END << std::endl;

		RPL::ERR_NONICKNAMEGIVEN( client );
		return;
	}

	for ( size_t i = 0; i < nickname.length(); i++ )
	{
		if ( nickname.find_first_of(" :+-&?!@#$%*;,./<>=\"\\~") != std::string::npos )
		{
			std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname << " erroneous nickname" << END << std::endl;

			RPL::ERR_ERRONEUSNICKNAME( client );
			return ;
		}
	}

	if ( !is_unique_nickname( nickname, client.getSocket(), server.getConnections() ) )
	{
		std::cout << Get::Time() << BOLD << " --- socket " << client.getSocket() << " Nick: " << nickname << " nickname is use" << END << std::endl;
		RPL::ERR_NICKNAMEINUSE( client, nickname );
	}
	else if ( !client.isRegistered() )
	{
		std::cout << Get::Time() << " --- User [socket " << client.getSocket() << "] can register with name [" << nickname << "]" << END << std::endl;
		client.setNickname( nickname );
		RPL::RPL_NICK( client, nickname );
	}
	else
	{
		std::cout << Get::Time() << " --- Nickname " << client.getNickname() << " changed to " << nickname << END << std::endl;
		RPL::RPL_NICK( client, nickname );
		client.setNickname( nickname );
	}
	client.printInfo();
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