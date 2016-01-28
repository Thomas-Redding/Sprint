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
	INDENT,
    DEDENT,
    NEWLINE,
    KEYWORD,
    IDENTIFIER,
	INTEGER_LITERAL,
    FLOAT_LITERAL,
    CHARACTER_LITERAL,
    STRING_LITERAL = 10,
	PUNCTUATION,
    BRACKET,
    UNKNOWN,
    NULL_TOKEN_TYPE,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	OPEN_CURLY_BRACE,
	CLOSE_CURLY_BRACE = 20,
	KEYWORD_ABSTRACT,
	KEYWORD_BREAK,
	KEYWORD_CASE,
	KEYWORD_CATCH,
	KEYWORD_CLASS,
	KEYWORD_CONST,
	KEYWORD_CONTINUE,
	KEYWORD_DELETE,
	KEYWORD_DO,
	KEYWORD_ELSE = 30,
	KEYWORD_ENUM,
	KEYWORD_FALSE,
	KEYWORD_FOR,
	KEYWORD_IF,
	KEYWORD_IN,
	KEYWORD_INLINE,
	KEYWORD_NEW,
	KEYWORD_NULL,
	KEYWORD_PROTECTED,
	KEYWORD_PRIVATE = 40,
	KEYWORD_PTR,
	KEYWORD_REF,
	KEYWORD_RETURN,
	KEYWORD_SIZEOF,
	KEYWORD_STATIC,
	KEYWORD_STRUCT,
	KEYWORD_SWITCH, 
	KEYWORD_THIS,
	KEYWORD_THROW,
	KEYWORD_TRUE = 50,
	KEYWORD_TRY,
	KEYWORD_VIRTUAL,
	KEYWORD_WHILE,
	KEYWORD_INT,
	KEYWORD_INT8,
	KEYWORD_INT16,
	KEYWORD_INT32,
	KEYWORD_INT64,
	KEYWORD_UINT,
	KEYWORD_UINT8 = 60,
	KEYWORD_UINT16,
	KEYWORD_UINT32,
	KEYWORD_UINT64,
	KEYWORD_AND,
	KEYWORD_OR,
	KEYWORD_NOT,
	KEYWORD_XOR,
	KEYWORD_FLOAT,
	KEYWORD_DOUBLE,
	KEYWORD_PUBLIC = 70,
	PERIOD,
	SEMI_COLON,
	PLUS,
	MINUS,
	ASTERISK,
	SLASH,
	AMPERSAND,
	POUND_SIGN,
	LESS_THAN,
	EQUALS = 80,
	GREATER_THAN,
	COMMA,
	VERTICAL_BAR,
	PERCENT,
	EXCLAMATION_POINT,
	AT,
	CARROT,
	COLON,
	general,
	in_class = 90,
	klass,
	method_declaration,
	method_declaration_and_implementation,
	member_variable_declaration,
	t_arg_d,
	method_args,
	block,
	type_name,
	while_loop,
	for_in_loop = 100,
	if_statement,
	literal,
	if_else_statement,
	block_components,
	assignment_expression,
	assignment_set,
	or_expression,
	and_expression,
	xor_expression,
	equality_expression = 110,
	equality_set,
	relational_expression,
	relational_set,
	shift_expression,
	shift_set,
	plus_expression,
	plus_set,
	times_expression,
	times_set,
	postfix_expression = 120,
	pointer_value,
	access_value,
	pointer_access,
	function_call,
	function_arg,
	t_args,
	ta,
	t_arg,
	line,
	program = 130,
	expression,
    template_parameter,
    template_parameter_child,
    function_parameter,
    type,
	unary_expression,
	access_modifier_set,
	postfix_expression_two,
	array,
	dictionary = 140,
	dictionary_arg,
	all_ints
};

const size_t token_threshold = 89;

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
};

#endif /* Token_cpp */
