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
#include <deque>

#include "Token.hpp"

class Tokenizer {
public:
	Tokenizer();
	std::vector<Token> process(std::string str);
private:
	enum StackState {
		paranthesis, curly_bracket, square_brace, multi_line_comment
	};
	std::set<std::string> keywords;
	
	std::string removeComents(std::string str);
	bool isStartOfIdentifierLetter(char c);
	bool isIdentifierLetter(char c);
	bool inComment();
	void resetToken(Token &token, long lineNum, long charNum);
	void storeAndReset(Token &token, long lineNum, long charNum);
	std::vector<std::string> split(std::string str, char delim);
	void tokenizeLine(std::string str, long lineNum);
	void setToken(Token &token, TokenType type, std::string str, long lineNum, long charNum);
	TokenType identifierStringToEnum(std::string str);
	void popOffStack(StackState state);
	
	std::vector<Token> rtn;
	std::vector<int> tabs;
	std::deque<StackState> stack;
};

#endif /* Tokenizer_cpp */
