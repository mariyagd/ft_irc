#include "RPL.hpp"

// Private constructor----------------------------------------------------------------------------------------------------

RPL::RPL( void ) {

	return;
}

// Destructor------------------------------------------------------------------------------------------------------------

RPL::~RPL( void ) {

	return;
}

// Upon registration------------------------------------------------------------------------------------------------------


std::string RPL::RPL_WELCOME( const Client & client, const Server & server ) {

	std::string message = "@time=" + PrintTime::printTime();
	message += ":" + std::string(server.getServerName()) + " 001 " + client.getNickname();
	message += " Welcome to the Internet Relay Network ";
	message += client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + "\r\n";;
	return message;
}

std::string RPL::RPL_YOURHOST( Client const & client, Server const & server ) {

	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 002 " + client.getNickname() + " Your host is " + server.getServerName() + ", running version 1.0\r\n";
	return message;
}

std::string RPL::RPL_CREATED( Client const & client, Server const & server ) {

	std::string date = PrintTime::printTime().substr(0, 10);
	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 003 " + client.getNickname() + " This server was created " + date + "\r\n";
	return message;
}


std::string RPL::RPL_MYINFO( Client const & client, Server const & server ) {

	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 004 " + client.getNickname() + " " + server.getServerName() + " v1.0 [user modes: none] [channel modes: itklo]\r\n";
	return message;
}

// For NICK -------------------------------------------------------------------------------------------------------------

//:tungsten.libera.chat 433 * mariyadancheva :Nickname is already in use.

std::string RPL::ERR_NICKNAMEINUSE( Client const & client, Server const & server, std::string & wantedNickname ) {

	(void)wantedNickname;
	(void)client;
	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 433 " + wantedNickname + " " + wantedNickname + "\r\n";
	return message;
}
std::string RPL::ERR_NONICKNAMEGIVEN( Server const & server ) {

	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 431 " + ":No nickname given\r\n";
	return message;
}


std::string RPL::ERR_ERRONEUSNICKNAME( Client const & client, Server const & server ) {

	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 432 " + client.getNickname() + "\r\n";
	return message;
}

std::string RPL::RPL_NICK( Client const & client, Server const & server, std::string & newNickname ) {

	(void)server;
	std::string message = "@time=" + PrintTime::printTime() + ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " NICK " + newNickname + "\r\n";
	return message;
}

std::string RPL::RPL_SAVENICK( Client const & client, Server const & server ) {

	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 043 " + client.getNickname() + client.getNickname() + "\r\n";
	return message;
}

// For WHOIS ------------------------------------------------------------------------------------------------------------

std::string RPL::RPL_WHOISUSER( Client const & client, Server const & server ) {

	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 311 " + client.getNickname() + " " + client.getUsername() + " " + client.getHostname() + " * :" + client.getRealname() + "\r\n";
	return message;
}

std::string RPL::ERR_NEEDMOREPARAMS( Client const & client, Server const & server, std::string const & command ) {

	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 461 " + client.getNickname() + " " + command + ":Not enough parameters. Disconnecting\r\n";
	return message;
}

// PING PONG

std::string RPL::ERR_NOORIGIN( Server const & server ) {

	std::string message = "@time=" + PrintTime::printTime() + ":" + server.getServerName() + " 409 :No origin specified\r\n";
	return message;
}