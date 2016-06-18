#ifndef PARSER_HPP
#define PARSER_HPP

#include <assert.h>
#include <iostream>
#include <stack>
#include <list>


#include "ParseNode.hpp"
#include "Brace.hpp"

enum Exposure {
	PUBLIC,
	PROTECTED,
	READ_ONLY,
	PRIVATE,
};

enum TreeType {
    KEYWORD,
    IDENTIFIER,
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    FLOAT_LITERAL2,
    CHARACTER_LITERAL,
    STRING_LITERAL,
    PUNCTUATION,
    BRACKET,
    UNKNOWN,
    NULL_TOKEN_TYPE,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	OPEN_CURLY_BRACE,
	CLOSE_CURLY_BRACE,
	KEYWORD_ABSTRACT,
	KEYWORD_BREAK,
	KEYWORD_CASE,
	KEYWORD_CATCH,
	KEYWORD_CLASS,
	KEYWORD_CONST,
	KEYWORD_CONTINUE,
	KEYWORD_DELETE,
	KEYWORD_DO,
	KEYWORD_ELSE,
	KEYWORD_ENUM,
	KEYWORD_FALSE,
	KEYWORD_FOR,
	KEYWORD_IF,
	KEYWORD_IN,
	KEYWORD_INLINE,
	KEYWORD_NEW,
	KEYWORD_NULL,
	KEYWORD_PROTECTED,
	KEYWORD_PRIVATE,
	KEYWORD_PTR,
	KEYWORD_REF,
	KEYWORD_RETURN,
	KEYWORD_SIZEOF,
	KEYWORD_STATIC,
	KEYWORD_STRUCT,
	KEYWORD_SWITCH, 
	KEYWORD_THIS,
	KEYWORD_THROW,
	KEYWORD_TRUE,
	KEYWORD_TRY,
	KEYWORD_VIRTUAL,
	KEYWORD_WHILE,
	KEYWORD_INT,
	KEYWORD_INT8,
	KEYWORD_INT16,
	KEYWORD_INT32,
	KEYWORD_INT64,
	KEYWORD_UINT,
	KEYWORD_UINT8,
	KEYWORD_UINT16,
	KEYWORD_UINT32,
	KEYWORD_UINT64,
	KEYWORD_AND,
	KEYWORD_OR,
	KEYWORD_NOT,
	KEYWORD_XOR,
	KEYWORD_FLOAT,
	KEYWORD_DOUBLE,
	KEYWORD_PUBLIC,
	KEYWORD_VOID,
	PERIOD,
	COLON,
	SEMI_COLON,
	PLUS,
	MINUS,
	ASTERISK,
	SLASH,
	AMPERSAND,
	POUND_SIGN,
	LESS_THAN,
	EQUALS,
	GREATER_THAN,
	COMMA,
	VERTICAL_BAR,
	PERCENT,
	EXCLAMATION_POINT,
	CARROT,
	QUESTION_MARK,
	BACK_SLASH,
	AT,
	TILDE,
	PLUS_EQUALS,
	MINUS_EQUALS,
	SLASH_EQUALS,
	ASTERISK_EQUALS,
	AMPERSAND_EQUALS,
	CARROT_EQUALS,
	COLON_EQUALS,
	VERTICAL_BAR_EQUALS,
	PLUS_PLUS,
	MINUS_MINUS,
	SHIFT_LEFT,
	SHIFT_RIGHT,
	GREATER_THAN_EQUALS,
	LESS_THAN_EQUALS,
	SHIFT_LEFT_EQUALS,
	SHIFT_RIGHT_EQUALS,
	EXCLAMATION_POINT_EQUALS,
	EQUAL_EQUALS,
	BACK_SLASH_EQUALS,
	ARROW,
	EQUAL_EQUAL_EQUALS,
	EXCLAMATION_POINT_EQUAL_EQUALS
};


struct ThomasNode {
	TreeType type;
	const Token* tokens;
	uint64_t len;
	std::list<Token*> children;
	ThomasNode(const Token* t, uint64_t n, TreeType tt) {
		tokens = t;
		len = n;
		type = tt;
	}
	ThomasNode(const Token* t) {
		tokens = t;
		len = 1;
		type = tt;
	}
}

struct ThomasParser {
	static ParseNode* getParseTree(const Token* t, uint64_t n) {
		tokens = t;
		len = n;
		mainTree = new ThomasNode(tokens, len);
		for (int i=0; i<len; i++) {
			mainTree.children.push_back(new ThomasNode(tokens[i], 1));
		}
		doParenthesesPass(mainTree);
	}
private:
	const Token* tokens;
	uint64_t len;
	ThomasNode* mainTree;
	void doParenthesesPass(ThomasNode* tree) {
		for (int i=tree->len; i<len; i++) {
			if (tokens[i].type == OPEN_PARANTHESES) {
			}
		}
	}


