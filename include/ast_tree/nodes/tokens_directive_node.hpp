#ifndef TOKENS_DIRECTIVE_NODE_HPP
#define TOKENS_DIRECTIVE_NODE_HPP

#include "ast_tree/nodes/directive_node.hpp"
#include <memory>
#include <vector>

class IdentifierNode;

class TokensDirectiveNode : public DirectiveNode
{
	std::vector<std::unique_ptr<IdentifierNode>> m_tokenList;
public:
	TokensDirectiveNode( std::vector<std::unique_ptr<IdentifierNode>>& tokenList );
	void visitChildren( Visitor* visitor ) final;
	void acceptVisitor( Visitor* visitor ) final;
};

#endif
