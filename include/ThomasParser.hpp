#ifndef THOMAS_PARSER_HPP
#define THOMAS_PARSER_HPP

#include <assert.h>
#include <iostream>
#include <stack>
#include <list>
#include <set>
#include <map>
#include <vector>

#include "Token.hpp"

enum TreeType {
	T_KEYWORD,
	T_IDENTIFIER,
	T_CLASS_IDENTIFIER,
	T_FUNC_DECL_IDENTIFIER,
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
	T_KEYWORD_DEFAULT,
	T_KEYWORD_CASE,
	T_KEYWORD_CATCH,
	T_KEYWORD_CLASS,
	T_KEYWORD_NAMESPACE,
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
	T_KEYWORD_UINT,
	T_KEYWORD_UINT8,
	T_KEYWORD_UINT16,
	T_KEYWORD_UINT32,
	T_KEYWORD_IMPORT,
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
	T_PERCENT_EQUALS,
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
	T_POSITIVE,
	T_NEGATIVE,
	T_PTR,
	T_KEYWORD_BOOL,
	T_KEYWORD_CHAR,
	T_KEYWORD_VAR,
	T_KEYWORD_FUNCTION,
	T_KEYWORD_REPEAT,
	T_OPEN_TEMPLATE,
	T_CLOSE_TEMPLATE,
	T_EXTENDS,
	T_LEFT_ARROW,
	T_KEYWORD_IS,
	general,				// make sure this is the first non-token type
	curly_brace_block,
	parenthesis_block,
	bracket_block,
	template_block,
	unary1_clause,
	unary2_clause,
	mult_clause,
	plus_clause,
	shift_clause,
	inequality_clause,
	equality_clause,
	bitwise_and_clause,
	bitwise_xor_clause,
	bitwise_or_clause,
	setting_clause,
	ternary_clause,
	comma_clause,
	statement,

	enum_implementation,
	namespace_implementation,
	class_implementation,
	constructor_implementation,
	function_implementation,
	function_declaration,
	for_loop,
	while_loop,
	do_while_loop,
	if_statement,
	if_else_statement,
	switch_statement,
	return_statement,
	break_statement,
	continue_statement,
	case_statement,
	variable_dec,
	try_block,
	catch_block,
	colon_clause,

	raw_type,				// make sure I'm the first "shortcut" enum and that all later enums are also "shortcuts"
	raw_type_or_void,
	int_type,
	unary1_value,
	unary2_value,
	plus_value,
	mult_value,
	shift_value,
	inequality_value,
	equality_value,
	bitwise_and_value,
	bitwise_xor_value,
	bitwise_or_value,
	setting_value,
	any_integer_type,
	structure,
	structure_or_statement,
	colon_value,
	comma_value				// make sure I'm the last enum
};

std::string treeTypeToString(TreeType t);

TreeType translateType(TokenType t);

struct ThomasNode {
	TreeType type;
	Token token;
	std::list<ThomasNode*> children;
	ThomasNode(TreeType tt) {
		type = tt;
	}
	ThomasNode(Token t);
	void print() {
		print(0);
	}
	void print(int depth) {
		std::string indent = "";
		for (int i = 0; i < depth; i++)
			indent += "   ";
		std::cout << indent << treeTypeToString(type) << " : " << token.str << "\n";
		for (std::list<ThomasNode*>::const_iterator it = children.begin(), end = children.end(); it != end; ++it)
		    (*it)->print(depth+1);
	}
};

class ThomasParseRule {
public:
	ThomasParseRule(int p, std::set<TreeType> par, std::set<TreeType> notPar, std::vector<TreeType> f, TreeType t) {
		precedence = p;
		parents = par;
		notParents = notPar;
		from = f;
		to = t;
	}
	std::string toString() {
		std::string rtn = "{set-o'-things}";
		rtn += " : ";
		for (int i = 0; i < from.size(); i++) {
			if (i != 0)
				rtn += ", ";
			rtn += treeTypeToString(from[i]);
		}
		rtn += " : ";
		rtn += treeTypeToString(to);
		return rtn;
	}
	int precedence;
	std::set<TreeType> parents;
	std::set<TreeType> notParents;
	std::vector<TreeType> from;
	TreeType to;
};

