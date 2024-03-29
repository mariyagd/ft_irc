#include "MODE.hpp"

MODE::MODE( void ) {

	return;
}

MODE::~MODE( void ) {

	return;
}

/*
 * MODE <target> [<modestring> [<mode arguments>...]]
 */
void	MODE::execute( std::vector< std::string > & command, Client & client, Server & server ) {

//	std::cout << Get::Time( ) << GREEN << " --- Processing MODE command" << END << std::endl;

	if ( !client.isRegistered() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Client not registered" << END << std::endl;
		RPL::ERR_NOTREGISTERED( client );
		return;
	}
	if ( command.size( ) == 1 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Need more params: MODE <target> [<modestring> [<mode arguments>...]]" << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "MODE" );
		return;
	}

	Channel * channel = server.getChannelByName( command[1] );
	Client * client2 = server.getClientByNickname( command[1] );

	if ( !channel  && !client2 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Channel " << command[1] << " doesn't exist in this server"  << END << std::endl;
		RPL::ERR_NOSUCHCHANNEL( client, command[1] );
	}
	else if ( channel && !client2 )  // if channel mode is requested
	{
		if ( command.size( ) == 2 )
		{
			std::cout << Get::Time( ) << GREEN_BOLD << " --- Send channel modes" << END << std::endl;
			RPL::RPL_CHANNELMODEIS( client, command[1], channel->getCurrentChannelModes( ) );
			RPL::RPL_CREATIONTIME( client, command[1], server.getChannelByName( command[1] )->getCreationTime( ) );
			return;
		}
		else
		{
			if ( command[2] == "b")
			{
				RPL::RPL_ENDOFBANLIST(client, command[1]);
				return;
			}
			command.erase( command.begin( ), command.begin( ) + 2 ); // delete MODE and <target>
			if ( errorsInModeCommand( command, client, channel ) )
				return ;
			else
			{
				names_exist( command, client, channel, server );
				channel->print_channel_modes();
				setChannelMode( client, command, channel );
				if ( !command.empty() && !command[0].empty())
				{
					std::string new_modes;
					concatenate( command, 0, new_modes );
					std::cout << Get::Time( ) << GREEN_BOLD << " --- Set new channel modes " << new_modes << END << std::endl;
					channel->print_channel_modes();
					RPL::INFORM_CHANNELMODE( client, channel->getChannelName(), command, channel->getAllClients() );
					channel->print_channels_info();
				}
			}
		}
	}
	else
	{
		std::cout << Get::Time() << BOLD << " --- Server doesn't support user modes " << END << std::endl;
		RPL::ERR_UMODEUNKNOWNFLAG( client );
	}
	return ;
}


bool MODE::isValidNumber( const std::string & number ) const {

	if (number.find_first_not_of("0123456789") != std::string::npos )
	{
		std::cout << BOLD << "mode +l: invalid number" << END << std::endl;
		return false;
	}
	std::cout << BOLD << "mode +l: valid number" << END << std::endl;
	return true;
}


bool MODE::mode_already_set( Channel * channel, std::vector< std::string > & command, size_t & i, size_t &j, bool status )
{
	switch (command[0][i])
	{
		case 't' :
		{
			if ( channel->getTopicMode() == status )
			{
				std::cout << BOLD << "mode " << ( status == true ? "+" : "-") << "t: already set" << END << std::endl;
				command[0].erase( command[0].begin() + i);
				--i;
				return true;
			}
			return false;
		}
		case 'l':
		{
			if ( !status && channel->getLimitMode() == status )
			{
				std::cout << BOLD << "mode " << ( status == true ? "+" : "-") << "l: already set " << i << " size " << command[0].size() << END << std::endl;
				command[0].erase( command[0].begin() + i  );
				--i;
				return true;
			}
			if ( status && channel->getLimitMode() == status && isValidNumber(command[j]) && channel->getLimit() == std::atoi( command[j].c_str() ) && std::stoi( command[j] ) > 0)
			{
				std::cout << BOLD << "mode " << ( status == true ? "+" : "-") << "l: already set" << END << std::endl;
				j++;
				return true;
			}
			if ( status && (!isValidNumber(command[j]) || std::atoi( command[j].c_str() ) <= 0 ))
			{
				std::cout << BOLD << "mode " << ( status == true ? "+" : "-") << "l: invalid number" << END << std::endl;
				command[0].erase( command[0].begin() + i-- );
				command.erase( command.begin() + j );
				return true;
			}
			return false;
		}
		case 'i':
		{
			if ( channel->getInviteMode() == status )
			{
				std::cout << BOLD << "mode " << ( status == true ? "+" : "-") << "i: already set" << END << std::endl;
				command[0].erase( command[0].begin() + i-- );
				return true;
			}
			return false;
		}
		case 'k':
		{
			if ( status && channel->getKeyMode() == status )
			{
				std::cout << BOLD << "mode " << ( status == true ? "+" : "-") << "k: already set" << END << std::endl;
					++j;
				return true;
			}
			else if ( !status && channel->getKeyMode() == status )
			{
				std::cout << BOLD << "mode " << ( status == true ? "+" : "-") << "k: already set" << END << std::endl;
				command[0].erase( command[0].begin() + i-- );
				if ( j < command.size() )
					command.erase( command.begin() + j );
				return true;
			}
			return false;
		}
		case 'o':
		{
			Client * other = channel->getClientByNickname( command[j] );
			if ( other )
			{
				if ( status && channel->isClientIsOperator( other->getNicknameId() ) != -1 )
				{
					std::cout << BOLD << "mode " << ( status == true ? "+" : "-") << "o " << command[j] << " already set" << END << std::endl;
					++j;
					return true;
				}
				if ( !status && channel->isClientIsOperator( other->getNicknameId() ) == -1 )
				{
					std::cout << BOLD << "mode " << ( status == true ? "+" : "-") << "o " << command[j] << " already set" << END << std::endl;
					++j;
					return true;
				}
			}
		}
	}
	return false;
}

