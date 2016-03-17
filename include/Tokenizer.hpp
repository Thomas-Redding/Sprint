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
#include <algorithm>
#include <deque>

#include "Token.hpp"

/*
 * This Tokenizer class conversts a string into a vector of tokens.
 */

class Tokenizer {
public:
	Tokenizer();
	/*
	 * @param str - the string to be converted into a vector of tokens
	 * @return - vector of tokens created from the inputed string
	 */
	std::vector<Token> process(std::string str);
private:
	enum StackState {
		paranthesis, curly_bracket, square_brace, multi_line_comment
	};
	std::vector<Token> rtn;
	std::vector<int> tabs;
	std::deque<StackState> stack;
	std::set<std::string> keywords;
	std::set<char> punctuation;

	/*
	 * @param str - converts a string to the corresponding TokenType enum
	 * @return - the TokenType of the given string
	 */
	TokenType identifierStringToEnum(std::string str);

	/*
	 * @return - returns whether the top of the stack is multi_line_comment
	 */
	bool isStackInComment();
	bool isIdentifierLetter(char c);
	bool isStartOfIdentifierLetter(char c);
	void popOffStack(StackState state, long lineNum, int it);
	void setToken(Token &token, TokenType type, std::string str, long lineNum, long charNum);
	std::vector<std::string> split(std::string str, char delim);
	void storeAndReset(Token &token, long lineNum, long charNum);
	void tokenizeLine(std::string str, long lineNum);
	void parseSpacelessTokens(std::string str, long lineNum, int it, bool isPreviousObject, bool isNextObject);
	void parseSpacelessAndEquallessTokens(std::string str, long lineNum, int it, bool isPreviousObject, bool isNextObject);

	/*
	 * @param str - a string of only puncutation (no spaces)
	 * @return - the input split along '=', '<', and '>'; but where
	 * +++=+++==++ becomes +++=, +++==, ++
	 */
	std::vector<std::string> equalitySplit(std::string str);
};

#endif /* Tokenizer_cpp */
