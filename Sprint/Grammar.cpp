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
	rules.push_back(GrammarRule(GrammarObject(GENERAL),				{Token(UNKNOWN, "")},													{})); // "null" rule
	rules.push_back(GrammarRule(GrammarObject(GENERAL),				{Token(KEYWORD, "enum")},												{GrammarObject(ENUM), GrammarObject(GENERAL)}));
	rules.push_back(GrammarRule(GrammarObject(ENUM),				{Token(KEYWORD, "enum"), Token(NEWLINE, "\n"), Token(INDENT)},			{GrammarObject(TERMINAL, KEYWORD, "enum"), GrammarObject(TERMINAL, NEWLINE, "\n"), GrammarObject(TERMINAL, INDENT), GrammarObject(ENUM_DECLARATION), GrammarObject(ENUM_DECLARATIONS), GrammarObject(TERMINAL, DEDENT)}));
	rules.push_back(GrammarRule(GrammarObject(ENUM_DECLARATIONS),	{Token(PUNCTUATION, ",")},												{GrammarObject(TERMINAL, PUNCTUATION, ","), GrammarObject(ENUM_DECLARATION), GrammarObject(ENUM_DECLARATIONS)}));
	rules.push_back(GrammarRule(GrammarObject(ENUM_DECLARATION),	{Token(IDENTIFIER), Token(PUNCTUATION, "="), Token(INTEGER_LITERAL)},	{GrammarObject(TERMINAL, IDENTIFIER), GrammarObject(TERMINAL, PUNCTUATION, "="), GrammarObject(TERMINAL, INTEGER_LITERAL)}));
	rules.push_back(GrammarRule(GrammarObject(ENUM_DECLARATION),	{Token(IDENTIFIER)},													{GrammarObject(TERMINAL, IDENTIFIER)}));
	rules.push_back(GrammarRule(GrammarObject(GENERAL),				{Token(KEYWORD, "class")},												{GrammarObject(CLASS), GrammarObject(GENERAL)}));
	rules.push_back(GrammarRule(GrammarObject(CLASS),				{Token(KEYWORD, "class")},												{GrammarObject(TERMINAL, KEYWORD, "class"), GrammarObject(TERMINAL, IDENTIFIER), GrammarObject(TERMINAL, NEWLINE, "\n"), GrammarObject(TERMINAL, INDENT), GrammarObject(DECLARATIONS), GrammarObject(TERMINAL, DEDENT)}));
	rules.push_back(GrammarRule(GrammarObject(DECLARATIONS),		{Token(KEYWORD, "int"), Token(IDENTIFIER), Token(BRACKET, "(")},		{GrammarObject(METHOD_DECLARATION), GrammarObject(TERMINAL, NEWLINE, "\n"), GrammarObject(DECLARATIONS)}));
	rules.push_back(GrammarRule(GrammarObject(DECLARATIONS),		{Token(KEYWORD, "int"), Token(IDENTIFIER)},								{GrammarObject(VARIABLE_DECLARATION), GrammarObject(TERMINAL, NEWLINE, "\n"), GrammarObject(DECLARATIONS)}));
	rules.push_back(GrammarRule(GrammarObject(VARIABLE_DECLARATION),{Token(KEYWORD, "int")},												{GrammarObject(TERMINAL, KEYWORD, "int"), GrammarObject(TERMINAL, IDENTIFIER)}));
	rules.push_back(GrammarRule(GrammarObject(METHOD_DECLARATION),	{Token(KEYWORD, "int")},												{GrammarObject(TERMINAL, KEYWORD, "int"), GrammarObject(TERMINAL, IDENTIFIER), GrammarObject(TERMINAL, BRACKET, "("), GrammarObject(VARIABLE_DECLARATION), GrammarObject(TERMINAL, BRACKET, ")"), GrammarObject(DECLARATIONS)}));
}

std::vector<GrammarObject> Grammar::getRule(GrammarObject current, std::vector<Token> &next, int index) {
	
	for(int i=1; i<rules.size(); i++) {
		if(equalityChecker(current, rules[i].current, rules[i].next, next, index))
			return rules[i].result;
	}
	return rules[0].result;
}

bool Grammar::equalityChecker(GrammarObject &a, GrammarObject &b, std::vector<Token> &exp, std::vector<Token> &next, int index) {
	
	if(index+exp.size() >= next.size())
		return false;
	for(int i=0; i<exp.size(); i++) {
		
		if(!exp[i].halfEquals(next[index+i]))
			return false;
	}
	if(a.type != b.type)
		return false;
	if(a.type != TERMINAL)
		return true;
	return a.token.equals(b.token) && a.type == b.type;
}
