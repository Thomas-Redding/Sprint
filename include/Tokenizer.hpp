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
	std::vector<Token> rtn;
	long it;
	bool isInSingleQuoteString;
	std::set<std::string> keywords;
	std::set<char> punctuation;
	std::set<char> brackets;
	bool isStartChar(char c);
	bool isMiddleChar(char c);
	inline bool isBracket(char c);
	void resetAndSave(Token &cur);
	inline bool isPunc(char c);
	void error(std::string str);

	std::vector<long> cumulativeCharsPerLine;
	long getLineNum();
	long getCharNum();
	std::vector<std::string> split(std::string str, char delim);
	void handlePunc(Token &cur);
};

#endif /* Tokenizer_cpp */
