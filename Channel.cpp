#include "Channel.hpp"

// Coplien's form------------------------------------------------------------------------------------------------------

Channel::Channel(void) : _name(""), _clients(), _operators(), _invited_list(), _creation_time(0), limitMode(false), inviteMode(false), topicMode(true), keyMode(false), _topicSetter(0), _topicTime(0){

	return;
}

Channel::Channel(const std::string &channelName) : _name(channelName), _clients(), _operators(), _invited_list(), _creation_time(0), limitMode(false), inviteMode(false), topicMode(true), keyMode(false),  _topicSetter(0), _topicTime(0) {

	struct timeval tp;
	if (gettimeofday(&tp, NULL) < 0)
	{
		std::cerr << Get::Time() << RED << " --- gettimeofday() failed" << strerror(errno) << END << std::endl;
		return;
	}

	_creation_time = (tp.tv_sec) + (tp.tv_usec / 1000);
}
Channel::Channel(Channel const &src) {
	(void)src;
	return;
}

Channel &Channel::operator=( Channel const &rhs ) {

	(void)rhs;
	return *this;
}

Channel::~Channel( void ) {
	
	return;
}

// Getters --------------------------------------------------------------------------------------------------------------

const std::string &Channel::getChannelName( void ) {
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

std::vector<Client *> &Channel::getAllClients(void) {

	return this->_clients;
}

std::set<int> Channel::getAllClientsSockets(void) {

	std::set<int> sockets;

	for (size_t i = 0; i < _clients.size(); ++i)
	{
		sockets.insert(_clients[i]->getSocket());
	}
	return sockets;
}

const long &Channel::getCreationTime( void ) const {

	return this->_creation_time;
}

const std::vector< std::string >	Channel::getOperatorsName( void ) const {

	std::vector< std::string > operatorNames;

	for ( size_t i = 0; i < _operators.size(); ++i )
	{
		for ( size_t j = 0; j < _clients.size(); ++j )
		{
			if ( _operators[i] == _clients[j]->getNicknameId( ) )
			{
				operatorNames.push_back( _clients[j]->getNickname() );
				break;
			}
		}
	}
	return operatorNames;
}

const std::vector< int > &Channel::getOperatorsID( void ) const {

	return _operators;
}

////Bool ------------------------------------------------------------------------------------------------------------------

// Check if clientId is present in _operators vector

int Channel::isClientIsOperator(int clientId) const {

	if (std::find(_operators.begin(), _operators.end(), clientId) != _operators.end())
		return clientId;
	return -1;
}

bool Channel::clientIsInChannel(const Client *client) const
{

	for (std::vector<Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}

Client * Channel::getClientByNickname( const std::string & nickname ) const {

	for ( size_t i = 0; i < _clients.size(); ++i )
	{
		if ( _clients[i]->getNickname() == nickname )
			return _clients[i];
	}
	return nullptr;

}


//// Method to add a client to the channel---------------------------------------------------------------------------------
void Channel::addClient(Client &client) {

	_clients.push_back( &client );
	client.addChannel( this );
}

void Channel::removeClient( const std::string &nickname ) {

	std::cout << Get::Time() << GREEN_BOLD << " --- " << nickname << " removed from channel " << this->getChannelName() << END << std::endl;

	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
		{
			(*it)->removeChannel(this);
			_clients.erase(it); // erase returns the next valid iterator
			return;
		}
	}
	return;
}

int Channel::getOperatorSocket( int operator_id ) const {

	for ( size_t i = 0; i < _clients.size(); ++i )
	{
		if ( _clients[i]->getNicknameId() == operator_id )
			return _clients[i]->getSocket();
	}
	return -1;
}

void Channel::addOperator( int id ) {
	
	std::cout << Get::Time() << " --- Client ID" << id << " is now an operator" << std::endl;
	int i = id;
	_operators.push_back( i );

}

void Channel::removeOperator( int id ) {

	std::vector< int >::iterator it = std::find( _operators.begin(), _operators.end(), id );
	if ( it != _operators.end() )
	{
		std::cout << Get::Time() << " --- Client ID" << id << " is no longer an operator" << std::endl;
		_operators.erase( it );
	}
}


void Channel::addInvited( int id ) {

	_invited_list.push_back( id );
}

void Channel::removeInvited( int id ) {

	std::vector< int >::iterator it = std::find( _invited_list.begin(), _invited_list.end(), id );
	if ( it != _invited_list.end() )
		_invited_list.erase( it );
}

//getMembersName
const  std::vector< std::string >	Channel::getMembersName( void ) const
{
	std::vector< std::string > membersName;

	for ( size_t i = 0; i < _clients.size(); ++i )
	{
		membersName.push_back( _clients[i]->getNickname() );
	}
	return membersName;

}


// Modes setters ---------------------------------------------------------------------------------------------------------------

void	Channel::setLimitMode( bool mode, const int & limit ) {

	limitMode = mode;
	_limit = limit;
}

void	Channel::setInviteMode( bool mode ) {

	inviteMode = mode;
}

void	Channel::setTopicMode( bool mode ) {

	topicMode = mode;
}

void	Channel::setKeyMode( bool mode, const std::string & key ) {

	keyMode = mode;
	_key = key;
}

// Modes getters ---------------------------------------------------------------------------------------------------------------

std::string Channel::getCurrentChannelModes( void ) const {

	std::string modes = "+";
	std::string append;


	if ( limitMode )
	{
		modes += "l";
		append += " " + std::to_string(_limit);
	}
	if ( topicMode )
		modes += "t";
	if ( keyMode )
	{
		modes += "k";
		append += " " + _key;
	}
	if ( inviteMode )
		modes += "i";

	return modes + append;
}

bool	Channel::getLimitMode( void ) const {

	return limitMode;
}

bool	Channel::getInviteMode( void ) const {

	return inviteMode;
}

bool	Channel::getTopicMode( void ) const {

	return topicMode;
}

bool	Channel::getKeyMode( void ) const {

	return keyMode;
}


const std::string & Channel::getKey( void ) const {

	return _key;
}

const std::string & Channel::getTopic( void ) const {

	return _topic;
}

int Channel::getLimit( void ) const {

	return _limit;
}

Client * Channel::getTopicSetter( void ) const {

	for ( size_t i = 0; i < _clients.size(); ++i )
	{
		if ( _clients[i]->getNicknameId() == _topicSetter )
			return _clients[i];
	}
	return nullptr;
}

long Channel::getTopicCreationTime( void ) const {

	return _topicTime;
}

void Channel::setTopic( const std::string & topic, int setter_id ) {

	_topic = topic;
	_topicSetter = setter_id;

	struct timeval tp;
	if (gettimeofday(&tp, NULL) < 0)
	{
		std::cerr << Get::Time() << RED << " --- gettimeofday() failed" << strerror(errno) << END << std::endl;
		return;
	}

	_topicTime = (tp.tv_sec) + (tp.tv_usec / 1000);
}

bool Channel::isClientInvited( int id ) const {

	std::vector< int >::const_iterator it = std::find( _invited_list.begin(), _invited_list.end(), id );
	if ( it != _invited_list.end() )
		return true;
	return false;
}

bool Channel::operator==( const Channel & rhs ) const {

	if ( this->_name == rhs._name )
		return true;
	return false;
}

void	Channel::print_channels_info() const {


		std::cout << std::setfill(' ');
//		std::cout << BLUE_BOLD << std::setw(50) << std::setfill('-') << "" << END << std::endl;
		std::cout << std::setw(15) << std::left << BLUE_BOLD << " Channel : " << END << "[" << _name << "]" << std::endl;
		std::cout << std::setw(15) << std::left << BLUE_BOLD << " Modes :   "  << END << "[" << this->getCurrentChannelModes() << "]" << std::endl;
		std::cout << std::setw(15) << std::left << BLUE_BOLD << "" << " Clients:  " << END << std::endl;
		for ( size_t j = 0; j < _clients.size(); ++j )
		{
			std::cout << std::setw(17) << std::left << "" << _clients[j]->getNickname() << std::endl;
		}
		std::cout << std::setw(15) << std::left << BLUE_BOLD << "" << " Operators: " << END << std::endl;

		for ( size_t i = 0; i < _operators.size(); ++i )
		{
			for ( size_t j = 0; j < _clients.size(); ++j )
			{
				if ( _operators[i] == _clients[j]->getNicknameId( ) )
				{
					std::cout << std::setw(17) << std::left << "" << _clients[j]->getNickname() << std::endl;
					break;
				}
			}
		}
//		std::cout << BLUE_BOLD << std::setw(50) << std::setfill('-') << ""  << END << std::endl;
}

void Channel::print_channel_modes( ) const {

	std::cout << BLUE_BOLD << "Current Modes : "  << END << "[" << this->getCurrentChannelModes() << "]" << std::endl;
}