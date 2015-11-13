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
	GENERAL, CLASS, METHOD_DECLARATIONS, METHOD_DECLARATION, PARAMETER, TERMINAL
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
	GrammarType type;
	Token token;
};

struct GrammarRule {
	GrammarRule(GrammarObject c, Token n, std::vector<GrammarObject> r) : current(c), next(n), result(r) {
	};
	GrammarObject current;
	Token next;
	std::vector<GrammarObject> result;
};

class Grammar {
public:
	Grammar();
	std::vector<GrammarObject> getRule(GrammarObject current, Token next);
private:
	std::vector<GrammarRule> rules;
	bool equalityChecker(GrammarObject &a, GrammarObject &b, Token &c, Token &d);
};

#endif /* Grammar_cpp */
