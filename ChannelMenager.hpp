#ifndef CHANNELMENAGER_HPP
# define CHANNELMENAGER_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include <vector>
# include <string>


class Channel;

class ChannelMenager {

protected:
	ChannelMenager( void );
public:
//	ChannelMenager( Server & server );

	std::vector< Channel * >			_channels;
	std::vector< int >					_operators;
	~ChannelMenager( void );

	std::vector< Channel * > &	getAllChannels( void );
	Channel *					createChannel( std::string name );
	Channel	*					getChannel( const std::string& channelName) ;
	bool						channelExists(const std::string& channelName);

	void						removeClientFromAllChannels(Client* client);
	void 						addOperator( int id );
	//getOperatorSocket
	int getOperatorSocket()const;

//	void 						addClientToChannel(const std::string& channelname, Client* client);
//	void						sendToChannel(std::string&kickMessage, std::string& channel);
	void	print_channels_info();

};

#endif