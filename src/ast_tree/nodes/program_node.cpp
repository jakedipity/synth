#include "ast_tree/nodes/program_node.hpp"
#include "ast_tree/nodes/directive_node.hpp"
#include "ast_tree/visitors/visitor.hpp"

ProgramNode::ProgramNode( std::unique_ptr<DirectiveNode> directiveNode ) : m_directiveNodes()
{
	m_directiveNodes.push_back( std::move( directiveNode ) );
}

void ProgramNode::visitChildren( Visitor* visitor )
{
	for( const auto& node : m_directiveNodes )
	{
		node->acceptVisitor( visitor );
	}
}

void ProgramNode::acceptVisitor( Visitor* visitor )
{
	visitor->visit( this );
}

void ProgramNode::addDirectives( std::vector<std::unique_ptr<DirectiveNode>>& directiveList )
{
	for( auto& directive : directiveList )
	{
		m_directiveNodes.push_back( std::move( directive ) );
	}
}
