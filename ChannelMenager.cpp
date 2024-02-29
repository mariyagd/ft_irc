#include "ChannelMenager.hpp"


// Public Constructor--------------------------------------------------------------------------------------------------

ChannelMenager::ChannelMenager( void ) : _channels() {

	return ;
}

//ChannelMenager::ChannelMenager( Server & server ) {
//
//	return ;
//}

// Destructor-----------------------------------------------------------------------------------------------------------

ChannelMenager::~ChannelMenager( void ) {

	return ;
}


// Channels--------------------------------------------------------------------------------------------------------------


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
			break; // Return pointer to the Channel object
	}
	return _channels[i]; // Return dereferenced pointer as alias
}

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

void ChannelMenager::addOperator( int id )
{
	std::cout << Get::Time() << " --- Client " << id << " is now an operator" << std::endl;
	_operators.push_back( id );
	 std::cout << "Added operator with ID: " <<id << " to channel" << std::endl;

}


int ChannelMenager::getOperatorSocket() const
{
	if (_operators.size() > 0)
	{
		return _operators[0];
	}
	return -1;
}

//
//Channel* ChannelMenager::getChannel(const std::string& channelName)
//{
//	// Find the channel in the vector
//	for (size_t i = 0; i < _channels.size(); ++i) {
//		if (_channels[i]->getChannelName() == channelName) {
//			return _channels[i]; // Return pointer to the Channel object
//		}
//	}
//	return nullptr; // Return nullptr if channel not found
//}
//
//void ChannelMenager:: addClientToChannel(const std::string& channelname, Client* client)
//{
//	for (size_t i = 0; i < _channels.size(); ++i)
//	{
//		if (_channels[i]->getChannelName() == channelname)
//		{
//			_channels[i]->addClient(client);
//			std::cout<<"Added client "<< client->getNickname()<< " to channel "<<channelname<<std::endl;
//			return;
//		}
//	}
//	std::cerr<< "channel "<< channelname <<" not found. "<<std::endl;
//}
//
//void ChannelMenager::addChannel(Channel *channel)
//{
//	_channels.push_back(channel);
//}
//
//void	ChannelMenager::sendToChannel(std::string&kickMessage, std::string& channel)
//{
//	// Get the list of clients in the specified channel
//	Channel* ch = getChannel(channel);
//	if (ch) {
//		std::vector<Client*> clients = ch->getClient();
//
//		// Send the kick message to each client in the channel
//		for (size_t i = 0; i < clients.size(); ++i) {
//			clients[i]->sendMessage(kickMessage);
//		}
//	}

