#pragma once

#include <map>
#include <set>

#include "Parser.hpp"

// struct CompilerDeduction {
// 	CompilerDeduction(StringTree *(*func_ptr) (ParseNode*)) {
// 		func = func_ptr;
// 	}
// 	CompilerDeduction(std::string string) {
// 		str = string;
// 	}
// 	std::string str;
// 	StringTree *(*func) (ParseNode*) = nullptr;
// };

class Compiler {
public:
	Compiler();
	std::string compile(ParseNode* tree);
private:
	std::string output;
	void compile_tree(ParseNode* tree);
	void compile_children(ParseNode* tree);
	void error(std::string message, ParseNode* tree);
	std::map<TreeType, std::string> simple_strings = {
		{P_KEYWORD_ABSTRACT, "abstract"},
		{P_KEYWORD_BREAK, "break"},
		{P_KEYWORD_DEFAULT, "default"},
		{P_KEYWORD_CASE, "case"},
		{P_KEYWORD_CATCH, "catch"},
		{P_KEYWORD_CLASS, "class"},
		{P_KEYWORD_NAMESPACE, "namespace"},
		{P_KEYWORD_CONST, "const"},
		{P_KEYWORD_CONTINUE, "continue"},
		{P_KEYWORD_DELETE, "delete"},
		{P_KEYWORD_DO, "do"},
		{P_KEYWORD_ELSE, "else"},
		{P_KEYWORD_ENUM, "enum"},
		{P_KEYWORD_FALSE, "false"},
		{P_KEYWORD_FOR, "for"},
		{P_KEYWORD_IF, "if"},
		{P_KEYWORD_IN, "in"},
		{P_KEYWORD_INLINE, "inline"},
		{P_KEYWORD_NEW, "new"},
		{P_KEYWORD_NULL, "null"},
		{P_KEYWORD_PROTECTED, "protected"},
		{P_KEYWORD_PRIVATE, "private"},
		{P_KEYWORD_PTR, "ptr"},
		{P_KEYWORD_REF, "ref"},
		{P_KEYWORD_RETURN, "return"},
		{P_KEYWORD_SIZEOF, "sizeof"},
		{P_KEYWORD_STATIC, "static"},
		{P_KEYWORD_STRUCT, "struct"},
		{P_KEYWORD_SWITCH, "switch"},
		{P_KEYWORD_THIS, "this"},
		{P_KEYWORD_THROW, "throw"},
		{P_KEYWORD_TRUE, "true"},
		{P_KEYWORD_TRY, "try"},
		{P_KEYWORD_VIRTUAL, "virtual"},
		{P_KEYWORD_WHILE, "while"},
		{P_KEYWORD_INT, "int64_t"},
		{P_KEYWORD_INT8, "int8_t"},
		{P_KEYWORD_INT16, "int16_t"},
		{P_KEYWORD_INT32, "int32_t"},
		{P_KEYWORD_UINT, "uint64_t"},
		{P_KEYWORD_UINT8, "uint8_t"},
		{P_KEYWORD_UINT16, "uint16_t"},
		{P_KEYWORD_UINT32, "uint32_t"},
		{P_KEYWORD_IMPORT, "include"},
		{P_KEYWORD_AND, "and"},
		{P_KEYWORD_OR, "or"},
		{P_KEYWORD_NOT, "not"},
		{P_KEYWORD_XOR, "xor"},
		{P_KEYWORD_FLOAT, "float"},
		{P_KEYWORD_DOUBLE, "double"},
		{P_KEYWORD_PUBLIC, "public"},
		{P_KEYWORD_VOID, "void"},
		{P_PERIOD, "."},
		{P_COLON, ","},
		{P_SEMI_COLON, ";"},
		{P_PLUS, "+"},
		{P_MINUS, "-"},
		{P_ASTERISK, "*"},
		{P_SLASH, "/"},
		{P_AMPERSAND, "&"},
		{P_POUND_SIGN, "#"},
		{P_LESS_THAN, "<"},
		{P_EQUALS, "="},
		{P_GREATER_THAN, ">"},
		{P_COMMA, ","},
		{P_VERTICAL_BAR, "|"},
		{P_PERCENT, "%"},
		{P_EXCLAMATION_POINT, "!"},
		{P_CARROT, "^"},
		{P_QUESTION_MARK, "?"},
		{P_BACK_SLASH, "\\"},
		{P_AT, "@"},
		{P_TILDE, "~"},
		{P_PLUS_EQUALS, "+="},
		{P_MINUS_EQUALS, "-="},
		{P_SLASH_EQUALS, "/="},
		{P_ASTERISK_EQUALS, "*="},
		{P_AMPERSAND_EQUALS, "&="},
		{P_CARROT_EQUALS, "^="},
		{P_COLON_EQUALS, ":="},
		{P_VERTICAL_BAR_EQUALS, "|="},
		{P_PERCENT_EQUALS, "%="},
		{P_PLUS_PLUS, "+="},
		{P_MINUS_MINUS, "--"},
		{P_SHIFT_LEFT, "<<="},
		{P_SHIFT_RIGHT, ">>="},
		{P_GREATER_THAN_EQUALS, ">="},
		{P_LESS_THAN_EQUALS, "<="},
		{P_SHIFT_LEFT_EQUALS, "<<="},
		{P_SHIFT_RIGHT_EQUALS, ">>="},
		{P_EXCLAMATION_POINT_EQUALS, "!="},
		{P_EQUAL_EQUALS, "=="},
		{P_BACK_SLASH_EQUALS, "\\="},
		{P_ARROW, "->"},
		{P_EQUAL_EQUAL_EQUALS, "==="},
		{P_EXCLAMATION_POINT_EQUAL_EQUALS, "!=="},
		{P_POSITIVE, "+"},
		{P_NEGATIVE, "-"},
		{P_PTR, "*"},
		{P_KEYWORD_BOOL, "bool"},
		{P_KEYWORD_CHAR, "char"},
		{P_KEYWORD_VAR, "var"},
		{P_KEYWORD_FUNCTION, "function"},
		{P_KEYWORD_REPEAT, "repeat"},
		{P_EXTENDS, "extends"},
		{P_LEFT_ARROW, "<-"},
		{P_KEYWORD_IS, "is"},
		{P_ASTERISK_ASTERISK, "**"},
		{P_ASTERISK_ASTERISK_EQUALS, "**="}
	};

	std::set<TreeType> just_compile_space_and_str = {
		P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL
	};

	std::set<TreeType> just_compile_children = {
		variable_dec, unary1_clause, unary2_clause,
		mult_clause, plus_clause, shift_clause,
		inequality_clause, equality_clause, bitwise_and_clause,
		bitwise_xor_clause, bitwise_or_clause, setting_clause,
		ternary_clause, comma_clause, statement
	};

	std::set<TreeType> vanilla_curly_brace_block = {
		block_of_statements_or_class,
		enum_block,
		bracket_access
	};
};
