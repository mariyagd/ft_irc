#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
# include<vector>

class Server;
class Client;

class Channel {

private:
	std::string					_name;
	std::vector< Client * >		_clients;
//	std::vector< Client & >		_operators;
	std::vector<int>			_operators;
	//modes
	//topic
public:
	Channel(void);
	Channel(const std::string& channelName);
	Channel(Channel const & src);
	Channel & operator=(Channel const & rhs);
	~Channel( void );

	std::string getName();
	std::vector<Client*>	getClient();

	int	getNicknameId(std::string nickname);
	void setName( std::string name );
	void addClient(Client* client);
	bool isClientInChannel(const std::string& nickname) const;
	bool	isOperator(const std::string& nickname);
	std::vector<int> getOperator();
	bool isClientIsOperator(const std::string& nickname);
//	Channel* createChannel(std::string& channelname, Server &server);
	void	removeUser(std::string &nickname);

};

# endif