void MODE::setChannelMode( Client & client, std::vector< std::string > & command, Channel * channel ) {

	(void)client;
	bool status = true;
	std::string & mode = command[0];
	size_t i = 0;
	size_t j = 1;

	for ( ; i < mode.size() ; ++i)
	{
		if ( i >= mode.size() || i < 0 || !mode[i])
			return;
		if ( mode[i] == '+' )
			status = true;
		else if ( mode[i] == '-' )
			status = false;
		else if ( !mode_already_set( channel, command, i, j, status ) )
		{
			switch ( mode[i] )
			{
				case 't':
				{
					std::cout << GREEN_BOLD << "set new mode: " << ( status == true ? "+" : "-") << "t" << END << std::endl;
					channel->setTopicMode( status );
					break;
				}
				case 'l':
				{
					if ( status )
					{
						std::cout << GREEN_BOLD << "set new mode: " << ( status == true ? "+" : "-") << "l " << std::atoi( command[j].c_str() ) << END << std::endl;
						channel->setLimitMode( status, std::atoi( command[j++].c_str() ) );
					}
					else if ( !status )
					{
						std::cout << GREEN_BOLD << "set new mode: " << ( status == true ? "+" : "-") << "l" << END << std::endl;
						channel->setLimitMode( status, 0 );
					}
					break;
				}
				case 'i':
				{
					std::cout << GREEN_BOLD << "set new mode: " << ( status == true ? "+" : "-") << "i" << END << std::endl;
					channel->setInviteMode( status );
					break;
				}
				case 'k':
				{
					if ( status )
					{
						std::cout << GREEN_BOLD << "set new mode: " << ( status == true ? "+" : "-") << "k " << command[j] << END << std::endl;
						channel->setKeyMode( status, command[j++] );
					}
					else
					{
						channel->setKeyMode( status, "" );
//						if ( j < command.size() )
							command[j++] = "*";
//						else
//							command.insert( command.begin() + j++, "*" );
						std::cout << GREEN_BOLD << "set new mode: " << ( status == true ? "+" : "-") << "k " << command[j - 1] << END << std::endl;
					}
					break;
				}
				case 'o':
				{
					Client * other = channel->getClientByNickname( command[j] );
					if ( other )
					{
						if ( status && channel->isClientIsOperator( other->getNicknameId( ) ) == -1 )
						{
							std::cout << GREEN_BOLD << "set new mode: " << ( status == true ? "+" : "-") << "o " << command[j] << END << std::endl;
							channel->addOperator( other->getNicknameId( ) );
						}
						else if ( !status && channel->isClientIsOperator( other->getNicknameId( ) ) != -1 )
						{
							std::cout << GREEN_BOLD << "set new mode: " << ( status == true ? "+" : "-") << "o " << command[j] << END << std::endl;
							channel->removeOperator( other->getNicknameId( ) );
						}
						++j;
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
	remove_signs( command );
	remove_extras( command );
}

void	MODE::remove_signs( std::vector< std::string > & command )
{
	// if command size == 1 and has only a sign -> clear
	if (command.empty() || ( command[0].size() == 1 && ( command[0][0] == '-' || command[0][0] == '+') ) )
	{
		command[0].clear();
		return;
	}

	// remove extra signs from begining
	for ( size_t i = 0; i + 1 < command[0].size(); ++i )
	{
		if ( ( command[0][i] == '-' || command[0][i] == '+') && ( command[0][i + 1] == '-' || command[0][i + 1] == '+') )
			command[0].erase(i, 1);
	}

	if (command.empty() )
		return;

	// remove extra signs from the end
	std::string::reverse_iterator rit = command[0].rbegin();
	while ( rit != command[0].rend() && ( *rit == '-' || *rit == '+' ) )
	{
		command[0].erase(( rit + 1 ).base());
		++rit;
	}
}

void MODE::remove_extras( std::vector< std::string > & command )
{
	// remove extra params
	bool status = true;
	size_t j = 1;

	if (command.empty() )
		return;
	for ( size_t i = 0; i < command[0].size(); ++i )
	{
		if (command[0][i] == '+')
			status = true;
		else if (command[0][i] == '-')
			status = false;
		else
		{
			if ( ( command[0][i] == 'l' || command[0][i] == 'o' || command[0][i] == 'k') && status )
				++j;
			if ( !status && command[0][i] == 'k' )
				++j;
			if (!status && command[0][i] == 'o')
				++j;
		}
	}
	if ( j < command.size() )
		command.erase( command.begin() + j, command.end() );
}

bool MODE::errorsInModeCommand( std::vector< std::string > & command, Client & client, Channel * channel )
{
	(void)channel;
	bool status = true;
	size_t j = 1;
	size_t k = 0;
	size_t c = 1;

	if ( channel->isClientIsOperator(client.getNicknameId()) == -1 )
	{
		std::cout << Get::Time() << RED_BOLD << " --- " << client.getNickname() << " is not an operator. Can not change modes"  << END << std::endl;
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
			if ( status && ( command[0][i] == 'l' || command[0][i] == 'o' || command[0][i] == 'k')  )
			{
				if ( command.size() >= j)
					++j;
				++c;
			}
			else if ( !status && command[0][i] == 'o' )
			{
				if ( command.size() >= j)
					++j;
				++c;
			}
			else if ( !status && command[0][i] == 'k' )
				++k;
		}
	}
	if ( c > command.size() )
	{
		std::cout << Get::Time() << RED_BOLD << " --- Not enough params " << END << std::endl;
		RPL::ERR_NEEDMOREPARAMS( client, "MODE" );
		return true;
	}
	else if ( j == command.size() && k != 0 )
	{
		j = 1;
		for ( size_t i = 0; i < command[0].size(); ++i )
		{
			if (command[0][i] == '+')
				status = true;
			else if (command[0][i] == '-')
				status = false;
			else
			{
				if ( status && ( command[0][i] == 'l' || command[0][i] == 'o' || command[0][i] == 'k')  )
					++j;
				else if ( !status && command[0][i] == 'o' )
					++j;
				else if ( !status && command[0][i] == 'k' )
				{
					command.insert( command.begin() + j, "*" );
					++j;
				}
			}
		}
	}

	return false;
}

void MODE::names_exist( std::vector< std::string > & command, Client & client, Channel * channel, Server & server )
{
	(void)channel;
	bool status = true;
	size_t j = 1;

	if (command[0][0] != '+' && command[0][0] != '-') //for nc, treat MODE #channelname i
		command[0] = "+" + command[0];
	for ( size_t i = 0; i < command[0].size(); ++i )
	{
		if (command[0][i] == '+')
			status = true;
		else if (command[0][i] == '-')
			status = false;
		else
		{
			if ( ( status && (command[0][i] == 'l' || command[0][i] == 'k') ) || ( !status && command[0][i] == 'k') )
				++j;
			else if ( command[0][i] == 'o' )
			{
				if ( command.size() >= j && server.getClientByNickname( command[j] ) == nullptr )
				{
					std::cout << Get::Time() << RED_BOLD << " --- " << command[j] << " doesn't exist" << END << std::endl;
					RPL::ERR_NOSUCHNICK( client, command[j] );
					command.erase( command.begin() + j );
					command[0].erase( command[0].begin() + i-- );
				}
				else if ( command.size() >= j && channel->getClientByNickname( command[j] ) == nullptr )
				{
					std::cout << Get::Time() << RED_BOLD << " --- " << command[j] << " is not in channel " << channel->getChannelName() << END << std::endl;
					RPL::ERR_USERNOTINCHANNEL( client, command[j], channel->getChannelName() );
					command.erase( command.begin() + j );
					command[0].erase( command[0].begin() + i-- );
				}
				else
					++j;
			}
		}
	}
	return;
}
