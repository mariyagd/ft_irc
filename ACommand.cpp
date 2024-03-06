#include "ACommand.hpp"

ACommand::ACommand( void ) {
	return;
}

ACommand::~ACommand( void ) {
	return;
}

void ACommand::splitMsgOnComma( std::string & msg, std::vector< std::string > & tokens ) {

	std::istringstream iss( msg );

	while ( !iss.eof() )
	{
		std::string token;
		getline(iss, token, ',' ); // split by comas
		if ( !token.empty() )
			tokens.push_back( token );
	}
}

void ACommand::concatenate( std::vector< std::string > & tokens, size_t i, std::string & msg ) {

	for ( ; i < tokens.size(); i++ )
	{
		msg += tokens[i];
		if ( ( i + 1 ) != tokens.size() )
			msg += " ";
	}
}

void ACommand::printVector( std::vector< std::string > & tokens ) {

	for ( size_t i = 0; i < tokens.size(); i++ )
		std::cout << CYAN_BG << "[" << tokens[i] << "]" << END << std::endl;
}

