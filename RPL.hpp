#ifndef RPL_HPP
# define RPL_HPP

# include "Irc.hpp"
# include "Server.hpp"
# include "Client.hpp"

#define RPL_WELCOME( server, client ) RPL::RPL_WELCOME( server, client )

class RPL  {

private:
	RPL( void );
public:
	~RPL( void );

	static char * RPL_WELCOME( Server const & server, Client & client )
	{
		std::string server_name = server.getServerName();
		std::string nickname = client.getNickname();
		std::string user = client.getUsername();
		std::string host = client.getHostname();

		std::string message = "@time=" + PrintTime::printTime() + " :" + _server_name + " 001 " + _nickname + " Welcome to the Internet Relay Network " + _user "\r\n";
	}

	static char * RPL_NICK( Server const & server, Client & client, std::string msg )
	{
		std::string message = "@time=" + PrintTime::printTime() + _user + "NICK" + msg + "\r\n";
	}

	static char * ERR_NICKNAMEINUSE( Server const & server, Client & client, std::string msg )
	{
		std::string message = "@time=" + PrintTime::printTime() + " :" + _server_name + " 433 " + _nickname + " " + msg + "\r\n";
	}

};

#endif