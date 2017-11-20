#include "string_scanner.hpp"

StringScanner::StringScanner( std::string input ) : m_stringBuffer( input )
{
}

std::string& StringScanner::getScanBuffer()
{
	return m_stringBuffer;
}
