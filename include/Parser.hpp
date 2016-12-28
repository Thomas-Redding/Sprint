#ifndef PARSER_HPP
#define PARSER_HPP

#include <assert.h>
#include <iostream>
#include <stack>
#include <list>
#include <set>
#include <map>
#include <vector>

#include "Token.hpp"

enum TreeType {
	P_KEYWORD,
	P_IDENTIFIER,
	P_CLASS_IDENTIFIER,
	P_FUNC_DECL_IDENTIFIER,
	P_INTEGER_LITERAL,
	P_FLOAT_LITERAL,
	P_FLOAT_LITERAL2,
	P_CHARACTER_LITERAL,
	P_STRING_LITERAL,
	P_PUNCTUATION,
	P_BRACKET,
	P_UNKNOWN,
	P_NULL_TOKEN_TYPE,
	P_OPEN_PARENTHESIS,
	P_CLOSE_PARENTHESIS,
	P_OPEN_BRACKET,
	P_CLOSE_BRACKET,
	P_OPEN_CURLY_BRACE,
	P_CLOSE_CURLY_BRACE,
	P_KEYWORD_ABSTRACT,
	P_KEYWORD_BREAK,
	P_KEYWORD_DEFAULT,
	P_KEYWORD_CASE,
	P_KEYWORD_CATCH,
	P_KEYWORD_CLASS,
	P_KEYWORD_NAMESPACE,
	P_KEYWORD_CONST,
	P_KEYWORD_CONTINUE,
	P_KEYWORD_DELETE,
	P_KEYWORD_DO,
	P_KEYWORD_ELSE,
	P_KEYWORD_ENUM,
	P_KEYWORD_FALSE,
	P_KEYWORD_FOR,
	P_KEYWORD_IF,
	P_KEYWORD_IN,
	P_KEYWORD_INLINE,
	P_KEYWORD_MUT,
	P_KEYWORD_NEW,
	P_KEYWORD_NULL,
	P_KEYWORD_PROTECTED,
	P_KEYWORD_PRIVATE,
	P_KEYWORD_PTR,
	P_KEYWORD_REF,
	P_KEYWORD_RETURN,
	P_KEYWORD_SIZEOF,
	P_KEYWORD_STATIC,
	P_KEYWORD_STRUCT,
	P_KEYWORD_SWITCH, 
	P_KEYWORD_THIS,
	P_KEYWORD_THROW,
	P_KEYWORD_TRUE,
	P_KEYWORD_TRY,
	P_KEYWORD_VIRTUAL,
	P_KEYWORD_WHILE,
	P_KEYWORD_INT,
	P_KEYWORD_INT8,
	P_KEYWORD_INT16,
	P_KEYWORD_INT32,
	P_KEYWORD_UINT,
	P_KEYWORD_UINT8,
	P_KEYWORD_UINT16,
	P_KEYWORD_UINT32,
	P_KEYWORD_IMPORT,
	P_KEYWORD_AND,
	P_KEYWORD_OR,
	P_KEYWORD_NOT,
	P_KEYWORD_XOR,
	P_KEYWORD_FLOAT,
	P_KEYWORD_DOUBLE,
	P_KEYWORD_PUBLIC,
	P_KEYWORD_VOID,
	P_PERIOD,
	P_COLON,
	P_SEMI_COLON,
	P_PLUS,
	P_MINUS,
	P_ASTERISK,
	P_SLASH,
	P_AMPERSAND,
	P_POUND_SIGN,
	P_LESS_THAN,
	P_EQUALS,
	P_GREATER_THAN,
	P_COMMA,
	P_VERTICAL_BAR,
	P_PERCENT,
	P_EXCLAMATION_POINT,
	P_CARROT,
	P_QUESTION_MARK,
	P_BACK_SLASH,
	P_AT,
	P_TILDE,
	P_PLUS_EQUALS,
	P_MINUS_EQUALS,
	P_SLASH_EQUALS,
	P_ASTERISK_EQUALS,
	P_AMPERSAND_EQUALS,
	P_CARROT_EQUALS,
	P_COLON_EQUALS,
	P_VERTICAL_BAR_EQUALS,
	P_PERCENT_EQUALS,
	P_PLUS_PLUS,
	P_MINUS_MINUS,
	P_SHIFT_LEFT,
	P_SHIFT_RIGHT,
	P_GREATER_THAN_EQUALS,
	P_LESS_THAN_EQUALS,
	P_SHIFT_LEFT_EQUALS,
	P_SHIFT_RIGHT_EQUALS,
	P_EXCLAMATION_POINT_EQUALS,
	P_EQUAL_EQUALS,
	P_BACK_SLASH_EQUALS,
	P_ARROW,
	P_EQUAL_EQUAL_EQUALS,
	P_EXCLAMATION_POINT_EQUAL_EQUALS,
	P_POSITIVE,
	P_NEGATIVE,
	P_PTR,
	P_KEYWORD_BOOL,
	P_KEYWORD_CHAR,
	P_KEYWORD_VAR,
	P_KEYWORD_FUNCTION,
	P_KEYWORD_REPEAT,
	P_OPEN_TEMPLATE,
	P_CLOSE_TEMPLATE,
	P_EXTENDS,
	P_LEFT_ARROW,
	P_LEFT_RIGHT_ARROW,
	P_KEYWORD_IS,
	P_ASTERISK_ASTERISK,
	P_ASTERISK_ASTERISK_EQUALS,

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
	colon_list,

