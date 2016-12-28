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

struct TemplateDupliator {
	void expand_tree(ParseNode* root);
};