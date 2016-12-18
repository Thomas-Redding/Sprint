#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "Parser.hpp"

class Compiler {
public:
	std::string compile(ParseNode* tree);
private:
	void error(std::string message, ParseNode* tree);
}

#endif