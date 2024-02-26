#include "PrintTime.hpp"

PrintTime::PrintTime( void ) {

	return;
}

PrintTime::~PrintTime( void ) {

	return;
}


std::string	PrintTime::printTime(  )
{
	time_t _time;
	struct tm * timeinfo;
	const size_t size = 80;
	char buffer [size];

	time (&_time);
	timeinfo = localtime (&_time);

	strftime (buffer, size, "%FT%X", timeinfo );
	std::string time = std::string (buffer ) + " ";
	return time;
}