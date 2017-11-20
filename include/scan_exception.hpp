#ifndef SCAN_EXCEPTION_HPP
#define SCAN_EXCEPTION_HPP

#include "open_parse_exception.hpp"

class ScanException : public OpenParseException
{
public:
	explicit ScanException( const std::string& message );
};

#endif
