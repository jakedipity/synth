#ifndef VISITOR_HPP
#define VISITOR_HPP

class ProgramNode;
class TokensDirectiveNode;
class IdentifierNode;

class Visitor
{
public:
	virtual ~Visitor() = default;
	virtual void visit( ProgramNode* node ) = 0;
	virtual void visit( TokensDirectiveNode* node ) = 0;
	virtual void visit( IdentifierNode* node ) = 0;
};

#endif
