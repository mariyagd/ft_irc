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
//	void						deleteChannel( Channel * );
	void						deleteAllChannels( void );
//	Channel	*					getChannel( const std::string& channelName) ;
	Channel	*					getChannelByName( const std::string& channelName) ;

	bool						mode_already_set(Channel * channel, const char & mode, bool status );
	bool						channelExists(const std::string& channelName);

	void						removeClientFromAllChannels(Client* client);

	void						print_channels_info();

};

#endif