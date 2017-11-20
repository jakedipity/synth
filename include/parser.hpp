#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include <memory>
#include <vector>

class Scanner;
class ASTNode;
class DirectiveNode;
class IdentifierNode;

struct ParseData
{
	std::unique_ptr<ASTNode> m_rootNode;
	ParseData();
};

class Parser
{
	ParseData m_parseData;
	std::unique_ptr<Scanner> m_scanContext;
	Token m_currentToken;
public:
	Parser( std::unique_ptr<Scanner> scanContext );
	~Parser();
	ParseData& getParseData();
	void parse();
private:
	bool acceptToken( Token token );
	void grammarStart();
	std::unique_ptr<DirectiveNode> grammarDirective();
	std::unique_ptr<std::vector<std::unique_ptr<DirectiveNode>>> grammarDirectiveP();
	std::unique_ptr<std::vector<std::unique_ptr<IdentifierNode>>> grammarTokenList();
	std::unique_ptr<std::vector<std::unique_ptr<IdentifierNode>>> grammarTokenListP();
	bool peekDirective();
	bool peekDirectiveP();
	bool peekTokenList();
	bool peekTokenListP();
	/*std::unique_ptr<ExpressionNode> grammarExpression();
	std::unique_ptr<ExpressionNode> grammarFactor();
	std::unique_ptr<ExpressionNode> grammarPrefix();
	std::unique_ptr<ExpressionNode> grammarTerm();
	bool peekExpression();
	bool peekFactor();
	bool peekPrefix();
	bool peekTerm();
	*/
};

#endif
