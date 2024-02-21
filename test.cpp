#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int	 main(void )
{
	char msg_str[84] = "PASS 0000 \nNICK mdanchev\nUSER mdanchev mdanchev localhost :Mariya Dancheva\r\n\0";
	std::string msg(msg_str);
	std::cout << msg << std::endl;

	size_t pos = 0;
	size_t j = 0;
	std::string nick;
	std::string username;
	std::string hostname;
	std::string servname;
	std::string realname;
	std::string password;

	std::vector< std::string > tokens;

	std::istringstream iss(msg);
	while ( !iss.eof() )
	{
		std::string temp;
		iss >> temp;
		tokens.push_back(temp);
	}
	if ( tokens[0] == "PASS" )
	{
		password = tokens[1];
	}
	if ( tokens[2] == "NICK" )
	{
		nick = tokens[3];
	}
	if ( tokens[4] == "USER" )
	{
		username = tokens[5];
		hostname = tokens[6];
		servname = tokens[7];
		realname = tokens[8];
		while ( !iss.eof() )
		{
			std::string temp;
			iss >> temp;
			realname += " " + temp;
		}
	}




//	pos = msg.find("USER");
//	if (pos == 0)
//	{
//		for ( j = 5; !std::isspace(msg[j]) && j < msg.size(); ++j );
//		username = msg.substr(pos + 5, j );
//		msg.erase(pos, j- pos);
//		for ( j = j + 1; !std::isspace(msg[j]) && j < msg.size(); ++j );
//		hostname = msg.substr(pos + 5, j );
//		msg.erase(0, j);
//		for ( j = j + 1; !std::isspace(msg[j]) && j < msg.size(); ++j );
//		servname = msg.substr(pos + 5, j );
//		msg.erase(0, j);
//		pos = msg.find(":");
//		realname = msg.substr(pos + 1, msg.size() - pos - 1);
//		msg.erase(pos, msg.size() - pos - 1);
//	}
//
//	std::cout << "msg " << msg << std::endl;
//	std::cout << "nick " << nick << std::endl;
//	std::cout << "username " << username << std::endl;
//	std::cout << "hostname " << hostname << std::endl;
//	std::cout << "servname " << servname << std::endl;
//	std::cout << "realname " << realname << std::endl;
}