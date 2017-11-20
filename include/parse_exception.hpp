#ifndef PARSE_EXCEPTION_HPP
#define PARSE_EXCEPTION_HPP

#include "open_parse_exception.hpp"

class ParseException : public OpenParseException
{
public:
	explicit ParseException( const std::string& message );
};

#endif
