#pragma once

#include <string>

enum TokenType {
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
	EXCLAMATION_POINT_EQUAL_EQUALS,
	POSITIVE,
	NEGATIVE,
	IDENTIFIER_CLASS,
	IDENTIFIER_NONCLASS
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
	const bool halfEquals(Token ty) {
		if(ty.str == "" || str == "")
			return type == ty.type;
		return equals(ty);
	}
	const bool equals(Token ty) {
		if(type == ty.type && str == ty.str)
			return true;
		return false;
	}
	const std::string toString() {
		return "(" + toString(type) + " '" + str + "' ~ " + std::to_string(lineNum) + "," + std::to_string(charNum) + ")";
	}
	TokenType type;
	std::string str;
	long lineNum;
	long charNum;

	static std::string toString(const TokenType& t) {
		std::string arr[119] = { "KEYWORD", "IDENTIFIER", "INTEGER_LITERAL", "FLOAT_LITERAL", "FLOAT_LITERAL2", "CHARACTER_LITERAL", "STRING_LITERAL", "PUNCTUATION", "BRACKET", "UNKNOWN", "NULL_TOKEN_TYPE", "OPEN_PARENTHESIS", "CLOSE_PARENTHESIS", "OPEN_BRACKET", "CLOSE_BRACKET", "OPEN_CURLY_BRACE", "CLOSE_CURLY_BRACE", "KEYWORD_ABSTRACT", "KEYWORD_BREAK", "KEYWORD_CASE", "KEYWORD_CATCH", "KEYWORD_CLASS", "KEYWORD_CONST", "KEYWORD_CONTINUE", "KEYWORD_DELETE", "KEYWORD_DO", "KEYWORD_ELSE", "KEYWORD_ENUM", "KEYWORD_FALSE", "KEYWORD_FOR", "KEYWORD_IF", "KEYWORD_IN", "KEYWORD_INLINE", "KEYWORD_NEW", "KEYWORD_NULL", "KEYWORD_PROTECTED", "KEYWORD_PRIVATE", "KEYWORD_PTR", "KEYWORD_REF", "KEYWORD_RETURN", "KEYWORD_SIZEOF", "KEYWORD_STATIC", "KEYWORD_STRUCT", "KEYWORD_SWITCH,", "KEYWORD_THIS", "KEYWORD_THROW", "KEYWORD_TRUE", "KEYWORD_TRY", "KEYWORD_VIRTUAL", "KEYWORD_WHILE", "KEYWORD_INT", "KEYWORD_INT8", "KEYWORD_INT16", "KEYWORD_INT32", "KEYWORD_INT64", "KEYWORD_UINT", "KEYWORD_UINT8", "KEYWORD_UINT16", "KEYWORD_UINT32", "KEYWORD_UINT64", "KEYWORD_AND", "KEYWORD_OR", "KEYWORD_NOT", "KEYWORD_XOR", "KEYWORD_FLOAT", "KEYWORD_DOUBLE", "KEYWORD_PUBLIC", "KEYWORD_VOID", "PERIOD", "COLON", "SEMI_COLON", "PLUS", "MINUS", "ASTERISK", "SLASH", "AMPERSAND", "POUND_SIGN", "LESS_THAN", "EQUALS", "GREATER_THAN", "COMMA", "VERTICAL_BAR", "PERCENT", "EXCLAMATION_POINT", "CARROT", "QUESTION_MARK", "BACK_SLASH", "AT", "TILDE", "PLUS_EQUALS", "MINUS_EQUALS", "SLASH_EQUALS", "ASTERISK_EQUALS", "AMPERSAND_EQUALS", "CARROT_EQUALS", "COLON_EQUALS", "VERTICAL_BAR_EQUALS", "PLUS_PLUS", "MINUS_MINUS", "SHIFT_LEFT", "SHIFT_RIGHT", "GREATER_THAN_EQUALS", "LESS_THAN_EQUALS", "SHIFT_LEFT_EQUALS", "SHIFT_RIGHT_EQUALS", "EXCLAMATION_POINT_EQUALS", "EQUAL_EQUALS", "BACK_SLASH_EQUALS", "ARROW", "EQUAL_EQUAL_EQUALS", "EXCLAMATION_POINT_EQUAL_EQUALS", "root", "classImplementation", "functionImplementation", "variable_declaration", "public_colon", "protected_colon", "private_colon" };
		return arr[int(t)];
	}
	static std::string toString(const Token& t) {
		return toString(t.type);
	}

	friend std::ostream& operator<<(std::ostream& stream, const Token& t) {
		return stream << Token::toString(t);
	}

};