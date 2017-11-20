#include "ast_tree/nodes/identifier_node.hpp"
#include "ast_tree/visitors/visitor.hpp"

IdentifierNode::IdentifierNode( std::string identifier ) : m_identifier( identifier )
{
}

void IdentifierNode::visitChildren( Visitor* )
{
}

void IdentifierNode::acceptVisitor( Visitor* visitor )
{
	visitor->visit( this );
}

const std::string& IdentifierNode::getIdentifier() const
{
	return m_identifier;
}
