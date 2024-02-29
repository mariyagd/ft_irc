#include "Channel.hpp"

// Coplien's form------------------------------------------------------------------------------------------------------

Channel::Channel(void) : _name(""), _clients(), _operators(), _creation_time(0)
{
	std::cout << "Create a channel instance" << std::endl;
	// this->setName("");
	std::cout << "Channel instanced created" << std::endl;
	return;
}

Channel::Channel(const std::string &channelName) : _name(channelName), _clients(), _operators(), _creation_time(0)
{

	struct timeval tp;
	if (gettimeofday(&tp, NULL) < 0)
	{
		std::cerr << RED << " --- gettimeofday() failed" << strerror(errno) << END << std::endl;
		return;
	}

	_creation_time = (tp.tv_sec) + (tp.tv_usec / 1000);
}
Channel::Channel(Channel const &src)
{

	(void)src;
	return;
}

Channel &Channel::operator=(Channel const &rhs)
{

	(void)rhs;
	return *this;
}

Channel::~Channel(void)
{
	return;
}

// Getters --------------------------------------------------------------------------------------------------------------

const std::string &Channel::getChannelName(void)
{
	return this->_name;
}

// Client & 	Channel::getClient( const std::string & nickname )
//{
//	std::vector< Client * >::const_iterator it = _clients.begin();
//	std::vector< Client * >::const_iterator ite = _clients.end();
//
//	for ( ; it != ite; ++it )
//	{
//		if ( (*it)->getNickname() == nickname )
//			return *(*it);
//	}
// }

std::vector<Client *> &Channel::getAllClients(void)
{

	return this->_clients;
}

const long &Channel::getCreationTime(void) const
{

	return this->_creation_time;
}


////Bool ------------------------------------------------------------------------------------------------------------------
//

//
int Channel::isClientIsOperator(int clientId) const
{
	// Check if clientId is present in _operators vector
	if (std::find(_operators.begin(), _operators.end(), clientId) != _operators.end())
	{
		std::cout << "Checking operator status for client ID: " << clientId << " is an operator" << std::endl;
		return clientId;
	}
	std::cout << "Client " << clientId << " is not an operator" << std::endl;
	return -1;
}

bool Channel::isClientInChannel(const Client *client) const
{

	for (std::vector<Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}

//// Method to add a client to the channel---------------------------------------------------------------------------------
void Channel::addClient(Client &client)
{

	_clients.push_back(&client);
}

void Channel::removeClient(const std::string &nickname)
{

	std::cout << Get::Time() << MAGNETA_BOLD << " --- Removing client " << BOLD << nickname << END << MAGNETA_BOLD << " from channel " << BOLD << this->getChannelName() << END << std::endl;

	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
		{
			_clients.erase(it); // erase returns the next valid iterator
			break;
		}
	}
}
