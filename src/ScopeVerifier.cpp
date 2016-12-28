
#include <string>
#include <iostream>
#include <vector>

#include "../include/ScopeVerifier.hpp"

void ScopeVerifier::verify_scope(ParseNode* root) {

	/**************************************************
		1) find global variables/classes/functions
	**************************************************/
	fringes.clear();
	std::map<std::string, TypeDeclaration> globalTypes;
	std::map<std::string, TypeInstantiation> globalVariables;

	// TODO:

	types.push_back(globalTypes);
	variables.push_back(globalVariables);
}

void ScopeVerifier::scope_error(std::string message) {
	std::cout << message << std::endl;
	exit(0);
}