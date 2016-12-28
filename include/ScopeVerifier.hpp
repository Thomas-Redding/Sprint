#pragma once

#include <vector>
#include <string>
#include <map>

#include "Parser.hpp"

/*
	Pass 1:
		- find all templates
		- give each instantiation its own code
*/

struct ScopeVerifier {
	// throws an error if a class/function is referenced out of scope
	// throws an error if a variable is undeclared (or referenced out of scope)
	// throws an error if it cannot determine if "{...}" is a value or a type
	void verify_scope(ParseNode* root);

	void scope_error(std::string message);

	std::vector< std::map<std::string, TypeDeclaration> > types;
	std::vector< std::map<std::string, TypeInstantiation> > variables;

	std::vector< ParseNode* > fringes;
};