#ifndef AST_NODE_HPP
#define AST_NODE_HPP

class Visitor;

class ASTNode
{
public:
	virtual ~ASTNode() = default;
	virtual void visitChildren( Visitor* visitor ) = 0;
	virtual void acceptVisitor( Visitor* visitor ) = 0;
};

#endif
