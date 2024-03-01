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

	std::vector< Channel * >			_channels;
	~ChannelMenager( void );

	std::vector< Channel * > &	getAllChannels( void );
	Channel *					createChannel( std::string name );
	Channel	*					getChannel( const std::string& channelName) ;

	bool						channelExists(const std::string& channelName);

	void						removeClientFromAllChannels(Client* client);

	void						print_channels_info();

};

#endif