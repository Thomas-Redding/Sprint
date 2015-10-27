//
//  Tokenizer.hpp
//  Sprint
//
//  Created by Thomas Redding on 10/21/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Tokenizer_cpp
#define Tokenizer_cpp

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <ctype.h>
#include <set>

#include "Token.hpp"

class Tokenizer {
public:
	Tokenizer();
	std::vector<Token> process(std::string str);
	std::string tokenTypeToString(TokenType t);
private:
	std::set<char> punctuation;
	std::set<char> brackets;
	std::set<std::string> keywords;
	
	std::string removeComents(std::string str);
	std::vector<std::string> split(std::string str, char delim);
	void tokenizeLine(std::string str, std::vector<Token> *rtn, int lineNum);
	bool isStartOfIdentifierLetter(char c);
	bool isIdentifierLetter(char c);
	std::vector<std::string> dividePunctuations(std::string str, char nextChar);
};

#endif /* Tokenizer_cpp */
