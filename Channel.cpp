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

std::vector<Client*>	Channel::getClient()
{
	return this->_clients;
}


 // Method to add a client to the channel
void Channel::addClient(Client* client) {
	_clients.push_back(client);
}

// // Method to check if a client is in the channel
bool Channel::isClientInChannel(const std::string& nickname) const {
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i]->getnickname() == nickname) {
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


Channel* Channel::createChannel(std::string& channelname, Server &server)
{
	Channel *channel = new Channel(); //create an instance of channel
	channel->setName(channelname);
	server.addChannel(channel);
	return channel;
}


// bool	Channel::isOperator(const std::string& nickname)
// {

// 	return true;
// }

std::vector<int> Channel::getOperator()
{	return this->_operators;
}
int	Channel::getNicknameId(std::string nickname)
{
	 // Simple hash function to convert the string to an integer
    int hash = 0;
    for (size_t i = 0; i < nickname.length(); ++i) {
        hash = (hash * 31) + nickname[i]; // A simple hash function
    }
    return hash;
}
bool Channel::isClientIsOperator(const std::string& nickname)
{
	 int nicknameId = getNicknameId(nickname);
	std::vector<int> ::iterator it; // = _operators.begin();
	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == nicknameId)
		{
			return true;
		}	
	}
	return false;
}

void	Channel::removeUser(std::string &nickname)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->getNickname() == nickname)
        {
            _clients.erase(it); // erase returns the next valid iterator
            break;
        }
    }
}




// ---------------------------------------------------------------------------------------------------------------------
