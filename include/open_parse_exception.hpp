#ifndef OPEN_PARSE_EXCEPTION_HPP
#define OPEN_PARSE_EXCEPTION_HPP

#include <iomanip>

class OpenParseException : public std::exception
{
	std::string m_exceptionMessage;
public:
	explicit OpenParseException( const std::string& message );
	virtual ~OpenParseException() = default;
	const char* what() const throw();
};

#endif
