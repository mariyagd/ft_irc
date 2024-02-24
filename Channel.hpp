
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
# include<vector>

class Server;
class Client;

class Channel {

private:
	std::string					_name;
	std::vector<Client>			_clients;
	std::vector<int>			_operators;
public:
	Channel( void );
	Channel(const std::string& channelName);
	Channel( Channel const & src );
	Channel & operator=( Channel const & rhs );
	std::string getName();
	std::vector<Client>	getClient();
	void setName(std::string name);
	void addClient(Client& client);
	bool isClientInChannel(const std::string& nickname) const;
	~Channel( void );
	static void	process_channel( std::string &msg, int i, Server &server );


};

# endif