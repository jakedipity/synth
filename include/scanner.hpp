#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "token.hpp"
#include <vector>
#include <stack>
#include <string>
#include <sstream>

enum class State : unsigned int
{
	STATE_INITIAL,
};

typedef std::stack<State> StateStack;

struct ScanData
{
	long unsigned int m_lineNumber;
	long unsigned int m_charPosition;
	ScanData();
};

class Scanner
{
	StateStack m_stateStack;
	ScanData m_scanData;
public:
	Scanner();
	virtual ~Scanner() = default;
	void pushState( State state );
	void popState();
	ScanData& getScanData();
	Token getNextToken();
private:
	virtual std::string& getScanBuffer() = 0;
};

#endif
