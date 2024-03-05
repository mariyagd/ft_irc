#include "MODE.hpp"

MODE::MODE( void ) {

	return;
}

MODE::~MODE( void ) {

	return;
}

bool mode_already_set( Channel * channel, char mode, bool status )
{
	if ( mode == 't' && channel->getTopicMode() == status )
		return true;
	else if ( mode == 'l' && channel->getLimitMode() == status )
		return true;
	else if ( mode == 'i' && channel->getInviteMode() == status )
		return true;
	else if ( mode == 'k' && channel->getKeyMode() == status )
		return true;
	return false;
}

void MODE::setChannelMode( Client & client, std::vector< std::string > & command, Channel * channel ) {

	bool status;
	std::string mode = command[0];
	size_t i = 0;
	size_t j = 1;

	for ( ; i < mode.size() ; ++i)
	{
		if ( mode[i] == '+' )
			status = true;
		else if ( mode[i] == '-' )
			status = false;
		else
		{
			switch ( mode[i] )
			{
				case 't':
				{
					if ( mode_already_set(channel, 't', status) )
						command[0].erase( i, 1 );
					else
						channel->setTopicMode( status );
					break;
				}
				case 'l':
				{
					if ( status && std::atoi( command[j].c_str() ) > 0 )
						channel->setLimitMode( status, std::stoi( command[j++] ) );
					else if ( !status && !mode_already_set( channel, 'l', status ) )
						channel->setLimitMode( status, 0 );
					else
					{
						std::cout << CYAN_BG << mode << END << std::endl;
						std::cout << CYAN_BG << mode[i] << END << std::endl;
						command.erase( command.begin() + j );
						command[0].erase( i, 1 );
						std::cout << CYAN_BG << mode << END << std::endl;
					}
					break;
				}
				case 'i':
				{
					if ( mode_already_set(channel, 'i', status) )
					{
						std::cout << CYAN_BG << mode << END << std::endl;
						std::cout << CYAN_BG << mode[i] << END << std::endl;
						command[0].erase( i, 1 );
						std::cout << CYAN_BG << mode << END << std::endl;
					}
					else
						channel->setInviteMode( status );
					break;
				}
				case 'k':
				{
					if ( status )
						channel->setKeyMode( status, command[j++] );
					else if ( !status && mode_already_set( channel, 'k', status ) )
						command[0].erase( i, 1 );
					else
					{
						channel->setKeyMode( status, "" );
						command[j++] = "*";
					}
					break;
				}
				case 'o':
				{
					Client * other = channel->getClientByNickname( command[j] );
					if ( other )
					{
						if ( status && channel->isClientIsOperator( other->getNicknameId() ) == -1 )
							channel->addOperator( other->getNicknameId( ) );
						else if ( !status )
							channel->removeOperator( other->getNicknameId( ) );
						++j;
					}
					else
					{
						RPL::ERR_NOSUCHNICK( client, command[j] );
						command.erase( command.begin( ) + j );
						command[0].erase( i, 1 );
					}
					break;
				}
				default:
				{
					RPL::ERR_UNKNOWNMODE( client, mode[i] );
					break;
				}
			}
		}
	}
	if (command[0].find_first_not_of("+-") == std::string::npos )
		command[0].clear();
	for ( size_t i = 1; i < command[0].size(); ++i )
	{
		if ( ( command[0][i - 1] == '-' || command[0][i - 1] == '+') && ( command[0][i] == '-' || command[0][i] == '+') )
			command[0].erase(i - 1, 1);
	}
}

bool MODE::errorsInModeCommand( std::vector< std::string > & command, Client & client, Channel * channel )
{
	(void)channel;
	bool status;
	size_t j = 1;

	if (command[0] == "b")
		return true;
	if ( channel->isClientIsOperator(client.getNicknameId()) == -1 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Client " << client.getNickname() << " is not an operator of channel " <<channel->getChannelName() << ". Can not change modes"  << END << std::endl;
		RPL::ERR_CHANOPRIVSNEEDED( client, channel->getChannelName() );
		return true;
	}
	if ( command[0].find_first_not_of("ltiko+-" ) != std::string::npos )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Unknown mode " << END << std::endl;
		RPL::ERR_UNKNOWNMODE(client, command[0][command[0].find_first_not_of("ltiko")]);
		return true;
	}
	for ( size_t i = 0; i < command[0].size(); ++i )
	{
		if (command[0][i] == '+')
			status = true;
		else if (command[0][i] == '-')
			status = false;
		else
		{
			if ( ( command[0][i] == 'l' || command[0][i] == 'o' || command[0][i] == 'k') && status )
			{
				if ( j++ >= command.size() )
				{
					std::cout << Get::Time() << RED_BOLD << " --- Not enough params " << END << std::endl;
					RPL::ERR_NEEDMOREPARAMS( client, "MODE" );
					return true;
				}
			}
		}
	}
	return false;
}

/*
 * MODE <target> [<modestring> [<mode arguments>...]]
 */
void	MODE::execute( std::vector< std::string > & command, Client & client, Server & server ) {

	( void ) command;
	( void ) client;
	( void ) server;

//	std::cout << Get::Time( ) << GREEN << " --- Processing MODE command" << END << std::endl;

	if ( command.size( ) == 1 )
	{
		RPL::ERR_NEEDMOREPARAMS( client, "MODE" );
	}
	else if ( command[1].find_first_of( "&#+!" ) == 0 )  // if channel mode is requested
	{
//		std::cout << Get::Time( ) << GREEN << " --- Processing MODE command for channels" << END << std::endl;

		Channel * channel = server.getChannelByName( command[1] );
		if ( !channel )
		{
			std::cout << Get::Time() << RED_BOLD << " --- Channel " << command[1] << " doesn't exist in this server"  << END << std::endl;
			RPL::ERR_NOSUCHCHANNEL( client, command[1] );
		}
		else if ( command.size( ) == 2 )
		{
			std::cout << Get::Time( ) << GREEN_BOLD << " --- Sending channel modes for channel " << command[1] << END << std::endl;
			RPL::RPL_CHANNELMODEIS( client, command[1], channel->getCurrentChannelModes( ) );
			RPL::RPL_CREATIONTIME( client, command[1], server.getChannelByName( command[1] )->getCreationTime( ) );
			return;
		}
		else
		{
			command.erase( command.begin( ), command.begin( ) + 2 ); // delete MODE and <target>
			if ( errorsInModeCommand( command, client, channel ) )
				return ;
			else
			{
				setChannelMode( client, command, channel );
				if (!command[0].empty())
				{
					std::cout << Get::Time( ) << GREEN_BOLD << " --- Set new channel modes " << command[1] << END << std::endl;
					RPL::INFORM_CHANNELMODE( client, channel->getChannelName(), command, channel->getAllClients() );
					channel->print_channels_info();
				}
			}
		}
	}
	else if ( command[1] == client.getNickname() && command.size() == 3)
	{
//		std::cout << Get::Time() << BOLD << " --- Send User MODE " << END << std::endl;
		RPL::ERR_UMODEUNKNOWNFLAG( client );
	}
	return ;
}