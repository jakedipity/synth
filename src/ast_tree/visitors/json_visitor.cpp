#include "ast_tree/visitors/json_visitor.hpp"
#include "ast_tree/nodes/program_node.hpp"
#include "ast_tree/nodes/identifier_node.hpp"
#include "ast_tree/nodes/tokens_directive_node.hpp"

JSONVisitor::JSONVisitor() : m_stream()
{
}

char* JSONVisitor::getOutput()
{
	std::string out = m_stream.str();
	char* cpy = new char[out.length()+1];
	out.copy( cpy, out.length(), 0 );
	cpy[out.length()] = '\0';
	return cpy;
}

void JSONVisitor::visit( ProgramNode* node )
{
	m_stream << "ProgramNode->" << std::endl;
	node->visitChildren( this );
}

void JSONVisitor::visit( TokensDirectiveNode* node )
{
	m_stream << "TokensDirectiveNode->" << std::endl;
	node->visitChildren( this );
}

void JSONVisitor::visit( IdentifierNode* node )
{
        m_stream << "\"" + node->getIdentifier() + "\"" << std::endl;
}