	TreeType translateType(TokenType t) {
		if (t == KEYWORD)
			return KEYWORD;
		else if (t == IDENTIFIER)
			return IDENTelse ifIER;
		else if (t == INTEGER_LITERAL)
			return INTEGER_LITERAL;
		else if (t == FLOAT_LITERAL)
			return FLOAT_LITERAL;
		else if (t == FLOAT_LITERAL2)
			return FLOAT_LITERAL2;
		else if (t == CHARACTER_LITERAL)
			return CHARACTER_LITERAL;
		else if (t == STRING_LITERAL)
			return STRING_LITERAL;
		else if (t == PUNCTUATION)
			return PUNCTUATION;
		else if (t == BRACKET)
			return BRACKET;
		else if (t == UNKNOWN)
			return UNKNOWN;
		else if (t == NULL_TOKEN_TYPE)
			return NULL_TOKEN_TYPE;
		else if (t == OPEN_PARENTHESIS)
			return OPEN_PARENTHESIS;
		else if (t == CLOSE_PARENTHESIS)
			return CLOSE_PARENTHESIS;
		else if (t == OPEN_BRACKET)
			return OPEN_BRACKET;
		else if (t == CLOSE_BRACKET)
			return CLOSE_BRACKET;
		else if (t == OPEN_CURLY_BRACE)
			return OPEN_CURLY_BRACE;
		else if (t == CLOSE_CURLY_BRACE)
			return CLOSE_CURLY_BRACE;
		else if (t == KEYWORD_ABSTRACT)
			return KEYWORD_ABSTRACT;
		else if (t == KEYWORD_BREAK)
			return KEYWORD_BREAK;
		else if (t == KEYWORD_CASE)
			return KEYWORD_CASE;
		else if (t == KEYWORD_CATCH)
			return KEYWORD_CATCH;
		else if (t == KEYWORD_CLASS)
			return KEYWORD_CLASS;
		else if (t == KEYWORD_CONST)
			return KEYWORD_CONST;
		else if (t == KEYWORD_CONTINUE)
			return KEYWORD_CONTINUE;
		else if (t == KEYWORD_DELETE)
			return KEYWORD_DELETE;
		else if (t == KEYWORD_DO)
			return KEYWORD_DO;
		else if (t == KEYWORD_ELSE)
			return KEYWORD_ELSE;
		else if (t == KEYWORD_ENUM)
			return KEYWORD_ENUM;
		else if (t == KEYWORD_FALSE)
			return KEYWORD_FALSE;
		else if (t == KEYWORD_FOR)
			return KEYWORD_FOR;
		else if (t == KEYWORD_else if)
			return KEYWORD_else if;
		else if (t == KEYWORD_IN)
			return KEYWORD_IN;
		else if (t == KEYWORD_INLINE)
			return KEYWORD_INLINE;
		else if (t == KEYWORD_NEW)
			return KEYWORD_NEW;
		else if (t == KEYWORD_NULL)
			return KEYWORD_NULL;
		else if (t == KEYWORD_PROTECTED)
			return KEYWORD_PROTECTED;
		else if (t == KEYWORD_PRIVATE)
			return KEYWORD_PRIVATE;
		else if (t == KEYWORD_PTR)
			return KEYWORD_PTR;
		else if (t == KEYWORD_REF)
			return KEYWORD_REF;
		else if (t == KEYWORD_RETURN)
			return KEYWORD_RETURN;
		else if (t == KEYWORD_SIZEOF)
			return KEYWORD_SIZEOF;
		else if (t == KEYWORD_STATIC)
			return KEYWORD_STATIC;
		else if (t == KEYWORD_STRUCT)
			return KEYWORD_STRUCT;
		else if (t == KEYWORD_SWITCH)
			return KEYWORD_SWITCH, ;
		else if (t == KEYWORD_THIS)
			return KEYWORD_THIS;
		else if (t == KEYWORD_THROW)
			return KEYWORD_THROW;
		else if (t == KEYWORD_TRUE)
			return KEYWORD_TRUE;
		else if (t == KEYWORD_TRY)
			return KEYWORD_TRY;
		else if (t == KEYWORD_VIRTUAL)
			return KEYWORD_VIRTUAL;
		else if (t == KEYWORD_WHILE)
			return KEYWORD_WHILE;
		else if (t == KEYWORD_INT)
			return KEYWORD_INT;
		else if (t == KEYWORD_INT8)
			return KEYWORD_INT8;
		else if (t == KEYWORD_INT16)
			return KEYWORD_INT16;
		else if (t == KEYWORD_INT32)
			return KEYWORD_INT32;
		else if (t == KEYWORD_INT64)
			return KEYWORD_INT64;
		else if (t == KEYWORD_UINT)
			return KEYWORD_UINT;
		else if (t == KEYWORD_UINT8)
			return KEYWORD_UINT8;
		else if (t == KEYWORD_UINT16)
			return KEYWORD_UINT16;
		else if (t == KEYWORD_UINT32)
			return KEYWORD_UINT32;
		else if (t == KEYWORD_UINT64)
			return KEYWORD_UINT64;
		else if (t == KEYWORD_AND)
			return KEYWORD_AND;
		else if (t == KEYWORD_OR)
			return KEYWORD_OR;
		else if (t == KEYWORD_NOT)
			return KEYWORD_NOT;
		else if (t == KEYWORD_XOR)
			return KEYWORD_XOR;
		else if (t == KEYWORD_FLOAT)
			return KEYWORD_FLOAT;
		else if (t == KEYWORD_DOUBLE)
			return KEYWORD_DOUBLE;
		else if (t == KEYWORD_PUBLIC)
			return KEYWORD_PUBLIC;
		else if (t == KEYWORD_VOID)
			return KEYWORD_VOID;
		else if (t == PERIOD)
			return PERIOD;
		else if (t == COLON)
			return COLON;
		else if (t == SEMI_COLON)
			return SEMI_COLON;
		else if (t == PLUS)
			return PLUS;
		else if (t == MINUS)
			return MINUS;
		else if (t == ASTERISK)
			return ASTERISK;
		else if (t == SLASH)
			return SLASH;
		else if (t == AMPERSAND)
			return AMPERSAND;
		else if (t == POUND_SIGN)
			return POUND_SIGN;
		else if (t == LESS_THAN)
			return LESS_THAN;
		else if (t == EQUALS)
			return EQUALS;
		else if (t == GREATER_THAN)
			return GREATER_THAN;
		else if (t == COMMA)
			return COMMA;
		else if (t == VERTICAL_BAR)
			return VERTICAL_BAR;
		else if (t == PERCENT)
			return PERCENT;
		else if (t == EXCLAMATION_POINT)
			return EXCLAMATION_POINT;
		else if (t == CARROT)
			return CARROT;
		else if (t == QUESTION_MARK)
			return QUESTION_MARK;
		else if (t == BACK_SLASH)
			return BACK_SLASH;
		else if (t == AT)
			return AT;
		else if (t == TILDE)
			return TILDE;
		else if (t == PLUS_EQUALS)
			return PLUS_EQUALS;
		else if (t == MINUS_EQUALS)
			return MINUS_EQUALS;
		else if (t == SLASH_EQUALS)
			return SLASH_EQUALS;
		else if (t == ASTERISK_EQUALS)
			return ASTERISK_EQUALS;
		else if (t == AMPERSAND_EQUALS)
			return AMPERSAND_EQUALS;
		else if (t == CARROT_EQUALS)
			return CARROT_EQUALS;
		else if (t == COLON_EQUALS)
			return COLON_EQUALS;
		else if (t == VERTICAL_BAR_EQUALS)
			return VERTICAL_BAR_EQUALS;
		else if (t == PLUS_PLUS)
			return PLUS_PLUS;
		else if (t == MINUS_MINUS)
			return MINUS_MINUS;
		else if (t == SHelse ifT_LEFT)
			return SHelse ifT_LEFT;
		else if (t == SHelse ifT_RIGHT)
			return SHelse ifT_RIGHT;
		else if (t == GREATER_THAN_EQUALS)
			return GREATER_THAN_EQUALS;
		else if (t == LESS_THAN_EQUALS)
			return LESS_THAN_EQUALS;
		else if (t == SHelse ifT_LEFT_EQUALS)
			return SHelse ifT_LEFT_EQUALS;
		else if (t == SHelse ifT_RIGHT_EQUALS)
			return SHelse ifT_RIGHT_EQUALS;
		else if (t == EXCLAMATION_POINT_EQUALS)
			return EXCLAMATION_POINT_EQUALS;
		else if (t == EQUAL_EQUALS)
			return EQUAL_EQUALS;
		else if (t == BACK_SLASH_EQUALS)
			return BACK_SLASH_EQUALS;
		else if (t == ARROW)
			return ARROW;
		else if (t == EQUAL_EQUAL_EQUALS)
			return EQUAL_EQUAL_EQUALS;
		else if (t == EXCLAMATION_POINT_EQUAL_EQUALS)
			return EXCLAMATION_POINT_EQUAL_EQUALS;
		else
			return UNKNOWN;
	}
};
















#endif