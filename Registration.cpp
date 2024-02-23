#include "Registration.hpp"

// Constructor ------------------------------------------------------------------------------------------------------------

Registration::Registration( void ) {

	return;
}

// Destructor -------------------------------------------------------------------------------------------------------------

Registration::~Registration( void ) {

	return;
}

// Member functions -------------------------------------------------------------------------------------------------------

void Registration::splitMsgIntoTokens( std::string &msg, std::vector< std::string > &tokens ) {

	std::istringstream iss(msg);
	while ( !iss.eof() )
	{
		std::string temp;
		iss >> temp;
		tokens.push_back(temp);
	}
	return;
}

bool Registration::ValidPassword( std::string &msg, Client & client, Server & server ) {

	std::string						realname;
	std::string						password;
	std::vector< std::string >		tokens;

	splitMsgIntoTokens(msg, tokens);

	size_t j = 0;
	if ( tokens[j] == "PASS" )
	{
		password = tokens[++j];
		++j;
	}
	if ( tokens[j] == "NICK" )
	{
		client.setNickname(tokens[++j]);
		++j;
	}
	if ( tokens[j] == "USER" )
	{
		client.setUsername(tokens[++j]);
		client.setHostname(tokens[++j]);
		client.setServname(tokens[++j]);
		for ( j += 1; j < tokens.size(); ++j )
		{
			realname += tokens[j] + " ";
		}
		client.setRealname(realname);
	}
	if ( client.getGavePassword() == true )
	{
		return true;
	}
	if ( password == server.getPassword() )
	{
		client.setGavePassword( true );
		return true;
	}
	return false;
}

bool Registration::ClientReadyToRegister( Client const & client ) {

	if ( client.getGavePassword() && !client.getNickname().empty() && !client.getUsername().empty() && !client.getHostname().empty() && !client.getServname().empty() && !client.getRealname().empty() )
		return true;
	else
		return false;
}