class ThomasParser {
private:
	const Token* tokens;
	uint64_t len;
	ThomasNode* mainTree;
	void doCurlyBracePass(ThomasNode* tree);
	void doParenthesesPass(ThomasNode* tree);
	void doBracketPass(ThomasNode* tree);
	void doTemplatePass(ThomasNode* tree);
	std::vector<ThomasParseRule> rules;
	std::vector<bool>leftRight;
	int firstToken = raw_type;
	int lastToken = comma_value;
	static bool thomasParserPrecedenceSorter(ThomasParseRule r1, ThomasParseRule r2);
public:
	std::set<TreeType> shortcuts[200];
	ThomasParser(std::vector<bool> lr, std::vector<ThomasParseRule> r) {
		rules = r;
		std::sort(rules.begin(), rules.end(), thomasParserPrecedenceSorter);
		leftRight = lr;
		// shortcuts[value] = {T_IDENTIFIER};
		shortcuts[raw_type] = {T_KEYWORD_INT, T_KEYWORD_INT8, T_KEYWORD_INT16, T_KEYWORD_INT32, T_KEYWORD_UINT, T_KEYWORD_UINT8, T_KEYWORD_UINT16, T_KEYWORD_UINT32, T_KEYWORD_CHAR, T_KEYWORD_BOOL, T_KEYWORD_FLOAT, T_KEYWORD_DOUBLE, T_KEYWORD_VAR, T_CLASS_IDENTIFIER};
		shortcuts[raw_type_or_void] = {T_KEYWORD_VOID, T_KEYWORD_INT, T_KEYWORD_INT8, T_KEYWORD_INT16, T_KEYWORD_INT32, T_KEYWORD_UINT, T_KEYWORD_UINT8, T_KEYWORD_UINT16, T_KEYWORD_UINT32, T_KEYWORD_CHAR, T_KEYWORD_BOOL, T_KEYWORD_FLOAT, T_KEYWORD_DOUBLE, T_KEYWORD_VAR, T_CLASS_IDENTIFIER};
		shortcuts[int_type] = {T_KEYWORD_INT, T_KEYWORD_INT8, T_KEYWORD_INT16, T_KEYWORD_INT32, T_KEYWORD_UINT, T_KEYWORD_UINT8, T_KEYWORD_UINT16, T_KEYWORD_UINT32};
		shortcuts[unary1_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause};
		shortcuts[unary2_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause};
		shortcuts[mult_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause};
		shortcuts[plus_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause};
		shortcuts[shift_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause};
		shortcuts[inequality_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause};
		shortcuts[equality_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause};
		shortcuts[bitwise_and_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause};
		shortcuts[bitwise_xor_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause};
		shortcuts[bitwise_or_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause, bitwise_or_clause};
		shortcuts[setting_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause, bitwise_or_clause, setting_clause, ternary_clause};
		shortcuts[colon_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause, bitwise_or_clause, setting_clause, ternary_clause, colon_clause};
		shortcuts[any_integer_type] = {T_KEYWORD_INT, T_KEYWORD_INT8, T_KEYWORD_INT16, T_KEYWORD_INT32, T_KEYWORD_UINT, T_KEYWORD_UINT8, T_KEYWORD_UINT16, T_KEYWORD_UINT32, T_KEYWORD_CHAR, T_KEYWORD_BOOL};
		shortcuts[comma_value] = {T_IDENTIFIER, T_INTEGER_LITERAL, T_FLOAT_LITERAL, T_STRING_LITERAL, parenthesis_block, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause, bitwise_or_clause, setting_clause, ternary_clause, colon_clause, comma_clause};
		shortcuts[structure] = {statement, for_loop, while_loop, do_while_loop, if_statement, if_else_statement, curly_brace_block};
		shortcuts[structure_or_statement] = {statement, for_loop, while_loop, do_while_loop, if_statement, if_else_statement, curly_brace_block, variable_dec};

	// listOfRules.push_back(ThomasParseRule(10, general, {T_FLOAT_LITERAL}, value));
	// listOfRules.push_back(ThomasParseRule(10, general, {T_IDENTIFIER}, value));
	// listOfRules.push_back(ThomasParseRule(10, general, {T_STRING_LITERAL
	}
	ThomasNode* getParseTree(const Token* t, uint64_t n);
	void parse();
	void parseLeftRight(ThomasNode *tree, int from, int to);
	void parseRightLeft(ThomasNode *tree, int from, int to);
	void error(std::string message, ThomasNode* tree);

};

#endif
