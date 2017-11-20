#ifndef PROGRAM_NODE_HPP
#define PROGRAM_NODE_HPP

#include "ast_tree/nodes/ast_node.hpp"
#include <memory>
#include <vector>

class DirectiveNode;

class ProgramNode : public ASTNode
{
	std::vector<std::unique_ptr<DirectiveNode>> m_directiveNodes;
public:
	ProgramNode( std::unique_ptr<DirectiveNode> directiveNode );
	void visitChildren( Visitor* visitor ) final;
	void acceptVisitor( Visitor* visitor ) final;
	void addDirectives( std::vector<std::unique_ptr<DirectiveNode>>& directiveList );
};

#endif
