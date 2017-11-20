#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "value.hpp"
#include <string>

class Token
{
	public: enum class TokenType : unsigned int
	{
		S_TOKEN_END,
		S_TOKEN_SKIP,
		S_TOKEN_ABORT,
		TOKEN_TOKENS_DIRECTIVE,
		TOKEN_OPENCURLY,
		TOKEN_CLOSECURLY,
		TOKEN_COMMA,
		TOKEN_IDENTIFIER
	};
	private: TokenType m_type;
	private: Value m_value;
	public: Token( TokenType tokenType, Value value );
	public: Token( TokenType tokenType );
	public: bool operator==( const Token& right ) const;
	public: bool operator!=( const Token& right ) const;
	public: bool operator==( const TokenType& right ) const;
	public: bool operator!=( const TokenType& right ) const;
	public: const TokenType& getType() const;
	public: const Value& getValue() const;
	public: const std::string& toString() const;
};

#endif
