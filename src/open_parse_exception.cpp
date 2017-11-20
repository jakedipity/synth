#include "open_parse_exception.hpp"

OpenParseException::OpenParseException( const std::string& message ) : m_exceptionMessage( std::move( message ) )
{
}

const char* OpenParseException::what() const throw()
{
	return m_exceptionMessage.c_str();
}
