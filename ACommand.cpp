#include "ACommand.hpp"

ACommand::ACommand( void ) : command() {
}

ACommand::ACommand( std::string & line ) {

	splitMsgOnSpace( line, command );
}

ACommand::~ACommand( void ) {
}

void	ACommand::splitMsgOnSpace( std::string & msg, std::vector< std::string > & tokens ) {

	std::istringstream iss( msg );
	size_t pos = 0;

	while ( !iss.eof() )
	{
		std::string token;
		iss >> token;
		if ( (pos = token.find( "\n" )) != std::string::npos )
		{
			token.erase( pos, 1 );
		}
		if ( !token.empty() )
			tokens.push_back( token );
	}
	return ;
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
