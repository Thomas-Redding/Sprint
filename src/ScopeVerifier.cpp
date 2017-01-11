
#include <string>
#include <iostream>
#include <vector>

#include "../include/ScopeVerifier.hpp"

struct Brace {
	uint64_t id;
	uint64_t static number_of_braces;
};

struct Identifier {
	const char* name;
};

void ScopeVerifier::verify(ParseNode* root) {
	// TODO
}

void ScopeVerifier::error(std::string message) {
	std::cout << message << std::endl;
	exit(0);
}