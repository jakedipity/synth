#include "ast_tree/nodes/tokens_directive_node.hpp"
#include "ast_tree/nodes/identifier_node.hpp"
#include "ast_tree/visitors/visitor.hpp"

#include <iostream>

TokensDirectiveNode::TokensDirectiveNode( std::vector<std::unique_ptr<IdentifierNode>>& tokenList ) : m_tokenList()
{
	for( auto& token : tokenList )
	{
		m_tokenList.push_back( std::move( token ) );
	}
}

void TokensDirectiveNode::visitChildren( Visitor* visitor )
{
	for( const auto& token : m_tokenList )
	{
		token->acceptVisitor( visitor );
	}
}

void TokensDirectiveNode::acceptVisitor( Visitor* visitor )
{
	visitor->visit( this );
}
