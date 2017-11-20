#include "string_scanner.hpp"
#include "parser.hpp"
#include "parse_exception.hpp"
#include "scan_exception.hpp"
#include "ast_tree/visitors/json_visitor.hpp"
#include "ast_tree/nodes/ast_node.hpp"
#include <iostream>

int main()
{
	std::string input( ".tokens{TOKEN_OPEN,TOKEN_CLOSE}" );
	std::unique_ptr<StringScanner> scanContext( new StringScanner( input ) );
	Parser parseContext( std::move( scanContext ) );
	
	try
	{
		parseContext.parse();
	}
	catch( const ParseException& exception )
	{
		std::cout << "Error: " << exception.what() << std::endl;
		return 1;
	}
	catch( const ScanException& exception )
	{
		std::cout << "Error: " << exception.what() << std::endl;
		return 1;
	}

	std::unique_ptr<ASTNode> node = std::move( parseContext.getParseData().m_rootNode );
	std::unique_ptr<JSONVisitor> printer = std::unique_ptr<JSONVisitor>( new JSONVisitor );
	node->acceptVisitor( printer.get() );
	std::cout << printer->getOutput() << std::endl;
	return 0;
}
