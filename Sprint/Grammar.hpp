//
//  Grammar.hpp
//  Sprint
//
//  Created by Thomas Redding on 11/12/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Grammar_cpp
#define Grammar_cpp

#include <stdio.h>
#include <vector>
#include "Token.hpp"

enum GrammarType {
	GENERAL, ENUM, ENUM_DECLARATIONS, ENUM_DECLARATION, CLASS, DECLARATIONS, VARIABLE_DECLARATION, METHOD_DECLARATION, TERMINAL
};

struct GrammarObject {
	GrammarObject(GrammarType ty) {
		type = ty;
	}
	GrammarObject(GrammarType ty, Token to) {
		type = ty;
		token = to;
	}
	GrammarObject(GrammarType ty, TokenType tt, std::string str) {
		type = ty;
		token.type = tt;
		token.str = str;
	}
	std::string toString() {
		if(type == TERMINAL)
			return token.str;
		else if(type == GENERAL)
			return "<GENERAL>";
		else if(type == DECLARATIONS)
			return "<DECLARATIONS>";
		else if(type == VARIABLE_DECLARATION)
			return "<VARIABLE_DECLARATION>";
		else if(type == METHOD_DECLARATION)
			return "<METHOD_DECLARATION>";
		else
			return "<???>";
	}
	GrammarType type;
	Token token;
};

struct GrammarRule {
	GrammarRule(GrammarObject c, std::vector<Token> n, std::vector<GrammarObject> r) : current(c), next(n), result(r) {
	};
	GrammarObject current;
	std::vector<Token> next;
	std::vector<GrammarObject> result;
};

class Grammar {
public:
	Grammar();
	std::vector<GrammarObject> getRule(GrammarObject current, std::vector<Token> &next, int index);
private:
	std::vector<GrammarRule> rules;
	bool equalityChecker(GrammarObject &a, GrammarObject &b, std::vector<Token> &next, std::vector<Token> &d, int index);
};

#endif /* Grammar_cpp */