	enum_implementation,
	namespace_implementation,
	class_declaration,
	class_implementation,
	constructor_implementation,
	function_implementation,
	function_declaration,
	function_head, // e.g. "foo(...) ->" or "foo<...>(...) ->"
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
	colon_type_clause,
	
	block_of_statements_or_class,
	enum_block,
	list_literal,
	set_literal,
	ordered_map_literal,
	unordered_map_literal,
	bracket_access,
	parenthesis,
	templates,
	empty_curly_brace_block,


	raw_type,				// make sure I'm the first "shortcut" enum and that all later enums are also "shortcuts"
	raw_type_or_void,
	// composite_type,
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
	ternary_value,
	setting_value,
	any_integer_type,
	structure,
	structure_or_statement,
	op,
	stuff_in_classes,
	function_name_candidate,
	valid_line,
	comma_value				// make sure I'm the last enum
};

std::string treeTypeToString(TreeType t);

TreeType translateType(TokenType t);

struct ParseNode {
	TreeType type;
	Token token = Token(UNKNOWN);
	std::list<ParseNode*> children;
	ParseNode(TreeType tt, int line, int ch) {
		token.lineNum = line;
		token.charNum = ch;
		type = tt;
	}
	bool is_leaf = false;
	std::list<Token>::iterator last_token;
	ParseNode(std::list<Token>::iterator it, std::list<Token>::iterator last);
	void print() {
		print(0);
	}
	void print(int depth) {
		std::string indent = "";
		for (int i = 0; i < depth; i++)
			indent += "   ";
		std::cout << indent << treeTypeToString(type) << " : " << token.str << " (" << token.lineNum << "," << token.charNum << ")\n";
		for (std::list<ParseNode*>::const_iterator it = children.begin(), end = children.end(); it != end; ++it)
		    (*it)->print(depth+1);
	}

	ParseNode* deep_copy() {
		ParseNode *rtn = new ParseNode(type, token.lineNum, token.charNum);
		for (std::list<ParseNode*>::iterator it = children.begin(); it != children.end(); ++it)
			rtn->children.push_back((*it)->deep_copy());
		return rtn;
	}

	uint64_t to_hash() {
		uint64_t rtn = 0;
		rtn += 101 * int(type);
		for (std::list<ParseNode*>::const_iterator it = children.begin(), end = children.end(); it != end; ++it)
			rtn += (*it)->to_hash();
		return rtn;
	}
};

