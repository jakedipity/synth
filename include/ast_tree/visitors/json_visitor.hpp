#ifndef JSON_VISITOR_HPP
#define JSON_VISITOR_HPP

#include "ast_tree/visitors/visitor.hpp"
#include <sstream>

class ASTNode;

class ProgramNode;
class IdentifierNode;
class TokensDirectiveNode;

class JSONVisitor : public Visitor
{
private:
	std::stringstream m_stream;
public:
	JSONVisitor();
	char* getOutput();
	void visit( ProgramNode* node ) final;
	void visit( TokensDirectiveNode* node ) final;
	void visit( IdentifierNode* node ) final;
};

#endif
