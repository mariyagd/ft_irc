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


Channel * ChannelMenager::getChannel(const std::string & channelName)
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
	std::cout << Get::Time() << MAGNETA_BG << BOLD << " --- Removing client " << MAGNETA_BG << MAGNETA2 << client->getNickname() << END << MAGNETA_BG  << BOLD << " from all channels on disconnection " << END << std::endl;

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
		std::cout << "Channel name: " << _channels[i]->getChannelName() << std::endl;
		std::vector< Client * > & clients =  _channels[i]->getAllClients();
		for ( size_t j = 0; j < clients.size(); ++j )
		{
			std::cout << "Client: " << " socket [" << clients[j]->getSocket() << "] nick [" << clients[j]->getNickname() << "]" << std::endl;
		}
		std::cout << "---------------------------" << std::endl;
	}
}