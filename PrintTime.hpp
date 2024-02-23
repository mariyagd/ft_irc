#ifndef PRINTTIME_HPP
# define PRINTTIME_HPP

# include <time.h>
# include <string>

class PrintTime {
private:
	PrintTime( void );

public:
	~PrintTime( void );
	static std::string printTime( void );
};

#endif