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

std::string Channel::getName()
{
	return this->_name;
}

std::vector<Client>	Channel::getClient()
{
	return this->_clients;
}


 // Method to add a client to the channel
void Channel::addClient(Client& client) {
	_clients.push_back(client);
}

// // Method to check if a client is in the channel
bool Channel::isClientInChannel(const std::string& nickname) const {
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i].getnickname() == nickname) {
			return true;
		}
	}
	return false;
}

void Channel::setName(std::string name)
{
	this->_name = name;
}

Channel::~Channel( void ) {
	return ;
}

// ---------------------------------------------------------------------------------------------------------------------
