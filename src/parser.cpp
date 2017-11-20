#include "parser.hpp"
#include "parse_exception.hpp"
#include "scanner.hpp"
#include "ast_tree/nodes/program_node.hpp"
#include "ast_tree/nodes/tokens_directive_node.hpp"
#include "ast_tree/nodes/identifier_node.hpp"

ParseData::ParseData() : m_rootNode()
{
}

Parser::Parser( std::unique_ptr<Scanner> scanContext ) : m_parseData(), m_scanContext( std::move( scanContext ) ), m_currentToken( m_scanContext->getNextToken() )
{
}

Parser::~Parser() = default;

ParseData& Parser::getParseData()
{
	return m_parseData;
}

void Parser::parse()
{
	grammarStart();
}

bool Parser::acceptToken( Token token )
{
	if( m_currentToken == token )
	{
		if( token != Token::TokenType::S_TOKEN_END )
		{
			m_currentToken = m_scanContext->getNextToken();
		}
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * Start		: Directive DirectiveP								{ $$ = new DirectiveNode( $1 ); if( $2.size() !=0 ) { for( auto& directive : $2 ) { $$.addDirective( std::move( directive ) ) } } }
 * 			;
 *
 * Directive		: TOKEN_DIRECTIVE_TOKENS TOKEN_OPENCURLY TokenList TOKEN_CLOSECURLY
 * 			;
 *
 * DirectiveP		: Directive DirectiveP
 * 			| %empty
 * 			;
 *
 * TokenList		: TOKEN_IDENTIFIER TokenListP
 * 			;
 *
 * TokenListP		: T_COMMA TOKEN_IDENTIFIER TokenListP
 * 			| %empty
 * 			;
 */

void Parser::grammarStart()
{
	if( peekDirective() )
	{
		auto var1 = grammarDirective();
		if( peekDirectiveP() )
		{
			auto var2 = grammarDirectiveP();
			if( acceptToken( Token::TokenType::S_TOKEN_END ) )
			{
				auto ret = std::unique_ptr<ProgramNode>( new ProgramNode( std::move( var1 ) ) );
				if( var2 )
				{
					ret->addDirectives( *var2.get() );
				}

				getParseData().m_rootNode = std::move( ret );
				return;	
			}
		}
	}

	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

std::unique_ptr<DirectiveNode> Parser::grammarDirective()
{
	if( acceptToken( Token::TokenType::TOKEN_TOKENS_DIRECTIVE ) )
	{
		if( acceptToken( Token::TokenType::TOKEN_OPENCURLY ) )
		{
			auto var3 = grammarTokenList();
			if( acceptToken( Token::TokenType::TOKEN_CLOSECURLY ) )
			{
				auto ret = std::unique_ptr<DirectiveNode>( new TokensDirectiveNode( *var3.get() ) );
				return ret;
			}
		}
	}

	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

std::unique_ptr<std::vector<std::unique_ptr<DirectiveNode>>> Parser::grammarDirectiveP()
{
	if( peekDirective() )
	{
		auto var1 = grammarDirective();
		if( peekDirectiveP() )
		{
			auto var2 = grammarDirectiveP();
			auto ret = std::unique_ptr<std::vector<std::unique_ptr<DirectiveNode>>>( new std::vector<std::unique_ptr<DirectiveNode>> );
			ret->push_back( std::move( var1 ) );
			if( var2 )
			{
				for( auto& directive : *var2.get() )
				{
					ret->push_back( std::move( directive ) );
				}
			}

			return ret;
		}
	}
	else
	{
		return std::unique_ptr<std::vector<std::unique_ptr<DirectiveNode>>>();
	}

	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

std::unique_ptr<std::vector<std::unique_ptr<IdentifierNode>>> Parser::grammarTokenList()
{
	Value lastValue = m_currentToken.getValue();
	if( acceptToken( Token::TokenType::TOKEN_IDENTIFIER ) )
	{
		auto var2 = grammarTokenListP();
		auto ret = std::unique_ptr<std::vector<std::unique_ptr<IdentifierNode>>>();
		if( var2 )
		{
			ret = std::move( var2 );
		}
		else
		{
			ret = std::unique_ptr<std::vector<std::unique_ptr<IdentifierNode>>>( new std::vector<std::unique_ptr<IdentifierNode>> );
		}

		ret->push_back( std::unique_ptr<IdentifierNode>( new IdentifierNode( lastValue.string ) ) );
		return ret;
	}

	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

std::unique_ptr<std::vector<std::unique_ptr<IdentifierNode>>> Parser::grammarTokenListP()
{
	if( acceptToken( Token::TokenType::TOKEN_COMMA ) )
	{
		Value lastValue = m_currentToken.getValue();
		if( acceptToken( Token::TokenType::TOKEN_IDENTIFIER ) )
		{
			auto var2 = grammarTokenListP();
			auto ret = std::unique_ptr<std::vector<std::unique_ptr<IdentifierNode>>>();
			if( var2 )
			{
				ret = std::move( var2 );
			}
			else
			{
				ret = std::unique_ptr<std::vector<std::unique_ptr<IdentifierNode>>>( new std::vector<std::unique_ptr<IdentifierNode>> );
			}

			ret->push_back( std::unique_ptr<IdentifierNode>( new IdentifierNode( lastValue.string ) ) );
			return ret;
		}
	}
	else
	{
		return std::unique_ptr<std::vector<std::unique_ptr<IdentifierNode>>>();
	}

	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

bool Parser::peekDirective()
{
	switch( m_currentToken.getType() )
	{
	case Token::TokenType::TOKEN_TOKENS_DIRECTIVE:
		return true;
	case Token::TokenType::S_TOKEN_END:
	case Token::TokenType::S_TOKEN_SKIP:
	case Token::TokenType::S_TOKEN_ABORT:
	case Token::TokenType::TOKEN_OPENCURLY:
	case Token::TokenType::TOKEN_CLOSECURLY:
	case Token::TokenType::TOKEN_COMMA:
	case Token::TokenType::TOKEN_IDENTIFIER:
	default:
		return false;
	}
}

bool Parser::peekDirectiveP()
{
	return true;
}

bool Parser::peekTokenList()
{
	switch( m_currentToken.getType() )
	{
	case Token::TokenType::TOKEN_IDENTIFIER:
		return true;
	case Token::TokenType::TOKEN_TOKENS_DIRECTIVE:
	case Token::TokenType::S_TOKEN_END:
	case Token::TokenType::S_TOKEN_SKIP:
	case Token::TokenType::S_TOKEN_ABORT:
	case Token::TokenType::TOKEN_OPENCURLY:
	case Token::TokenType::TOKEN_CLOSECURLY:
	case Token::TokenType::TOKEN_COMMA:
	default:
		return false;
	}
}

bool Parser::peekTokenListP()
{
	return true;
}

/*
void Parser::grammarStart()
{
	if( peekExpression() )
	{
		auto var1 = grammarExpression();

		if( acceptToken( Token::TokenType::S_TOKEN_END ) )
		{
			m_parseData.m_rootNode = std::unique_ptr<MainNode>( new MainNode( std::move( var1 ) ) );
			return;
		}
	}
	
	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

std::unique_ptr<ExpressionNode> Parser::grammarExpression()
{
	if( peekFactor() )
	{
		auto var1 = grammarFactor();

		if( acceptToken( Token::TokenType::TOKEN_OR ) )
		{
			auto var2 = grammarExpression();
			return std::unique_ptr<ExpressionNode>( new OrNode( std::move( var2 ), std::move( var1 ) ) );
		}

		return var1;
	}

	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

std::unique_ptr<ExpressionNode> Parser::grammarFactor()
{
	if( peekPrefix() )
	{
		auto var1 = grammarPrefix();
		
		if( peekFactor() )
		{
			auto var2 = grammarFactor();
			return std::unique_ptr<ExpressionNode>( new AndNode( std::move( var2 ), std::move( var1 ) ) );
		}
		else if( acceptToken( Token::TokenType::TOKEN_AND ) )
		{
			auto var3 = grammarFactor();
			return std::unique_ptr<ExpressionNode>( new AndNode( std::move( var3 ), std::move( var1 ) ) );
		}

		return var1;
	}

	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

std::unique_ptr<ExpressionNode> Parser::grammarPrefix()
{
	if( peekTerm() )
	{
		auto var1 = grammarTerm();
		return var1;
	}
	else if( acceptToken( Token::TokenType::TOKEN_NOT ) )
	{
		auto var2 = grammarPrefix();
		return std::unique_ptr<ExpressionNode>( new NotNode( std::move( var2 ) ) );
	}
	else if( acceptToken( Token::TokenType::TOKEN_GT ) )
	{
		auto var2 = grammarTerm();
		return std::unique_ptr<ExpressionNode>( new GreaterNode( std::move( var2 ) ) );
	}
	else if( acceptToken( Token::TokenType::TOKEN_GTE ) )
	{
		auto var2 = grammarTerm();
		return std::unique_ptr<ExpressionNode>( new GreaterEqualNode( std::move( var2 ) ) );
	}
	else if( acceptToken( Token::TokenType::TOKEN_LT ) )
	{
		auto var2 = grammarTerm();
		return std::unique_ptr<ExpressionNode>( new LessNode( std::move( var2 ) ) );
	}
	else if( acceptToken( Token::TokenType::TOKEN_LTE ) )
	{
		auto var2 = grammarTerm();
		return std::unique_ptr<ExpressionNode>( new LessEqualNode( std::move( var2 ) ) );
	}
	else if( acceptToken( Token::TokenType::TOKEN_EQ ) )
	{
		auto var2 = grammarTerm();
		return std::unique_ptr<ExpressionNode>( new EqualNode( std::move( var2 ) ) );
	}

	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

std::unique_ptr<ExpressionNode> Parser::grammarTerm()
{
	Value lastValue = m_currentToken.getValue();

	if( acceptToken( Token::TokenType::TOKEN_TRUE ) )
	{
		return std::unique_ptr<BoolTrueNode>( new BoolTrueNode() );
	}
	else if( acceptToken( Token::TokenType::TOKEN_FALSE ) )
	{
		return std::unique_ptr<BoolFalseNode>( new BoolFalseNode() );
	}
	else if( acceptToken( Token::TokenType::TOKEN_IDENTIFIER ) )
	{
		return std::unique_ptr<ExpressionNode>( new IdentifierNode( lastValue.string ) );
	}
	else if( acceptToken( Token::TokenType::TOKEN_INTEGER ) )
	{
		return std::unique_ptr<ExpressionNode>( new IntegerNode( lastValue.integer ) );
	}
	else if( acceptToken( Token::TokenType::TOKEN_QUOTE ) )
	{
		return std::unique_ptr<ExpressionNode>( new QuoteNode( std::unique_ptr<IdentifierNode>( new IdentifierNode( lastValue.string ) ) ) );
	}
	else if( acceptToken( Token::TokenType::TOKEN_LEN ) )
	{
		if( acceptToken( Token::TokenType::TOKEN_OPENPAREN ) )
		{
			lastValue = m_currentToken.getValue();
			if( acceptToken( Token::TokenType::TOKEN_INTEGER ) )
			{
				auto tmp1 = std::unique_ptr<IntegerNode>( new IntegerNode( lastValue.integer ) );
				if( acceptToken( Token::TokenType::TOKEN_CLOSEPAREN ) )
				{
					return std::unique_ptr<ExpressionNode>( new LenNode( std::move( tmp1 ) ) );
				}
			}
		}
	}
	else if( acceptToken( Token::TokenType::TOKEN_OPENPAREN ) )
	{
		if( peekExpression() )
		{
			auto var1 = grammarExpression();
			if( acceptToken( Token::TokenType::TOKEN_CLOSEPAREN ) )
			{
				return var1;
			}
		}
	}

	throw ParseException( "Error: did not expect \"" + m_currentToken.toString() + "\"" );
}

bool Parser::peekExpression()
{
	return peekFactor();
}

bool Parser::peekFactor()
{
	return peekPrefix();
}

bool Parser::peekPrefix()
{
	switch( m_currentToken.getType() )
	{
	case Token::TokenType::TOKEN_NOT:
	case Token::TokenType::TOKEN_GT:
	case Token::TokenType::TOKEN_GTE:
	case Token::TokenType::TOKEN_LT:
	case Token::TokenType::TOKEN_LTE:
	case Token::TokenType::TOKEN_EQ:
		return true;
	case Token::TokenType::S_TOKEN_SKIP:
	case Token::TokenType::S_TOKEN_END:
	case Token::TokenType::S_TOKEN_ABORT:
	case Token::TokenType::TOKEN_QUOTE:
	case Token::TokenType::TOKEN_AND:
	case Token::TokenType::TOKEN_OR:
	case Token::TokenType::TOKEN_LEN:
	case Token::TokenType::TOKEN_TRUE:
	case Token::TokenType::TOKEN_FALSE:
	case Token::TokenType::TOKEN_OPENPAREN:
	case Token::TokenType::TOKEN_CLOSEPAREN:
	case Token::TokenType::TOKEN_IDENTIFIER:
	case Token::TokenType::TOKEN_INTEGER:
	default:
		return peekTerm();
	}
}

bool Parser::peekTerm()
{
	switch( m_currentToken.getType() )
	{
	case Token::TokenType::TOKEN_TRUE:
	case Token::TokenType::TOKEN_FALSE:
	case Token::TokenType::TOKEN_IDENTIFIER:
	case Token::TokenType::TOKEN_INTEGER:
	case Token::TokenType::TOKEN_QUOTE:
	case Token::TokenType::TOKEN_LEN:
	case Token::TokenType::TOKEN_OPENPAREN:
		return true;
	case Token::TokenType::S_TOKEN_SKIP:
	case Token::TokenType::S_TOKEN_END:
	case Token::TokenType::S_TOKEN_ABORT:
	case Token::TokenType::TOKEN_NOT:
	case Token::TokenType::TOKEN_GT:
	case Token::TokenType::TOKEN_GTE:
	case Token::TokenType::TOKEN_LT:
	case Token::TokenType::TOKEN_LTE:
	case Token::TokenType::TOKEN_EQ:
	case Token::TokenType::TOKEN_AND:
	case Token::TokenType::TOKEN_OR:
	case Token::TokenType::TOKEN_CLOSEPAREN:
	default:
		return false;
	}
}
*/