class ParseRule {
public:
	ParseRule(int p, std::set<TreeType> par, std::set<TreeType> notPar, std::vector<TreeType> f, TreeType t) {
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

class Parser {
private:
	ParseNode* mainTree;
	void doCurlyBracePass(ParseNode* tree);
	void doParenthesesPass(ParseNode* tree);
	void doBracketPass(ParseNode* tree);
	void doTemplatePass(ParseNode* tree);
	void parseLeftRight(ParseNode *tree, int from, int to);
	void parseRightLeft(ParseNode *tree, int from, int to);
	void parse_enum_block(ParseNode* tree);
	void error(std::string message, ParseNode* tree);
	void classify_parsed_block(ParseNode *tree);
	Token getPreviousToken(ParseNode *tree);
	std::vector<ParseRule> rules;
	std::vector<bool>leftRight;

	int debug_counter = 1000;

	int firstToken = raw_type;
	int lastToken = comma_value;
	static bool thomasParserPrecedenceSorter(ParseRule r1, ParseRule r2);
public:
	std::set<TreeType> shortcuts[250];
	Parser(std::vector<bool> lr, std::vector<ParseRule> r) {
		rules = r;
		std::sort(rules.begin(), rules.end(), thomasParserPrecedenceSorter);
		leftRight = lr;

		// shortcuts[value] = {P_IDENTIFIER};
		shortcuts[raw_type] = {P_KEYWORD_INT, P_KEYWORD_INT8, P_KEYWORD_INT16, P_KEYWORD_INT32, P_KEYWORD_UINT, P_KEYWORD_UINT8, P_KEYWORD_UINT16, P_KEYWORD_UINT32, P_KEYWORD_CHAR, P_KEYWORD_BOOL, P_KEYWORD_FLOAT, P_KEYWORD_DOUBLE, P_KEYWORD_VAR, P_CLASS_IDENTIFIER, list_literal, set_literal, ordered_map_literal, unordered_map_literal};
		shortcuts[raw_type_or_void] = {P_KEYWORD_VOID, P_KEYWORD_INT, P_KEYWORD_INT8, P_KEYWORD_INT16, P_KEYWORD_INT32, P_KEYWORD_UINT, P_KEYWORD_UINT8, P_KEYWORD_UINT16, P_KEYWORD_UINT32, P_KEYWORD_CHAR, P_KEYWORD_BOOL, P_KEYWORD_FLOAT, P_KEYWORD_DOUBLE, P_KEYWORD_VAR, P_CLASS_IDENTIFIER, list_literal, set_literal, ordered_map_literal, unordered_map_literal};
		// shortcuts[composite_type] = {};
		shortcuts[int_type] = {P_KEYWORD_INT, P_KEYWORD_INT8, P_KEYWORD_INT16, P_KEYWORD_INT32, P_KEYWORD_UINT, P_KEYWORD_UINT8, P_KEYWORD_UINT16, P_KEYWORD_UINT32};
		shortcuts[unary1_value] =      {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause};
		shortcuts[unary2_value] =      {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause};
		shortcuts[mult_value] =        {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause};
		shortcuts[plus_value] =        {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause};
		shortcuts[shift_value] =       {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause};
		shortcuts[inequality_value] =  {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause};
		shortcuts[equality_value] =    {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause};
		shortcuts[bitwise_and_value] = {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause};
		shortcuts[bitwise_xor_value] = {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause};
		shortcuts[bitwise_or_value] =  {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause, bitwise_or_clause};
		shortcuts[ternary_value] =     {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause, bitwise_or_clause, ternary_clause};
		shortcuts[setting_value] =     {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause, bitwise_or_clause, ternary_clause, setting_clause};
		shortcuts[comma_value] =       {P_IDENTIFIER, P_INTEGER_LITERAL, P_FLOAT_LITERAL, P_STRING_LITERAL, parenthesis, list_literal, set_literal, ordered_map_literal, unordered_map_literal, unary1_clause, unary2_clause, mult_clause, plus_clause, shift_clause, inequality_clause, equality_clause, bitwise_and_clause, bitwise_xor_clause, bitwise_or_clause, ternary_clause, setting_clause, comma_clause};
		shortcuts[any_integer_type] = {P_KEYWORD_INT, P_KEYWORD_INT8, P_KEYWORD_INT16, P_KEYWORD_INT32, P_KEYWORD_UINT, P_KEYWORD_UINT8, P_KEYWORD_UINT16, P_KEYWORD_UINT32, P_KEYWORD_CHAR, P_KEYWORD_BOOL};
		shortcuts[structure] =              {statement, for_loop, while_loop, do_while_loop, if_statement, if_else_statement, block_of_statements_or_class};
		shortcuts[stuff_in_classes] = {variable_dec, class_declaration, class_implementation, function_declaration, function_implementation};
		shortcuts[structure_or_statement] = {statement, for_loop, while_loop, do_while_loop, if_statement, if_else_statement, block_of_statements_or_class, statement, variable_dec, return_statement, case_statement, continue_statement, break_statement};
		shortcuts[op] = {P_LESS_THAN, P_LESS_THAN_EQUALS, P_GREATER_THAN, P_GREATER_THAN_EQUALS, P_EQUALS, P_EQUAL_EQUALS,
			P_POSITIVE, P_MINUS, P_SLASH, P_ASTERISK, P_PLUS_PLUS, P_MINUS_MINUS, P_SHIFT_RIGHT, P_SHIFT_LEFT, P_PERCENT,
			P_PLUS_EQUALS, P_MINUS_EQUALS, P_SLASH_EQUALS, P_ASTERISK_EQUALS, P_SHIFT_RIGHT_EQUALS, P_SHIFT_LEFT_EQUALS, P_PERCENT_EQUALS, P_LEFT_ARROW, P_LEFT_RIGHT_ARROW};
		shortcuts[function_name_candidate] = {P_IDENTIFIER, P_LESS_THAN, P_LESS_THAN_EQUALS, P_GREATER_THAN, P_GREATER_THAN_EQUALS, P_EQUALS, P_EQUAL_EQUALS,
			P_POSITIVE, P_MINUS, P_SLASH, P_ASTERISK, P_PLUS_PLUS, P_MINUS_MINUS, P_SHIFT_RIGHT, P_SHIFT_LEFT, P_PERCENT, P_BACK_SLASH, P_ASTERISK_ASTERISK,
			P_PLUS_EQUALS, P_MINUS_EQUALS, P_SLASH_EQUALS, P_ASTERISK_EQUALS, P_SHIFT_RIGHT_EQUALS, P_SHIFT_LEFT_EQUALS, P_PERCENT_EQUALS, P_BACK_SLASH_EQUALS, P_ASTERISK_ASTERISK_EQUALS};

	// listOfRules.push_back(ParseRule(10, general, {P_FLOAT_LITERAL}, value));
	// listOfRules.push_back(ParseRule(10, general, {P_IDENTIFIER}, value));
	// listOfRules.push_back(ParseRule(10, general, {P_STRING_LITERAL
	}
	ParseNode* getParseTree(std::list<Token> *tokens);
	void parse(ParseNode *tree);
};

#endif
