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
    etc = 0,
    etc_not,
    NEWLINE,
    KEYWORD,
    IDENTIFIER,
	INTEGER_LITERAL,
    FLOAT_LITERAL,
    CHARACTER_LITERAL,
    STRING_LITERAL,
	PUNCTUATION,
    BRACKET = 10,
    UNKNOWN,
    NULL_TOKEN_TYPE,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	OPEN_CURLY_BRACE,
	CLOSE_CURLY_BRACE,
	KEYWORD_ABSTRACT,
	KEYWORD_BREAK = 20,
	KEYWORD_CASE,
	KEYWORD_CATCH,
	KEYWORD_CLASS,
	KEYWORD_CONST,
	KEYWORD_CONTINUE,
	KEYWORD_DELETE,
	KEYWORD_DO,
	KEYWORD_ELSE,
	KEYWORD_ENUM,
	KEYWORD_FALSE = 30,
	KEYWORD_FOR,
	KEYWORD_IF,
	KEYWORD_IN,
	KEYWORD_INLINE,
	KEYWORD_NEW,
	KEYWORD_NULL,
	KEYWORD_PROTECTED,
	KEYWORD_PRIVATE,
	KEYWORD_PTR,
	KEYWORD_REF = 40,
	KEYWORD_RETURN,
	KEYWORD_SIZEOF,
	KEYWORD_STATIC,
	KEYWORD_STRUCT,
	KEYWORD_SWITCH, 
	KEYWORD_THIS,
	KEYWORD_THROW,
	KEYWORD_TRUE,
	KEYWORD_TRY,
	KEYWORD_VIRTUAL = 50,
	KEYWORD_WHILE,
	KEYWORD_INT,
	KEYWORD_INT8,
	KEYWORD_INT16,
	KEYWORD_INT32,
	KEYWORD_INT64,
	KEYWORD_UINT,
	KEYWORD_UINT8,
	KEYWORD_UINT16,
	KEYWORD_UINT32 = 60,
	KEYWORD_UINT64,
	KEYWORD_AND,
	KEYWORD_OR,
	KEYWORD_NOT,
	KEYWORD_XOR,
	KEYWORD_FLOAT,
	KEYWORD_DOUBLE,
	KEYWORD_PUBLIC,
	PERIOD,
	SEMI_COLON = 70,
	PLUS,
	MINUS,
	ASTERISK,
	SLASH,
	AMPERSAND,
	POUND_SIGN,
	LESS_THAN,
	EQUALS,
	GREATER_THAN,
	COMMA = 80,
	VERTICAL_BAR,
	PERCENT,
	EXCLAMATION_POINT,
	AT,
	CARROT,
	COLON,
	QUESTION_MARK,
	END_OF_LINE,
	START_OF_BLOCK,
	END_OF_BLOCK = 90,
	general,
	in_class,
	klass,
	method_declaration,
	method_declaration_and_implementation,
	member_variable_declaration,
	t_arg_d,
	method_args,
	block,
	type_name = 100,
	while_loop,
	for_in_loop,
	if_statement,
	literal,
	if_else_statement,
	block_components,
	assignment_expression,
	assignment_set,
	or_expression,
	and_expression = 110,
	xor_expression,
	equality_expression,
	equality_set,
	relational_expression,
	relational_set,
	shift_expression,
	shift_set,
	plus_expression,
	plus_set,
	times_expression = 120,
	times_set,
	postfix_expression,
	pointer_value,
	access_value,
	pointer_access,
	function_call,
	function_arg,
	t_args,
	ta,
	t_arg = 130,
	line,
	program,
	expression,
    template_parameter,
    template_parameter_child,
    function_parameter,
    type,
	unary_expression,
	access_modifier_set,
	postfix_expression_two = 140,
	array,
	dictionary,
	dictionary_arg,
	all_ints,
	conditional_expression
};

const size_t token_threshold = 91;

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
	TokenType type;
	std::string str;
	long lineNum;
	long charNum;
	
	static std::string tokenTypeToString(const TokenType& t) {
		std::string arr[146] = {"etc", "etc_not", "NEWLINE", "KEYWORD", "IDENTIFIER", "INTEGER_LITERAL", "FLOAT_LITERAL", "CHARACTER_LITERAL", "STRING_LITERAL", "PUNCTUATION", "BRACKET", "UNKNOWN", "NULL_TOKEN_TYPE", "OPEN_PARENTHESIS", "CLOSE_PARENTHESIS", "OPEN_BRACKET", "CLOSE_BRACKET", "OPEN_CURLY_BRACE", "CLOSE_CURLY_BRACE", "KEYWORD_ABSTRACT", "KEYWORD_BREAK", "KEYWORD_CASE", "KEYWORD_CATCH", "KEYWORD_CLASS", "KEYWORD_CONST", "KEYWORD_CONTINUE", "KEYWORD_DELETE", "KEYWORD_DO", "KEYWORD_ELSE", "KEYWORD_ENUM", "KEYWORD_FALSE", "KEYWORD_FOR", "KEYWORD_IF", "KEYWORD_IN", "KEYWORD_INLINE", "KEYWORD_NEW", "KEYWORD_NULL", "KEYWORD_PROTECTED", "KEYWORD_PRIVATE", "KEYWORD_PTR", "KEYWORD_REF", "KEYWORD_RETURN", "KEYWORD_SIZEOF", "KEYWORD_STATIC", "KEYWORD_STRUCT", "KEYWORD_SWITCH", "KEYWORD_THIS", "KEYWORD_THROW", "KEYWORD_TRUE", "KEYWORD_TRY", "KEYWORD_VIRTUAL", "KEYWORD_WHILE", "KEYWORD_INT", "KEYWORD_INT8", "KEYWORD_INT16", "KEYWORD_INT32", "KEYWORD_INT64", "KEYWORD_UINT", "KEYWORD_UINT8", "KEYWORD_UINT16", "KEYWORD_UINT32", "KEYWORD_UINT64", "KEYWORD_AND", "KEYWORD_OR", "KEYWORD_NOT", "KEYWORD_XOR", "KEYWORD_FLOAT", "KEYWORD_DOUBLE", "KEYWORD_PUBLIC", "PERIOD", "SEMI_COLON", "PLUS", "MINUS", "ASTERISK", "SLASH", "AMPERSAND", "POUND_SIGN", "LESS_THAN", "EQUALS", "GREATER_THAN", "COMMA", "VERTICAL_BAR", "PERCENT", "EXCLAMATION_POINT", "AT", "CARROT", "COLON", "QUESTION_MARK", "END_OF_LINE", "START_OF_BLOCK", "END_OF_BLOCK", "general", "in_class", "klass", "method_declaration", "method_declaration_and_implementation", "member_variable_declaration", "t_arg_d", "method_args", "block", "type_name", "while_loop", "for_in_loop", "if_statement", "literal", "if_else_statement", "block_components", "assignment_expression", "assignment_set", "or_expression", "and_expression", "xor_expression", "equality_expression", "equality_set", "relational_expression", "relational_set", "shift_expression", "shift_set", "plus_expression", "plus_set", "times_expression", "times_set", "postfix_expression", "pointer_value", "access_value", "pointer_access", "function_call", "function_arg", "t_args", "ta", "t_arg", "line", "program", "expression", "template_parameter", "template_parameter_child", "function_parameter", "type", "unary_expression", "access_modifier_set", "postfix_expression_two", "array", "dictionary", "dictionary_arg", "all_ints", "conditional_expression"};
		return arr[int(t)];
	}
};


#endif /* Token_cpp */
