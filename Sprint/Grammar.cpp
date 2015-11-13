//
//  Grammar.cpp
//  Sprint
//
//  Created by Thomas Redding on 11/12/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "Grammar.hpp"

/*
 enum TokenType {
	INDENT, DEDENT, NEWLINE, KEYWORD, IDENTIFIER,
	INTEGER_LITERAL, FLOAT_LITERAL, CHARACTER_LITERAL, STRING_LITERAL,
	PUNCTUATION, BRACKET, UNKNOWN
 };
*/

Grammar::Grammar() {
	rules.push_back(GrammarRule(GrammarObject(GENERAL),				Token(UNKNOWN, ""),			{})); // "null" rule
	rules.push_back(GrammarRule(GrammarObject(GENERAL),				Token(KEYWORD, "class"),	{GrammarObject(CLASS)}));
	rules.push_back(GrammarRule(GrammarObject(CLASS),				Token(KEYWORD, "class"),	{GrammarObject(TERMINAL, KEYWORD, "class"), GrammarObject(TERMINAL, IDENTIFIER), GrammarObject(TERMINAL, NEWLINE, "\n"), GrammarObject(TERMINAL, INDENT), GrammarObject(METHOD_DECLARATIONS)}));
	rules.push_back(GrammarRule(GrammarObject(METHOD_DECLARATIONS),	Token(KEYWORD, "void"),		{GrammarObject(METHOD_DECLARATION), GrammarObject(METHOD_DECLARATIONS)}));
	rules.push_back(GrammarRule(GrammarObject(METHOD_DECLARATION),	Token(KEYWORD, "void"),		{GrammarObject(TERMINAL, KEYWORD, "void"), GrammarObject(TERMINAL, IDENTIFIER), GrammarObject(TERMINAL, BRACKET, "("), GrammarObject(PARAMETER), GrammarObject(TERMINAL, BRACKET, ")"), GrammarObject(METHOD_DECLARATIONS), GrammarObject(TERMINAL, NEWLINE)}));
	rules.push_back(GrammarRule(GrammarObject(PARAMETER),			Token(KEYWORD, "int"),		{GrammarObject(TERMINAL, KEYWORD, "int"), GrammarObject(TERMINAL, IDENTIFIER)}));
}

std::vector<GrammarObject> Grammar::getRule(GrammarObject current, Token next) {
	for(int i=1; i<rules.size(); i++) {
		if(equalityChecker(current, rules[i].current, next, rules[i].next))
			return rules[i].result;
	}
	return rules[0].result;
}

bool Grammar::equalityChecker(GrammarObject &a, GrammarObject &b, Token &c, Token &d) {
	if(!c.equals(d))
		return false;
	if(a.type != b.type)
		return false;
	if(a.type != TERMINAL)
		return true;
	return a.token.equals(b.token) && a.type == b.type;
}
