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
    etc,
    etc_not,
    
	INDENT,
    DEDENT,
    NEWLINE = 4,
    KEYWORD,
    IDENTIFIER,
	INTEGER_LITERAL,
    FLOAT_LITERAL = 8,
    CHARACTER_LITERAL,
    STRING_LITERAL,
	PUNCTUATION,
    BRACKET = 12,
    UNKNOWN,
    NULL_TOKEN_TYPE,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	OPEN_CURLY_BRACE,
	CLOSE_CURLY_BRACE,
	KEYWORD_CLASS,
	
    foo,
    bar = 16,
    program,
};

const size_t token_threshold = 15;


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
	bool halfEquals(Token ty) {
		if(ty.str == "" || str == "")
			return type == ty.type;
		return equals(ty);
	}
	bool equals(Token ty) {
		if(type == ty.type && str == ty.str)
			return true;
		return false;
	}
    friend std::ostream& operator<<(std::ostream& o, const Token& t) {
        return o << t.type;
    }
	TokenType type;
	std::string str;
	long lineNum;
	long charNum;
};

#endif /* Token_cpp */
