#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <ctype.h>
#include <set>
#include <stack>
#include <algorithm>

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
	std::list<Token> process(std::string str);
private:
	std::list<Token> rtn;
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
	long getLineNum(int go_back);
	long getCharNum(int go_back);
	std::vector<std::string> split(std::string str, char delim);
	void categorizeIdentifier(Token &cur);
	TokenType categorizePunc(const std::string &str);
	const bool isPunc(TokenType t);
	const bool isKeyWord(TokenType t);
	const std::string tokenToString2(const Token& t);
};