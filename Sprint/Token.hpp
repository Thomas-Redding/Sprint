//
//  Token.hpp
//  Sprint
//
//  Created by Thomas Redding on 10/21/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Token_cpp
#define Token_cpp

#include <string>

enum TokenType {
	INDENT, DEDENT, NEWLINE, KEYWORD, IDENTIFIER,
	INTEGER_LITERAL, FLOAT_LITERAL, CHARACTER_LITERAL, STRING_LITERAL,
	PUNCTUATION, BRACKET, UNKNOWN
};

class Token {
public:
	Token() {
	}
	Token(TokenType ty) {
		type = ty;
		str = "";
	}
	Token(TokenType ty, std::string st) {
		type = ty;
		str = st;
	}
	Token(TokenType ty, std::string st, long ln, long cn) {
		type = ty;
		str = st;
		lineNum = ln;
		charNum = cn;
	}
	bool equals(Token ty) {
		if(type == ty.type && str == ty.str)
			return true;
		return false;
	}
	TokenType type;
	std::string str;
	long lineNum;
	long charNum;
};

#endif /* Token_cpp */
