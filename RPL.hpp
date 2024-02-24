#ifndef RPL_HPP
# define RPL_HPP

# include "Server.hpp"

class Server;
class Client;

class RPL  {

private:
	RPL( void );

public:
	~RPL( void );

	static std::string RPL_WELCOME( Client const & client, Server const & server );
	static std::string RPL_YOURHOST( Client const & client, Server const & server );
	static std::string RPL_CREATED( Client const & client, Server const & server );
	static std::string RPL_MYINFO( Client const & client, Server const & server );

	static std::string ERR_NICKNAMEINUSE( Client const & client, Server const & server, std::string & wantedNickname );
	static std::string ERR_ERRONEUSNICKNAME( Client const & client, Server const & server );
	static std::string RPL_SAVENICK( Client const & client, Server const & server);

	static std::string RPL_NICK( Client const & client, Server const & server, std::string & newNickname );

	static std::string RPL_WHOISUSER( Client const & client, Server const & server );
};

#endif