#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>

int resolveHost(char * hostname , char* ip);

int main(int argc , char *argv[])
{
	if(argc <2)
	{
		printf("No hostname is given to resolve");
		exit(1);
	}
	char *hostname = argv[1];
	char ip[100];     resolveHost(hostname,ip);
	printf("%s resolved to %s" , hostname , ip);
}

int resolveHost(char * hostname , char* ip)
{
	struct hostent *hent;
	struct in_addr **addr_list;
	int i;

	if ( (hent = gethostbyname( hostname ) ) == NULL)
	{
		herror("gethostbyname error");
		return 1;
	}
	addr_list = (struct in_addr **) hent->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++)
	{
		std::cout << inet_ntoa(*addr_list[i]) << std::endl;
		strcpy(ip , inet_ntoa(*addr_list[i]));
		return 0;
	}
	return 1;
}

/*
 * 	if ( command[0].find_first_not_of("ltiko+-" ) != std::string::npos )
	{
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
			if (command[0][i] == 'l' )
			{
				if (status && command.size() < 1 || std::atoi( command[1].c_str() ) <= 0 )
				{
					RPL::ERR_NEEDMOREPARAMS( client, "MODE" );
					return true;
				}
				else if ( status )
				{
					channel->setLimitMode( status, std::stoi(command[1]) );
					command.erase( command.begin() + 1 );
				}
				else
					channel->setLimitMode( status, 0 );
			}
			else if ( command[0][i] == 'o' )
			{
					Client * other = channel->getClientByNickname( command[1] );
					if ( other )
					{
						if ( status )
							channel->addOperator( other->getNicknameId() );
						else
							channel->removeOperator( other->getNicknameId()  );
					}
					else
						RPL::ERR_NOSUCHNICK( client, command[1] );
					command.erase( command.begin() + 1 );
			}
			else if (command[0][i] == 'k' && status == true )
			{
				if (command.size() < 1 )
				{
					RPL::ERR_NEEDMOREPARAMS( client, "MODE" );
					return true;
				}
				else
				{
					if (status)
						channel->setKeyMode( status, command[1] );
					else
						channel->setKeyMode( status, "" );
				}
				command.erase( command.begin() + 1 );
			}
		}
	}
	return false;
 */