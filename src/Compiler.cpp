#include "../include/Compiler.hpp"

std::string Compiler::compile(ParseNode* tree) {
	return "";
}

void Compiler::error(std::string message, ParseNode* tree) {
	std::cout << message << " (" << tree->token.lineNum << ", " << tree->token.charNum << ")\n";
	exit(0);
}
