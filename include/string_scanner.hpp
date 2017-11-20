#ifndef STRING_SCANNER_HPP
#define STRING_SCANNER_HPP

#include "scanner.hpp"

class StringScanner : public Scanner
{
	std::string m_stringBuffer;
public:
	StringScanner( std::string input );
private:
	std::string& getScanBuffer() final;
};

#endif
