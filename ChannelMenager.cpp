#include "ChannelMenager.hpp"


// Public Constructor--------------------------------------------------------------------------------------------------

ChannelMenager::ChannelMenager( void ) : _channels() {

	return ;
}

// Destructor-----------------------------------------------------------------------------------------------------------

ChannelMenager::~ChannelMenager( void ) {

	for (size_t i = 0; i < _channels.size(); ++i)
		delete _channels[i];
	return ;
}


// Getters --------------------------------------------------------------------------------------------------------------

std::vector< Channel * > &		ChannelMenager::getAllChannels( void ) {

	return ChannelMenager::_channels;
}

Channel * ChannelMenager::createChannel( std::string name )
{
	Channel * new_channel = new Channel(name);
	_channels.push_back( new_channel );
	return new_channel;
}

void	ChannelMenager::deleteChannel( Channel * rhs ) {

	if (!rhs)
		return;

	std::vector<Channel *>::iterator it = std::find(_channels.begin(), _channels.end(), rhs);
	if ( it != _channels.end() )
	{
		std::cout << MAGNETA_BOLD << "Channel " << rhs->getChannelName() << " is empty. Deleting channel from server" << END << std::endl;

		std::vector< Client * > & allClients = (*it)->getAllClients();
		for (size_t i = 0; i < allClients.size(); ++i)
		{
			(*it)->removeClient( allClients[i]->getNickname() );
		}

		_channels.erase(it);
		delete rhs;
	}
	else
	{
		std::cout << MAGNETA_BOLD << " --- Could not find Channel " << rhs->getChannelName() << " in the server" << END << std::endl;
	}
}

void ChannelMenager::deleteAllChannels( ) {

	for (std::vector< Channel * >::iterator  it = _channels.begin(); it != _channels.end(); ++it )
	{
		if ( *it != nullptr )
		{
			std::cout << Get::Time << GREEN_BOLD << " --- Deleting channel " << (*it)->getChannelName() << END << std::endl;
			delete *it;
			*it = nullptr;
		}
	}
}

Channel * ChannelMenager::getChannelByName(const std::string & channelName)
{
	// Find the channel in the vector
	size_t i = 0;
	for ( ; i < _channels.size(); ++i)
	{
		if ( _channels[i]->getChannelName() == channelName )
			return _channels[i]; // Return pointer to the Channel object
	}
	return nullptr; // Return null if it doesn't exist
}

// Bool ------------------------------------------------------------------------------------------------------------------

bool	ChannelMenager::channelExists(const std::string & channelName)
{
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i]->getChannelName() == channelName)
		{
			return true;
		}
	}
	return false;
}

//// Public methodds ------------------------------------------------------------------------------------------------------

void	ChannelMenager::removeClientFromAllChannels( Client * client )
{
	std::cout << Get::Time() << MAGNETA_BG << BOLD << " --- Removing client " << client->getNickname() << " from all channels on disconnection " << END << std::endl;

	for (size_t i = 0; i < _channels.size(); ++i)
	{
		_channels[i]->removeClient( client->getNickname() );
	}
	std::cout << Get::Time() << MAGNETA_BG << BOLD << " --- Removing end    " << END << std::endl;

}

// Printing --------------------------------------------------------------------------------------------------------------

void	ChannelMenager::print_channels_info() {


	for (size_t i = 0; i < _channels.size(); ++i )
	{
//		std::cout << BLUE_BOLD << std::setw(50) << std::setfill('-') << "" << END << std::endl;
		std::cout << BLUE_BOLD << " Channel : [" << END << _channels[i]->getChannelName() << "]" << std::endl;
		std::cout << BLUE_BOLD << " Modes : ["  << END << _channels[i]->getCurrentChannelModes() << "]" << std::endl;
		std::cout << BLUE_BOLD << " Clients: " << END << std::endl;
		std::vector< Client * > & clients =  _channels[i]->getAllClients();
		for ( size_t j = 0; j < clients.size(); ++j )
		{
			std::cout << " id [" << clients[j]->getNicknameId() << "] nick [" << clients[j]->getNickname() << "]" << std::endl;
		}
		std::cout << BLUE_BOLD << " Operators: " << END << std::endl;
		_channels[i]->getOperatorsName();
//		std::cout << BLUE_BOLD << std::setw(50) << std::setfill('-') << ""  << END << std::endl;
	}
}