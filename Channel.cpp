#include "Channel.hpp"

// Coplien's form------------------------------------------------------------------------------------------------------

Channel::Channel( void ) {
	std::cout << "Create a channel instance" << std::endl;
	// this->setName("");
	std::cout << "Channel instanced created" << std::endl;
	return ;
}

Channel::Channel(const std::string& channelName) : _name(channelName) {

}
Channel::Channel( Channel const & src ) {

	( void )src;
	return ;
}

Channel & Channel::operator=( Channel const & rhs ) {

	( void )rhs;
	return *this;
}

Channel::~Channel( void ) {
	return ;
}

// Getters --------------------------------------------------------------------------------------------------------------

const std::string & Channel::getChannelName( void )
{
	return this->_name;
}

//Client & 	Channel::getClient( const std::string & nickname )
//{
//	std::vector< Client * >::const_iterator it = _clients.begin();
//	std::vector< Client * >::const_iterator ite = _clients.end();
//
//	for ( ; it != ite; ++it )
//	{
//		if ( (*it)->getNickname() == nickname )
//			return *(*it);
//	}
//}

std::vector< Client * > &	Channel::getAllClients( void ) {

	return this->_clients;
}


//std::vector< int > Channel::getOperator( int id ) {
//
//	for ( )
//	return this->_operators;
//}

//// Setters --------------------------------------------------------------------------------------------------------------
//
//void Channel::setName(std::string name)
//{
//	this->_name = name;
//}
//
//
////Bool ------------------------------------------------------------------------------------------------------------------
//
//// // Method to check if a client is in the channel
//bool Channel::isClientInChannel( const std::string& nickname ) const {
//
//	for (size_t i = 0; i < _clients.size(); ++i)
//	{
//		if ( _clients[i]->getnickname() == nickname )
//			return true;
//	}
//	return false;
//}
//
////Channel* Channel::createChannel(std::string& channelname, Server &server)
////{
////	Channel *channel = new Channel(); //create an instance of channel
////	channel->setName(channelname);
////	server.addChannel( channel );
////	return channel;
////}
//
//
//// bool	Channel::isOperator(const std::string& nickname)
//// {
//
//// 	return true;
//// }
//
//bool Channel::isClientIsOperator(const std::string& nickname)
//{
//	int nicknameId = getNicknameId(nickname);
//	std::vector<int> ::iterator it; // = _operators.begin();
//	for (it = _operators.begin(); it != _operators.end(); it++)
//	{
//		if (*it == nicknameId)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
bool	Channel::isClientInChannel( const Client * client ) const
{
	for (std::vector< Client * >::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ( *it == client )
			return true;
	}
	return false;
}
//
//

//// Method to add a client to the channel---------------------------------------------------------------------------------
void Channel::addClient( Client & client ) {

	_clients.push_back( &client );
}

void	Channel::removeClient( const std::string & nickname )
{

	std::cout << Get::Time() << MAGNETA_BOLD << " --- Removing client " << BOLD << nickname << END << MAGNETA_BOLD << " from channel " << BOLD << this->getChannelName() << END << std::endl;

	for (std::vector< Client * >::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ( (*it)->getNickname() == nickname)
		{
			_clients.erase(it); // erase returns the next valid iterator
			break;
		}
	}
}