//
//  Parser.cpp
//  Sprint
//
//  Created by Thomas Redding on 10/22/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "Parser.hpp"

Parser::Parser() {
	//
}

void Parser::parse(std::vector<Token> *tokens) {
	index = -1;
	parseHelper(tokens, NONE);
}

ParserTree* Parser::parseHelper(std::vector<Token> *tokens, ParserState parserState) {
	/*
	 enum TokenType {
	 INDENT, DEDENT, NEWLINE, KEYWORD, IDENTIFIER,
	 INTEGER_LITERAL, FLOAT_LITERAL, CHARACTER_LITERAL, STRING_LITERAL,
	 PUNCTUATION, BRACKET, UNKNOWN
	 };
	 */
	// todo
	index++;
	Token t = tokens->at(index);
	if(parserState == NONE) {
		if(t.type == INDENT) {
			exit("Syntax Error: Found indentation prior to any characters");
		}
		else if(t.type == KEYWORD && t.str == "class") {
			ParserTree *tree;
			ParserToken token;
			token.token = t;
			token.state = CLASS;
			tree->setParserToken(token);
			parseHelper(tokens, CLASS);
		}
	}
	return nullptr;
}

void Parser::exit(std::string message) {
	std::cout << message << "\n";
	_exit(1);
}
