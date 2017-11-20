#ifndef IDENTIFIER_NODE_HPP
#define IDENTIFIER_NODE_HPP

#include "ast_tree/nodes/ast_node.hpp"
#include <string>

class IdentifierNode : public ASTNode
{
	std::string m_identifier;
public:
	IdentifierNode( std::string identifier );
	void visitChildren( Visitor* visitor ) final;
	void acceptVisitor( Visitor* visitor ) final;
	const std::string& getIdentifier() const;
};

#endif
