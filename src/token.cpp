#include "token.hpp"
#include <vector>

static std::vector<std::string> g_tokenStrings = {
	"$end",
	"$skip",
	"$abort",
	".tokens",
	"{",
	"}",
	",",
	"identifier"
};

Token::Token( Token::TokenType tokenType, Value value ) : m_type( tokenType ), m_value( value )
{
}

Token::Token( Token::TokenType tokenType ) : Token( tokenType, Value() )
{
}

bool Token::operator==( const Token& right ) const
{
	return m_type == right.m_type;
}

bool Token::operator!=( const Token& right ) const
{
	return !( *this == right );
}

bool Token::operator==( const TokenType& right ) const
{
	return m_type == right;
}

bool Token::operator!=( const TokenType& right ) const
{
	return !( *this == right );
}

const Token::TokenType& Token::getType() const
{
	return m_type;
}

const Value& Token::getValue() const
{
	return m_value;
}

const std::string& Token::toString() const
{
	return g_tokenStrings.at( static_cast<unsigned int>( m_type ) );
}
