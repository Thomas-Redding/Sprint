#ifndef PARSER_HPP
#define PARSER_HPP

#include <assert.h>
#include <iostream>
#include <stack>
#include <list>


#include "Token.hpp"

enum TreeType {
    T_KEYWORD,
    T_IDENTIFIER,
    T_INTEGER_LITERAL,
    T_FLOAT_LITERAL,
    T_FLOAT_LITERAL2,
    T_CHARACTER_LITERAL,
    T_STRING_LITERAL,
    T_PUNCTUATION,
    T_BRACKET,
    T_UNKNOWN,
    T_NULL_TOKEN_TYPE,
	T_OPEN_PARENTHESIS,
	T_CLOSE_PARENTHESIS,
	T_OPEN_BRACKET,
	T_CLOSE_BRACKET,
	T_OPEN_CURLY_BRACE,
	T_CLOSE_CURLY_BRACE,
	T_KEYWORD_ABSTRACT,
	T_KEYWORD_BREAK,
	T_KEYWORD_CASE,
	T_KEYWORD_CATCH,
	T_KEYWORD_CLASS,
	T_KEYWORD_CONST,
	T_KEYWORD_CONTINUE,
	T_KEYWORD_DELETE,
	T_KEYWORD_DO,
	T_KEYWORD_ELSE,
	T_KEYWORD_ENUM,
	T_KEYWORD_FALSE,
	T_KEYWORD_FOR,
	T_KEYWORD_IF,
	T_KEYWORD_IN,
	T_KEYWORD_INLINE,
	T_KEYWORD_NEW,
	T_KEYWORD_NULL,
	T_KEYWORD_PROTECTED,
	T_KEYWORD_PRIVATE,
	T_KEYWORD_PTR,
	T_KEYWORD_REF,
	T_KEYWORD_RETURN,
	T_KEYWORD_SIZEOF,
	T_KEYWORD_STATIC,
	T_KEYWORD_STRUCT,
	T_KEYWORD_SWITCH, 
	T_KEYWORD_THIS,
	T_KEYWORD_THROW,
	T_KEYWORD_TRUE,
	T_KEYWORD_TRY,
	T_KEYWORD_VIRTUAL,
	T_KEYWORD_WHILE,
	T_KEYWORD_INT,
	T_KEYWORD_INT8,
	T_KEYWORD_INT16,
	T_KEYWORD_INT32,
	T_KEYWORD_INT64,
	T_KEYWORD_UINT,
	T_KEYWORD_UINT8,
	T_KEYWORD_UINT16,
	T_KEYWORD_UINT32,
	T_KEYWORD_UINT64,
	T_KEYWORD_AND,
	T_KEYWORD_OR,
	T_KEYWORD_NOT,
	T_KEYWORD_XOR,
	T_KEYWORD_FLOAT,
	T_KEYWORD_DOUBLE,
	T_KEYWORD_PUBLIC,
	T_KEYWORD_VOID,
	T_PERIOD,
	T_COLON,
	T_SEMI_COLON,
	T_PLUS,
	T_MINUS,
	T_ASTERISK,
	T_SLASH,
	T_AMPERSAND,
	T_POUND_SIGN,
	T_LESS_THAN,
	T_EQUALS,
	T_GREATER_THAN,
	T_COMMA,
	T_VERTICAL_BAR,
	T_PERCENT,
	T_EXCLAMATION_POINT,
	T_CARROT,
	T_QUESTION_MARK,
	T_BACK_SLASH,
	T_AT,
	T_TILDE,
	T_PLUS_EQUALS,
	T_MINUS_EQUALS,
	T_SLASH_EQUALS,
	T_ASTERISK_EQUALS,
	T_AMPERSAND_EQUALS,
	T_CARROT_EQUALS,
	T_COLON_EQUALS,
	T_VERTICAL_BAR_EQUALS,
	T_PLUS_PLUS,
	T_MINUS_MINUS,
	T_SHIFT_LEFT,
	T_SHIFT_RIGHT,
	T_GREATER_THAN_EQUALS,
	T_LESS_THAN_EQUALS,
	T_SHIFT_LEFT_EQUALS,
	T_SHIFT_RIGHT_EQUALS,
	T_EXCLAMATION_POINT_EQUALS,
	T_EQUAL_EQUALS,
	T_BACK_SLASH_EQUALS,
	T_ARROW,
	T_EQUAL_EQUAL_EQUALS,
	T_EXCLAMATION_POINT_EQUAL_EQUALS,
	general
};


TreeType translateType(TokenType t);

struct ThomasNode {
	TreeType type;
	Token token;
	std::list<ThomasNode*> children;
	ThomasNode(TreeType tt) {
		type = tt;
	}
	ThomasNode(Token t) {
		token = t;
		type = translateType(t.type);
	}
};

class ThomasParser {
private:
	const Token* tokens;
	uint64_t len;
	ThomasNode* mainTree;
	void doCurlyBracePass(ThomasNode* tree);
public:
	ThomasNode* getParseTree(const Token* t, uint64_t n);
};



#endif