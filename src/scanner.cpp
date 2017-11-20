#include "scanner.hpp"
#include "scan_exception.hpp"
#include <functional>
#include <regex>

std::vector<std::string> g_tokenStrings = {
	"$end",
	"$skip",
	"$abort",
	".tokens"
};

std::vector<std::pair<std::regex, std::function<void( Scanner*, std::string )>>> g_global_lex_list = {
	{
		std::regex( "^\n" ),
		[]( Scanner* context, std::string ) { context->getScanData().m_lineNumber++; }
	},
	{
		std::regex( "^." ),
		[]( Scanner* context, std::string matchStr ) { context->getScanData().m_charPosition += matchStr.length(); }
	}
};

std::vector<std::vector<std::pair<std::regex, std::function<Token( Scanner*, std::string )>>>> g_lex_list = {
	{
		{
			std::regex( "^\\.token" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_TOKENS_DIRECTIVE ); }
		},
		{
			std::regex( "^\\{" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_OPENCURLY ); }
		},
		{
			std::regex( "^\\}" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_CLOSECURLY ); }
		},
		{
			std::regex( "^," ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_COMMA ); }
		},
		{
			std::regex( "^[a-zA-Z_][a-zA-Z_0-9]*" ),
			[]( Scanner*, std::string matchStr ) {
				char* cpy = new char[matchStr.length()+1];
				matchStr.copy( cpy, matchStr.length(), 0 );
				cpy[matchStr.length()] = '\0';
				Value val;
				val.string = cpy;
				return Token( Token::TokenType::TOKEN_IDENTIFIER, val );
			}
		},
		{
			std::regex( "^$" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::S_TOKEN_END ); }
		},
		{
			std::regex( "^." ),
			[]( Scanner*, std::string matchStr ) {
				throw ScanException( "Unknown symbol \"" + matchStr + "\"" );
				return Token( Token::TokenType::S_TOKEN_ABORT );
			}
		}
	}
};
/*
	{
		{
			std::regex( "^AND" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_AND ); }
		},
		{
			std::regex( "^OR" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_OR ); }
		},
		{
			std::regex( "^!" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_NOT ); }
		},
		{
			std::regex( "^>=" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_GTE ); }
		},
		{
			std::regex( "^>" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_GT ); }
		},
		{
			std::regex( "^<=" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_LTE ); }
		},
		{
			std::regex( "^<" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_LT ); }
		},
		{
			std::regex( "^=" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_EQ ); }
		},
		{
			std::regex( "^len" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_LEN ); }
		},
		{
			std::regex( "^true" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_TRUE ); }
		},
		{
			std::regex( "^false" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_FALSE ); }
		},
		{
			std::regex( "^\\(" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_OPENPAREN ); }
		},
		{
			std::regex( "^\\)" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::TOKEN_CLOSEPAREN ); }
		},
		{
			std::regex( "^\n" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::S_TOKEN_SKIP ); }
		},
		{
			std::regex( "^[ \t\r\f\v]+" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::S_TOKEN_SKIP ); }
		},
		{
			std::regex( "^(0|[1-9][0-9]*)" ),
			[]( Scanner*, std::string matchStr ) {
				Value val;
				val.integer = atoi( matchStr.c_str() );
				return Token( Token::TokenType::TOKEN_INTEGER, val );
			}
		},
		{
			std::regex( "^[a-zA-Z][a-zA-Z0-9]*" ),
			[]( Scanner*, std::string matchStr ) {
				char* cpy = new char[matchStr.length()+1];
				matchStr.copy( cpy, matchStr.length(), 0 );
				cpy[matchStr.length()] = '\0';
				Value val;
				val.string = cpy;
				return Token( Token::TokenType::TOKEN_IDENTIFIER, val );
			}
		},
		{
			std::regex( "^\"" ),
			[]( Scanner* context, std::string ) {
				context->pushState( State::STATE_QUOTE );
				context->getScanData().m_quoteStream.str("");
				return Token( Token::TokenType::S_TOKEN_SKIP );
			}
		},
		{
			std::regex( "^$" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::S_TOKEN_END ); }
		},
		{
			std::regex( "^." ),
			[]( Scanner*, std::string matchStr ) {
				throw ScanException( "Unknown symbol \"" + matchStr + "\"" );
				return Token( Token::TokenType::S_TOKEN_ABORT );
			}
		}
	},
	{
		{
			std::regex( "^\"" ),
			[]( Scanner* context, std::string ) {
				context->popState();
				std::string streamStr = context->getScanData().m_quoteStream.str();
				char* cpy = new char[streamStr.length()+1];
				streamStr.copy( cpy, streamStr.length(), 0 );
				cpy[streamStr.length()] = '\0';
				Value val;
				val.string = cpy;
				return Token( Token::TokenType::TOKEN_QUOTE, val );
			}
		},
		{
			std::regex( "^\n" ),
			[]( Scanner*, std::string ) { return Token( Token::TokenType::S_TOKEN_SKIP ); }
		},
		{
			std::regex( "^$" ),
			[]( Scanner*, std::string ) {
				throw ScanException( "Quote left unclosed" );
				return Token( Token::TokenType::S_TOKEN_ABORT );
			}
		},
		{
			std::regex( "^." ),
			[]( Scanner* context, std::string matchStr ) {
				context->getScanData().m_quoteStream << matchStr;
				return Token( Token::TokenType::S_TOKEN_SKIP );
			}
		}
	}
};
*/

ScanData::ScanData() : m_lineNumber( 1 ), m_charPosition( 1 )
{

}

Scanner::Scanner() : m_stateStack(), m_scanData()
{
	m_stateStack.push( State::STATE_INITIAL );
}

void Scanner::pushState( State state )
{
	m_stateStack.push( state );
}

void Scanner::popState()
{
	m_stateStack.pop();
}

ScanData& Scanner::getScanData()
{
	return m_scanData;
}

Token Scanner::getNextToken()
{
	std::smatch match;
	std::string matchStr;
	std::string& scanBuffer = getScanBuffer();
scanner_start:
	for( const auto& lexRule : g_lex_list[static_cast<unsigned int>( m_stateStack.top() )] )
	{
		if( std::regex_search( scanBuffer, match, std::get<0>( lexRule ) ) )
		{
			matchStr = match.str();
			Token token = std::get<1>( lexRule )( this, matchStr );
			scanBuffer.erase( 0, matchStr.length() );
			for( const auto& globalLexRule : g_global_lex_list )
			{
				if( std::regex_search( scanBuffer, match, std::get<0>( globalLexRule ) ) )
				{
					std::get<1>( globalLexRule )( this, matchStr );
					break;
				}
			}

			if( token == Token::TokenType::S_TOKEN_SKIP )
			{
				goto scanner_start;
			}

			return token;
		}
	}

	throw ScanException( "Unable to interpret next token" );
}
