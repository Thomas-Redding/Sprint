#include "../include/Parser.hpp"

bool is_int(TreeType type) {
	return type == P_KEYWORD_INT || type == P_KEYWORD_INT8 || type == P_KEYWORD_INT16 || type == P_KEYWORD_INT32 || type == P_KEYWORD_UINT || type == P_KEYWORD_UINT8 || type == P_KEYWORD_UINT16 || type == P_KEYWORD_UINT32 || type == P_KEYWORD_UINT || type == P_KEYWORD_BOOL || type == P_KEYWORD_CHAR;
}

std::string treeTypeToString(TreeType t) {
	if (t == P_AMPERSAND_EQUALS) return "P_AMPERSAND_EQUALS";
	else if (t == P_AMPERSAND) return "P_AMPERSAND";
	else if (t == P_ARROW) return "P_ARROW";
	else if (t == P_ASTERISK_ASTERISK_EQUALS) return "P_ASTERISK_ASTERISK_EQUALS";
	else if (t == P_ASTERISK_ASTERISK) return "P_ASTERISK_ASTERISK";
	else if (t == P_ASTERISK_EQUALS) return "P_ASTERISK_EQUALS";
	else if (t == P_ASTERISK) return "P_ASTERISK";
	else if (t == P_AT) return "P_AT";
	else if (t == P_BACK_SLASH_EQUALS) return "P_BACK_SLASH_EQUALS";
	else if (t == P_BACK_SLASH) return "P_BACK_SLASH";
	else if (t == P_BRACKET) return "P_BRACKET";
	else if (t == P_CARROT_EQUALS) return "P_CARROT_EQUALS";
	else if (t == P_CARROT) return "P_CARROT";
	else if (t == P_CHARACTER_LITERAL) return "P_CHARACTER_LITERAL";
	else if (t == P_CLOSE_BRACKET) return "P_CLOSE_BRACKET";
	else if (t == P_CLOSE_CURLY_BRACE) return "P_CLOSE_CURLY_BRACE";
	else if (t == P_CLOSE_PARENTHESIS) return "P_CLOSE_PARENTHESIS";
	else if (t == P_CLOSE_TEMPLATE) return "P_CLOSE_TEMPLATE";
	else if (t == P_COLON_EQUALS) return "P_COLON_EQUALS";
	else if (t == P_COLON) return "P_COLON";
    else if (t == P_PLUS) return "P_PLUS";
    else if (t == P_MINUS) return "P_MINUS";
	else if (t == P_COMMA) return "P_COMMA";
    else if (t == P_VERTICAL_BAR) return "P_VERTICAL_BAR";
    else if (t == P_ASTERISK_ASTERISK) return "P_ASTERISK_ASTERISK";
    else if (t == P_MINUS_MINUS) return "P_MINUS_MINUS";
    else if (t == P_SEMI_COLON) return "P_SEMI_COLON";
	else if (t == P_EQUAL_EQUAL_EQUALS) return "P_EQUAL_EQUAL_EQUALS";
	else if (t == P_EQUAL_EQUALS) return "P_EQUAL_EQUALS";
	else if (t == P_EQUALS) return "P_EQUALS";
	else if (t == P_EXCLAMATION_POINT_EQUAL_EQUALS) return "P_EXCLAMATION_POINT_EQUAL_EQUALS";
	else if (t == P_EXCLAMATION_POINT_EQUALS) return "P_EXCLAMATION_POINT_EQUALS";
	else if (t == P_EXCLAMATION_POINT) return "P_EXCLAMATION_POINT";
	else if (t == P_FLOAT_LITERAL) return "P_FLOAT_LITERAL";
	else if (t == P_FLOAT_LITERAL2) return "P_FLOAT_LITERAL2";
	else if (t == P_GREATER_THAN_EQUALS) return "P_GREATER_THAN_EQUALS";
	else if (t == P_GREATER_THAN) return "P_GREATER_THAN";
	else if (t == P_IDENTIFIER) return "P_IDENTIFIER";
	else if (t == P_INTEGER_LITERAL) return "P_INTEGER_LITERAL";
	else if (t == P_KEYWORD_AND) return "P_KEYWORD_AND";
	else if (t == P_KEYWORD_BOOL) return "P_KEYWORD_BOOL";
	else if (t == P_KEYWORD_BREAK) return "P_KEYWORD_BREAK";
	else if (t == P_KEYWORD_CASE) return "P_KEYWORD_CASE";
	else if (t == P_KEYWORD_CATCH) return "P_KEYWORD_CATCH";
	else if (t == P_KEYWORD_CHAR) return "P_KEYWORD_CHAR";
	else if (t == P_KEYWORD_CLASS) return "P_KEYWORD_CLASS";
	else if (t == P_KEYWORD_COMMUTE) return "P_KEYWORD_COMMUTE";
	else if (t == P_KEYWORD_CONST) return "P_KEYWORD_CONST";
	else if (t == P_KEYWORD_CONTINUE) return "P_KEYWORD_CONTINUE";
	else if (t == P_KEYWORD_DEFAULT) return "P_KEYWORD_DEFAULT";
	else if (t == P_KEYWORD_DO) return "P_KEYWORD_DO";
	else if (t == P_KEYWORD_DOUBLE) return "P_KEYWORD_DOUBLE";
	else if (t == P_KEYWORD_ELSE) return "P_KEYWORD_ELSE";
	else if (t == P_KEYWORD_ENUM) return "P_KEYWORD_ENUM";
	else if (t == P_KEYWORD_FALSE) return "P_KEYWORD_FALSE";
	else if (t == P_KEYWORD_FLOAT) return "P_KEYWORD_FLOAT";
	else if (t == P_KEYWORD_FOR) return "P_KEYWORD_FOR";
	else if (t == P_KEYWORD_IF) return "P_KEYWORD_IF";
	else if (t == P_KEYWORD_IMPORT) return "P_KEYWORD_IMPORT";
	else if (t == P_KEYWORD_IN) return "P_KEYWORD_IN";
	else if (t == P_KEYWORD_INFINITY) return "P_KEYWORD_INFINITY";
	else if (t == P_KEYWORD_INT) return "P_KEYWORD_INT";
	else if (t == P_KEYWORD_INT16) return "P_KEYWORD_INT16";
	else if (t == P_KEYWORD_INT32) return "P_KEYWORD_INT32";
	else if (t == P_KEYWORD_INT8) return "P_KEYWORD_INT8";
	else if (t == P_KEYWORD_IS) return "P_KEYWORD_IS";
	else if (t == P_KEYWORD_MUT) return "P_KEYWORD_MUT";
	else if (t == P_KEYWORD_NAN) return "P_KEYWORD_NAN";
	else if (t == P_KEYWORD_NOT) return "P_KEYWORD_NOT";
	else if (t == P_KEYWORD_NULL) return "P_KEYWORD_NULL";
	else if (t == P_KEYWORD_OR) return "P_KEYWORD_OR";
	else if (t == P_KEYWORD_PRIVATE) return "P_KEYWORD_PRIVATE";
	else if (t == P_KEYWORD_PUBLIC) return "P_KEYWORD_PUBLIC";
    else if (t == P_KEYWORD_PROTECTED) return "P_KEYWORD_PROTECTED";
	else if (t == P_KEYWORD_REPEAT) return "P_KEYWORD_REPEAT";
	else if (t == P_KEYWORD_RETURN) return "P_KEYWORD_RETURN";
	else if (t == P_KEYWORD_SIZEOF) return "P_KEYWORD_SIZEOF";
	else if (t == P_KEYWORD_STATIC) return "P_KEYWORD_STATIC";
	else if (t == P_KEYWORD_SWITCH) return "P_KEYWORD_SWITCH";
    else if (t == P_KEYWORD_VOID) return "P_KEYWORD_VOID";
    else if (t == P_KEYWORD_UNION) return "P_KEYWORD_UNION";
	else if (t == curly_brace_block) return "curly_brace_block";
	else if (t == parenthesis_block) return "parenthesis_block";
	else if (t == bracket_block) return "bracket_block";
	else if (t == template_block) return "template_block";
	else if (t == mult_clause) return "mult_clause";
	else if (t == plus_clause) return "plus_clause";
	else if (t == general) return "general";
	else if (t == unary1_value) return "unary1_value";
	else if (t == unary1_clause) return "unary1_clause";
	else if (t == unary2_value) return "unary2_value";
	else if (t == unary2_clause) return "unary2_clause";
	else if (t == mult_value) return "mult_value";
	else if (t == mult_clause) return "mult_clause";
	else if (t == plus_value) return "plus_value";
	else if (t == plus_clause) return "plus_clause";
	else if (t == shift_value) return "shift_value";
	else if (t == shift_clause) return "shift_clause";
	else if (t == inequality_value) return "inequality_value";
	else if (t == inequality_clause) return "inequality_clause";
	else if (t == equality_value) return "equality_value";
	else if (t == equality_clause) return "equality_clause";
	else if (t == bitwise_and_value) return "bitwise_and_value";
	else if (t == bitwise_and_clause) return "bitwise_and_clause";
	else if (t == bitwise_xor_value) return "bitwise_xor_value";
	else if (t == bitwise_xor_clause) return "bitwise_xor_clause";
	else if (t == bitwise_or_value) return "bitwise_or_value";
	else if (t == bitwise_or_clause) return "bitwise_or_clause";
	else if (t == logical_and_value) return "logical_and_value";
	else if (t == logical_xor_value) return "logical_xor_value";
	else if (t == logical_or_value) return "logical_or_value";
	else if (t == logical_and_clause) return "logical_and_clause";
	else if (t == logical_xor_clause) return "logical_xor_clause";
	else if (t == logical_or_clause) return "logical_or_clause";
	else if (t == setting_value) return "setting_value";
	else if (t == setting_clause) return "setting_clause";
	else if (t == ternary_clause) return "ternary_clause";
	else if (t == comma_value) return "comma_value";
	else if (t == comma_clause) return "comma_clause";
	else if (t == enum_implementation) return "enum_implementation";
	else if (t == class_implementation) return "class_implementation";
	else if (t == function_implementation) return "function_implementation";
	else if (t == function_declaration) return "function_declaration";
	else if (t == function_head) return "function_head";
	else if (t == statement) return "statement";
	else if (t == if_statement) return "if_statement";
	else if (t == if_else_statement) return "if_else_statement";
	else if (t == for_loop) return "for_loop";
	else if (t == while_loop) return "while_loop";
	else if (t == do_while_loop) return "do_while_loop";
	else if (t == switch_statement) return "switch_statement";
	else if (t == return_statement) return "return_statement";
	else if (t == break_statement) return "break_statement";
	else if (t == continue_statement) return "continue_statement";
	else if (t == case_statement) return "case_statement";
	else if (t == variable_dec) return "variable_dec";
	else if (t == colon_clause) return "colon_clause";
	else if (t == list_literal) return "list_literal";
	else if (t == list_type) return "list_type";
	else if (t == set_literal) return "set_literal";
	else if (t == set_type) return "set_type";
	else if (t == ordered_map_literal) return "ordered_map_literal";
	else if (t == unordered_map_literal) return "unordered_map_literal";
	else if (t == unordered_map_type) return "unordered_map_type";
	else if (t == colon_list) return "colon_list";
	else if (t == colon_type_clause) return "colon_type_clause";
	else if (t == parenthesis) return "parenthesis";
	else if (t == templates) return "templates";
	else if (t == block_of_statements_or_class) return "block_of_statements_or_class";
	else if (t == enum_block) return "enum_block";
	else if (t == function_pointer_declaration) return "function_pointer_declaration";
	else if (t == static_member) return "static_member";
    else if (t == bracket_access) return "bracket_access";
    else if (t == function_params_block) return "function_params_block";
    else if (t == function_param) return "function_param";
    else if (t == P_POSITIVE) return "P_POSITIVE";
    else if (t == empty_curly_brace_block) return "empty_curly_brace_block";
    else if (t == function_pointer_params_block) return "function_pointer_params_block";
    else if (t == function_pointer_type) return "function_pointer_type";
    else if (t == anonymous_function) return "anonymous_function";
    else if (t == permissions_line) return "permissions_line";
    else if (t == P_KEYWORD_UNION) return "P_KEYWORD_UNION";
    else if (t == union_block) return "union_block";
    else if (t == union_implementation) return "union_implementation";
	else return std::to_string(static_cast<TreeType>(t));
}

ParseNode::ParseNode(std::list<Token>::iterator it, std::list<Token>::iterator last) {
	token = *it;
	type = translateType(token.type);
	is_leaf = true;
}

TreeType translateType(TokenType t) {
	if (t == AMPERSAND_EQUALS) return P_AMPERSAND_EQUALS;
	else if (t == AMPERSAND) return P_AMPERSAND;
	else if (t == ARROW) return P_ARROW;
	else if (t == ASTERISK_ASTERISK_EQUALS) return P_ASTERISK_ASTERISK_EQUALS;
	else if (t == ASTERISK_ASTERISK) return P_ASTERISK_ASTERISK;
	else if (t == ASTERISK_EQUALS) return P_ASTERISK_EQUALS;
	else if (t == ASTERISK) return P_ASTERISK;
	else if (t == AT) return P_AT;
	else if (t == BACK_SLASH_EQUALS) return P_BACK_SLASH_EQUALS;
	else if (t == BACK_SLASH) return P_BACK_SLASH;
	else if (t == BRACKET) return P_BRACKET;
	else if (t == CARROT_EQUALS) return P_CARROT_EQUALS;
	else if (t == CARROT) return P_CARROT;
	else if (t == CHARACTER_LITERAL) return P_CHARACTER_LITERAL;
	else if (t == CLOSE_BRACKET) return P_CLOSE_BRACKET;
	else if (t == CLOSE_CURLY_BRACE) return P_CLOSE_CURLY_BRACE;
	else if (t == CLOSE_PARENTHESIS) return P_CLOSE_PARENTHESIS;
	else if (t == CLOSE_TEMPLATE) return P_CLOSE_TEMPLATE;
	else if (t == COLON_EQUALS) return P_COLON_EQUALS;
	else if (t == COLON) return P_COLON;
	else if (t == COMMA) return P_COMMA;
	else if (t == EQUAL_EQUAL_EQUALS) return P_EQUAL_EQUAL_EQUALS;
	else if (t == EQUAL_EQUALS) return P_EQUAL_EQUALS;
	else if (t == EQUALS) return P_EQUALS;
	else if (t == EXCLAMATION_POINT_EQUAL_EQUALS) return P_EXCLAMATION_POINT_EQUAL_EQUALS;
	else if (t == EXCLAMATION_POINT_EQUALS) return P_EXCLAMATION_POINT_EQUALS;
	else if (t == EXCLAMATION_POINT) return P_EXCLAMATION_POINT;
	else if (t == FLOAT_LITERAL) return P_FLOAT_LITERAL;
	else if (t == FLOAT_LITERAL2) return P_FLOAT_LITERAL2;
	else if (t == GREATER_THAN_EQUALS) return P_GREATER_THAN_EQUALS;
	else if (t == GREATER_THAN) return P_GREATER_THAN;
	else if (t == IDENTIFIER) return P_IDENTIFIER;
	else if (t == INTEGER_LITERAL) return P_INTEGER_LITERAL;
	else if (t == KEYWORD_AND) return P_KEYWORD_AND;
	else if (t == KEYWORD_BOOL) return P_KEYWORD_BOOL;
	else if (t == KEYWORD_BREAK) return P_KEYWORD_BREAK;
	else if (t == KEYWORD_CASE) return P_KEYWORD_CASE;
	else if (t == KEYWORD_CATCH) return P_KEYWORD_CATCH;
	else if (t == KEYWORD_CHAR) return P_KEYWORD_CHAR;
	else if (t == KEYWORD_CLASS) return P_KEYWORD_CLASS;
	else if (t == KEYWORD_COMMUTE) return P_KEYWORD_COMMUTE;
	else if (t == KEYWORD_CONST) return P_KEYWORD_CONST;
	else if (t == KEYWORD_CONTINUE) return P_KEYWORD_CONTINUE;
	else if (t == KEYWORD_DEFAULT) return P_KEYWORD_DEFAULT;
	else if (t == KEYWORD_DO) return P_KEYWORD_DO;
	else if (t == KEYWORD_DOUBLE) return P_KEYWORD_DOUBLE;
	else if (t == KEYWORD_ELSE) return P_KEYWORD_ELSE;
	else if (t == KEYWORD_ENUM) return P_KEYWORD_ENUM;
	else if (t == KEYWORD_FALSE) return P_KEYWORD_FALSE;
	else if (t == KEYWORD_FLOAT) return P_KEYWORD_FLOAT;
	else if (t == KEYWORD_FOR) return P_KEYWORD_FOR;
	else if (t == KEYWORD_IF) return P_KEYWORD_IF;
	else if (t == KEYWORD_IMPORT) return P_KEYWORD_IMPORT;
	else if (t == KEYWORD_IN) return P_KEYWORD_IN;
	else if (t == KEYWORD_INFINITY) return P_KEYWORD_INFINITY;
	else if (t == KEYWORD_INT) return P_KEYWORD_INT;
	else if (t == KEYWORD_INT16) return P_KEYWORD_INT16;
	else if (t == KEYWORD_INT32) return P_KEYWORD_INT32;
	else if (t == KEYWORD_INT8) return P_KEYWORD_INT8;
	else if (t == KEYWORD_IS) return P_KEYWORD_IS;
	else if (t == KEYWORD_MUT) return P_KEYWORD_MUT;
	else if (t == KEYWORD_NAN) return P_KEYWORD_NAN;
	else if (t == KEYWORD_NOT) return P_KEYWORD_NOT;
	else if (t == KEYWORD_NULL) return P_KEYWORD_NULL;
	else if (t == KEYWORD_OR) return P_KEYWORD_OR;
	else if (t == KEYWORD_PRIVATE) return P_KEYWORD_PRIVATE;
	else if (t == KEYWORD_PUBLIC) return P_KEYWORD_PUBLIC;
    else if (t == KEYWORD_PROTECTED) return P_KEYWORD_PROTECTED;
	else if (t == KEYWORD_REPEAT) return P_KEYWORD_REPEAT;
	else if (t == KEYWORD_RETURN) return P_KEYWORD_RETURN;
	else if (t == KEYWORD_SIZEOF) return P_KEYWORD_SIZEOF;
	else if (t == KEYWORD_STATIC) return P_KEYWORD_STATIC;
	else if (t == KEYWORD_SWITCH ) return P_KEYWORD_SWITCH ;
	else if (t == KEYWORD_THIS) return P_KEYWORD_THIS;
	else if (t == KEYWORD_THROW) return P_KEYWORD_THROW;
	else if (t == KEYWORD_TRUE) return P_KEYWORD_TRUE;
	else if (t == KEYWORD_TRY) return P_KEYWORD_TRY;
	else if (t == KEYWORD_UINT) return P_KEYWORD_UINT;
	else if (t == KEYWORD_UINT16) return P_KEYWORD_UINT16;
	else if (t == KEYWORD_UINT32) return P_KEYWORD_UINT32;
	else if (t == KEYWORD_UINT8) return P_KEYWORD_UINT8;
	else if (t == KEYWORD_VOID) return P_KEYWORD_VOID;
    else if (t == KEYWORD_UNION) return P_KEYWORD_UNION;
	else if (t == KEYWORD_WHILE) return P_KEYWORD_WHILE;
	else if (t == KEYWORD_XOR) return P_KEYWORD_XOR;
	else if (t == KEYWORD) return P_KEYWORD;
	else if (t == LEFT_ARROW) return P_LEFT_ARROW;
	else if (t == LEFT_RIGHT_ARROW) return P_LEFT_RIGHT_ARROW;
	else if (t == LESS_THAN_EQUALS) return P_LESS_THAN_EQUALS;
	else if (t == LESS_THAN) return P_LESS_THAN;
	else if (t == MINUS_EQUALS) return P_MINUS_EQUALS;
	else if (t == MINUS_MINUS) return P_MINUS_MINUS;
	else if (t == MINUS) return P_MINUS;
	else if (t == NEGATIVE) return P_NEGATIVE;
	else if (t == NULL_TOKEN_TYPE) return P_NULL_TOKEN_TYPE;
	else if (t == OPEN_BRACKET) return P_OPEN_BRACKET;
	else if (t == OPEN_CURLY_BRACE) return P_OPEN_CURLY_BRACE;
	else if (t == OPEN_PARENTHESIS) return P_OPEN_PARENTHESIS;
	else if (t == OPEN_TEMPLATE) return P_OPEN_TEMPLATE;
	else if (t == PERCENT_EQUALS) return P_PERCENT_EQUALS;
	else if (t == PERCENT) return P_PERCENT;
	else if (t == PERIOD) return P_PERIOD;
	else if (t == PLUS_EQUALS) return P_PLUS_EQUALS;
	else if (t == PLUS_PLUS) return P_PLUS_PLUS;
	else if (t == PLUS) return P_PLUS;
	else if (t == POSITIVE) return P_POSITIVE;
	else if (t == POUND_SIGN) return P_POUND_SIGN;
	else if (t == PUNCTUATION) return P_PUNCTUATION;
	else if (t == QUESTION_MARK) return P_QUESTION_MARK;
	else if (t == SEMI_COLON) return P_SEMI_COLON;
	else if (t == SHIFT_LEFT_EQUALS) return P_SHIFT_LEFT_EQUALS;
	else if (t == SHIFT_LEFT) return P_SHIFT_LEFT;
	else if (t == SHIFT_RIGHT_EQUALS) return P_SHIFT_RIGHT_EQUALS;
	else if (t == SHIFT_RIGHT) return P_SHIFT_RIGHT;
	else if (t == SLASH_EQUALS) return P_SLASH_EQUALS;
	else if (t == SLASH) return P_SLASH;
	else if (t == STRING_LITERAL) return P_STRING_LITERAL;
	else if (t == TILDE) return P_TILDE;
	else if (t == UNKNOWN) return P_UNKNOWN;
	else if (t == VERTICAL_BAR_EQUALS) return P_VERTICAL_BAR_EQUALS;
	else if (t == VERTICAL_BAR) return P_VERTICAL_BAR;
	else return general;
}

ParseNode* Parser::getParseTree(std::list<Token> *tokens) {
	mainTree = new ParseNode(general, 0, 0);
	bool is_first = true;
	std::list<Token>::iterator behind_it = tokens->begin();
	for (std::list<Token>::iterator it = tokens->begin(); it != tokens->end(); ++it) {
		if (is_first) {
			is_first = true;
		}
		--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); exit(0); }
		mainTree->children.push_back(new ParseNode(it, behind_it));
		++behind_it;
	}

	doCurlyBracePass(mainTree);
	doParenthesesPass(mainTree);
	doBracketPass(mainTree);
	doTemplatePass(mainTree);
	parse(mainTree, nullptr);
	return mainTree;
}

bool Parser::thomasParserPrecedenceSorter(ParseRule r1, ParseRule r2) {
	return r1.precedence < r2.precedence;
}

void Parser::parse_enum_block(ParseNode* tree) {
	tree->type = enum_block;
}

void Parser::parse_union_block(ParseNode* tree) {
    tree->type = union_block;
}

void Parser::parse_function_pointer_args(ParseNode** tree) {
    ParseNode* par = *tree;
    ParseNode* new_tree = new ParseNode(function_pointer_params_block, par->token.lineNum, par->token.charNum);
    if (par->children.size() == 0) {
        delete par;
        *tree = new_tree;
        return;
    }

    new_tree->children.push_back(new ParseNode(function_pointer_param, par->token.lineNum, par->token.charNum));
    for (std::list<ParseNode*>::iterator it = par->children.begin(); it != par->children.end(); ++it) {
        if ((*it)->type == P_COMMA)
            new_tree->children.push_back(new ParseNode(function_pointer_param, (*it)->token.lineNum, (*it)->token.charNum));
        else
            new_tree->children.back()->children.push_back(*it);
    }

    for (std::list<ParseNode*>::iterator it = new_tree->children.begin(); it != new_tree->children.end(); ++it) {
        int from = 0;
        int to = 0;
        for (int i=0; i<leftRight.size(); ++i) {
            from = to;
            int j;
            for (j=from; j<rules.size(); ++j) {
                if (rules[j].precedence != rules[from].precedence)
                    break;
            }
            to = j;
            if (leftRight[i])
                parseLeftRight(*it, from, to);
            else
                parseRightLeft(*it, from, to);
        }
    }

    delete par;
    *tree = new_tree;
}

void Parser::parse_function_params(ParseNode** tree) {
    ParseNode* par = *tree;
    ParseNode* new_tree = new ParseNode(function_params_block, par->token.lineNum, par->token.charNum);
    if (par->children.size() == 0) {
        delete par;
        *tree = new_tree;
        return;
    }

    new_tree->children.push_back(new ParseNode(function_param, par->token.lineNum, par->token.charNum));
    for (std::list<ParseNode*>::iterator it = par->children.begin(); it != par->children.end(); ++it) {
        if ((*it)->type == P_COMMA)
            new_tree->children.push_back(new ParseNode(function_param, (*it)->token.lineNum, (*it)->token.charNum));
        else
            new_tree->children.back()->children.push_back(*it);
    }

    for (std::list<ParseNode*>::iterator it = new_tree->children.begin(); it != new_tree->children.end(); ++it) {
        int from = 0;
        int to = 0;
        for (int i=0; i<leftRight.size(); ++i) {
            from = to;
            int j;
            for (j=from; j<rules.size(); ++j) {
                if (rules[j].precedence != rules[from].precedence)
                    break;
            }
            to = j;
            if (leftRight[i])
                parseLeftRight(*it, from, to);
            else
                parseRightLeft(*it, from, to);
        }
    }

    delete par;
    *tree = new_tree;
}

void Parser::parse(ParseNode* tree, ParseNode* parent) {
	int from = 0;
	int to = 0;
	if (tree->children.size() == 0) {
        if (tree->type != general)
            classify_parsed_block(tree, parent);
        return;
    }

	for (std::list<ParseNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
		if ((*it)->type == curly_brace_block || (*it)->type == bracket_block || (*it)->type == parenthesis_block || (*it)->type == template_block) {
			if ((*it)->type == curly_brace_block && it != tree->children.begin()) {
				// check if this is an enum declaration
				std::list<ParseNode*>::iterator it2 = it;
				--it2;
                if ((*it2)->type == P_IDENTIFIER) {
                    --it2;
                    if ((*it2)->type == P_KEYWORD_ENUM) {
                        parse_enum_block(*it);
                    }
                    else if ((*it2)->type == P_KEYWORD_UNION) {
                        parse_union_block(*it);
                    }
                    else {
                        parse(*it, tree);
                    }
                }
				else {
					parse(*it, tree);
				}
			}
            else if ((*it)->type == parenthesis_block) {
                if (it == tree->children.end())
                    error("Function signature missing arrow", *it);
                auto it2 = it;
                ++it2;
                if (it == tree->children.begin()) {
                    if ((*it2)->type == P_ARROW) {
                        parse_function_pointer_args(&(*it));
                    }
                    else {
                        parse(*it, tree);
                    }
                }
                else {
                    auto it3 = it;
                    --it3;
                    if ((*it2)->type == P_ARROW) {
                        ++it2;
                        ++it2;
                        if ((*it2)->type == templates) ++it2;
                        if ((*it2)->type == empty_curly_brace_block || block_of_statements_or_class)
                            parse_function_params(&(*it));
                        else
                            parse_function_pointer_args(&(*it));
                    }
                    else {
                        parse(*it, tree);
                    }
                }
            }
			else {
				parse(*it, tree);
			}
		}
	}
	for (int i=0; i<leftRight.size(); ++i) {
		from = to;
		int j;
		for (j=from; j<rules.size(); ++j) {
			if (rules[j].precedence != rules[from].precedence)
				break;
		}
		to = j;
		if (leftRight[i])
			parseLeftRight(tree, from, to);
		else
			parseRightLeft(tree, from, to);
	}
	if (tree->type != general)
		classify_parsed_block(tree, parent);
}

void Parser::classify_parsed_block(ParseNode *tree, ParseNode *parent) {
	if (tree->type == curly_brace_block) {
		if (tree->children.size() == 0) {
			// {}
			tree->type = empty_curly_brace_block;
		}
		else if (tree->children.size() == 1) {
			// either (1) a block with one statement, a set literal, or a 
			ParseNode *child = *(tree->children.begin());
			if (shortcuts[structure_or_statement].find(child->type) != shortcuts[structure_or_statement].end()) {
				tree->type = block_of_statements_or_class;
			}
			else if (shortcuts[stuff_in_classes].find(child->type) != shortcuts[stuff_in_classes].end()) {
				tree->type = block_of_statements_or_class;
			}
			else if(child->type == colon_list) {
				// {1 : 2, 3 : 4}
				tree->type = unordered_map_literal;
			}
			else if (child->type == colon_clause) {
				// {1: 2}
				tree->type = unordered_map_literal;
			}
			else if (child->type == colon_type_clause) {
				// {1: 2}
				tree->type = unordered_map_literal;
			}
			else if (child->type == comma_clause) {
				// {1, 2}
				tree->type = set_literal;
			}
			else if (shortcuts[comma_value].find(child->type) != shortcuts[comma_value].end() || shortcuts[raw_type].find(child->type) != shortcuts[raw_type].end() || child->type == set_literal || child->type == unordered_map_literal || child->type == list_literal || child->type == ordered_map_literal) {
				// {1}
				tree->type = set_literal;
			}
			else if (child->type == P_COLON) {
				tree->type = unordered_map_literal;
			}
			else {
				error("Poorly formatted curly-brace block", tree);
			}
		}
		else {
			for (std::list<ParseNode*>::iterator it = tree->children.begin(); it != tree->children.end(); ++it) {
				if (shortcuts[structure_or_statement].find((*it)->type) == shortcuts[structure_or_statement].end()) {
					std::list<ParseNode*>::iterator it2;
					for (it2 = tree->children.begin(); it2 != tree->children.end(); ++it2) {
						if (shortcuts[stuff_in_classes].find((*it2)->type) == shortcuts[stuff_in_classes].end())
							error("Poorly formatted code block.", *it2);
					}
					if (it2 == tree->children.end())
						tree->type = block_of_statements_or_class;
					else
						error("Poorly formatted block of statements.", *it);
				}
			}
			tree->type = block_of_statements_or_class;
		}
	}
	else if (tree->type == bracket_block) {
		if (tree->children.size() == 0) {
			// []
			tree->type = list_literal;
		}
		else if (tree->children.size() == 1) {
			// either a list literal or a ordered map literal
			ParseNode *child = *(tree->children.begin());
			if(child->type == colon_list) {
				// [1 : 2, 3 : 4]
				tree->type = ordered_map_literal;
			}
			else if (child->type == colon_clause) {
				// [1: 2]
				tree->type = ordered_map_literal;
			}
			else if (child->type == colon_type_clause) {
				// [1: 2]
				tree->type = ordered_map_literal;
			}
			else if (child->type == comma_clause) {
				// [1, 2]
				tree->type = list_literal;
			}
			else if (shortcuts[comma_value].find(child->type) != shortcuts[comma_value].end() || shortcuts[raw_type].find(child->type) != shortcuts[raw_type].end() || child->type == set_literal || child->type == unordered_map_literal || child->type == list_literal || child->type == ordered_map_literal) {
				// [1] - list literal or bracket-accessor
                if (parent->children.front() == tree) {
                    tree->type = list_literal;
                    return;
                }

                for (std::list<ParseNode*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it) {
                    if (*it == tree) {
                        --it;
                        if ((*it)->type == P_IDENTIFIER) {
                            tree->type = bracket_access;
                            return;
                        }
                        break;
                    }
                }
                
				tree->type = list_literal;
			}
			else if (child->type == P_COLON) {
				tree->type = ordered_map_literal;
			}
			else {
				error("Poorly formatted bracket block", tree);
			}
		}
		else {
			error("Poorly formatted bracket block", tree);
		}
	}
	else if (tree->type == parenthesis_block) {
		// do nothing for now
		tree->type = parenthesis;
	}
	else if (tree->type == template_block) {
		// do nothing for now
		tree->type = templates;
	}
	else {
		error("Error 413. Please contact tfredding@gmail.com", tree);
	}
}

void Parser::parseLeftRight(ParseNode *tree, int from, int to) {
	bool skipRecursion = false;
	for (std::list<ParseNode*>::iterator it = tree->children.begin(); it != tree->children.end();) {
		if ((*it)->children.size() > 0 && !skipRecursion) {
			if ((*it)->type == bracket_block || (*it)->type == curly_brace_block || (*it)->type == parenthesis_block || (*it)->type == template_block) {
				// do nothing
			}
			else if ((*it)->type == block_of_statements_or_class || (*it)->type == list_literal || (*it)->type == set_literal || (*it)->type == ordered_map_literal || (*it)->type == unordered_map_literal || (*it)->type == bracket_access || (*it)->type == parenthesis || (*it)->type == templates) {
				// do nothing
			}
			else {
				parseLeftRight(*it, from, to);
			}
		}

		int ruleToApply = -1;
		int ruleSize = 0;
		for (int i = from; i < to; ++i) {
			if (rules[i].notParents.find(tree->type) == rules[i].notParents.end() && (rules[i].parents.size() == 0 || rules[i].parents.find(tree->type) != rules[i].notParents.end())) {
				int j;
				std::list<ParseNode*>::iterator it2 = it;
				for (j = 0; j < rules[i].from.size(); ++j) {
					if (rules[i].from[j] < firstToken) {
						if ((*it2)->type != rules[i].from[j])
							break;
					}
					else {
						// alias
						if(shortcuts[rules[i].from[j]].find((*it2)->type) == shortcuts[rules[i].from[j]].end())
							break;
					}
					++it2;
					if (it2 == tree->children.end()) {
						++j;
						break;
					}
				}
				if (j == rules[i].from.size()) {
					ruleToApply = i;
					break;
				}
			}
		}

		if (-1 < ruleToApply) {
			int ruleSize = rules[ruleToApply].from.size();
			std::list<ParseNode*>::iterator it2 = it;
			ParseNode *newTree = new ParseNode(rules[ruleToApply].to, (*it2)->token.lineNum, (*it2)->token.charNum);
			for (int i= 0; i < ruleSize; ++i) {
				--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
				newTree->children.push_back(*it2);
				++it2;
			}
			std::list<ParseNode*>::iterator before = it;
			--before;
			tree->children.erase(it, it2);
			--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
			tree->children.insert(++before, newTree);
			it = before;
			--it;
			skipRecursion = true;
		}
		else {
			++it;
			skipRecursion = false;
		}

		if (it == tree->children.end())
			return;
	}
	return;
}

std::string nodeToStr(ParseNode node) {
	std::string rtn = "(";
	rtn += treeTypeToString(node.type);
	rtn += ", ";
	rtn += node.token.str;
	rtn += ")";
	return rtn;
}

void Parser::parseRightLeft(ParseNode *tree, int from, int to) {
	for (std::list<ParseNode*>::iterator it = --tree->children.end(); true; --it) {
		if ((*it)->children.size() > 0) {
			if ((*it)->type == bracket_block || (*it)->type == curly_brace_block || (*it)->type == parenthesis_block || (*it)->type == template_block) {
				// do nothing
			}
			else if ((*it)->type == block_of_statements_or_class || (*it)->type == list_literal || (*it)->type == set_literal || (*it)->type == ordered_map_literal || (*it)->type == unordered_map_literal || (*it)->type == bracket_access || (*it)->type == parenthesis || (*it)->type == templates) {
				// do nothing
			}
			else {
				parseRightLeft(*it, from, to);
			}
		}

		int ruleToApply = -1;
		int ruleSize = 0;
		for (int i = from; i < to; ++i) {
			if (rules[i].notParents.find(tree->type) == rules[i].notParents.end() && (rules[i].parents.size() == 0 || rules[i].parents.find(tree->type) != rules[i].notParents.end())) {
				std::list<ParseNode*>::iterator it2 = it;
				int j;
				for (j = 0; j < rules[i].from.size(); ++j) {
					if (rules[i].from[j] < firstToken) {
						if ((*it2)->type != rules[i].from[j])
							break;
					}
					else {
						// alias
						if(shortcuts[rules[i].from[j]].find((*it2)->type) == shortcuts[rules[i].from[j]].end())
							break;
					}
					++it2;
					if (it2 == tree->children.end()) {
						++j;
						break;
					}
				}
				if (j == rules[i].from.size()) {
					ruleToApply = i;
					break;
				}
			}
		}

		// int a = b * c * d;
		if (-1 < ruleToApply) {
			int ruleSize = rules[ruleToApply].from.size();
			std::list<ParseNode*>::iterator it2 = it;
			ParseNode *newTree = new ParseNode(rules[ruleToApply].to, (*it)->token.lineNum, (*it)->token.charNum);
			for (int i= 0; i < ruleSize; ++i) {
				--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
				newTree->children.push_back(*it2);
				++it2;
			}
			std::list<ParseNode*>::iterator before = it;
			--before;
			tree->children.erase(it, it2);
			--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
			tree->children.insert(++before, newTree);
			it = before;
			--it;
		}

		if (it == tree->children.begin())
			return;
	}
	return;
}

/*
int precedence;
TreeType parent;
std::vector<TreeType> from;
TreeType to;
*/

void Parser::doCurlyBracePass(ParseNode* tree) {
	std::stack<std::list<ParseNode*>::iterator> st;
	for (std::list<ParseNode*>::iterator it=tree->children.begin(); it != tree->children.end(); ++it) {
		if ((*it)->children.size() > 0) {
			doCurlyBracePass(*it);
		}
		else {
			if ((*it)->type == P_OPEN_CURLY_BRACE) {
				st.push(it);
			}
			else if ((*it)->type == P_CLOSE_CURLY_BRACE) {
				// create new parenthesis_block
				if (st.size() == 0) {
					error("Closed curly brace has no open counterpart.", *it);
				}
				std::list<ParseNode*>::iterator leftPar = st.top();
				std::list<ParseNode*>::iterator leftNonPar = ++st.top();
				std::list<ParseNode*>::iterator rightPar = it;
				std::list<ParseNode*>::iterator end = ++it;
				--it;
				st.pop();
				ParseNode* newTree = new ParseNode(curly_brace_block, (*leftPar)->token.lineNum, (*leftPar)->token.charNum);
				for (std::list<ParseNode*>::iterator it2=leftNonPar; it2 != rightPar; ++it2) {
					--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
					newTree->children.push_back(*it2);
				}
				--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
				tree->children.insert(leftPar, newTree);
				tree->children.erase(leftPar, end);
			}
		}
	}
}

void Parser::doTemplatePass(ParseNode* tree) {
	std::stack<std::list<ParseNode*>::iterator> st;
	for (std::list<ParseNode*>::iterator it=tree->children.begin(); it != tree->children.end(); ++it) {
		if ((*it)->children.size() > 0) {
			doTemplatePass(*it);
		}
		else {
			if ((*it)->type == P_OPEN_TEMPLATE) {
				st.push(it);
			}
			else if ((*it)->type == P_CLOSE_TEMPLATE) {
				// create new parenthesis_block
				if (st.size() == 0) {
					error("Closed template has no open counterpart.", *it);
				}
				std::list<ParseNode*>::iterator leftPar = st.top();
				std::list<ParseNode*>::iterator leftNonPar = ++st.top();
				std::list<ParseNode*>::iterator rightPar = it;
				std::list<ParseNode*>::iterator end = ++it;
				--it;
				st.pop();
				ParseNode* newTree = new ParseNode(template_block, (*leftPar)->token.lineNum, (*leftPar)->token.charNum);
				for (std::list<ParseNode*>::iterator it2=leftNonPar; it2 != rightPar; ++it2) {
					--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
					newTree->children.push_back(*it2);
				}
				--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
				tree->children.insert(leftPar, newTree);
				tree->children.erase(leftPar, end);
			}
		}
	}
}


void Parser::doParenthesesPass(ParseNode* tree) {
	std::stack<std::list<ParseNode*>::iterator> st;
	for (std::list<ParseNode*>::iterator it=tree->children.begin(); it != tree->children.end(); ++it) {
		if ((*it)->children.size() > 0) {
			doParenthesesPass(*it);
		}
		else {
			if ((*it)->type == P_OPEN_PARENTHESIS) {
				st.push(it);
			}
			else if ((*it)->type == P_CLOSE_PARENTHESIS) {
				// create new parenthesis_block
				if (st.size() == 0) {
					error("Closed paranthesis has no open counterpart.", *it);
				}
				std::list<ParseNode*>::iterator leftPar = st.top();
				std::list<ParseNode*>::iterator leftNonPar = ++st.top();
				std::list<ParseNode*>::iterator rightPar = it;
				std::list<ParseNode*>::iterator end = ++it;
				--it;
				st.pop();
				ParseNode* newTree = new ParseNode(parenthesis_block, (*leftPar)->token.lineNum, (*leftPar)->token.charNum);
				for (std::list<ParseNode*>::iterator it2=leftNonPar; it2 != rightPar; ++it2) {
					--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
					newTree->children.push_back(*it2);
				}
				--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
				tree->children.insert(leftPar, newTree);
				tree->children.erase(leftPar, end);
			}
		}
	}
}

void Parser::error(std::string message, ParseNode* tree) {
	// delete me before final release
	std::cout << message << " (" << tree->token.lineNum << ", " << tree->token.charNum << ")\n";
	exit(0);
}

void Parser::doBracketPass(ParseNode* tree) {
	std::stack<std::list<ParseNode*>::iterator> st;
	for (std::list<ParseNode*>::iterator it=tree->children.begin(); it != tree->children.end(); ++it) {
		if ((*it)->children.size() > 0) {
			doBracketPass(*it);
		}
		else {
			if ((*it)->type == P_OPEN_BRACKET) {
				st.push(it);
			}
			else if ((*it)->type == P_CLOSE_BRACKET) {
				// create new parenthesis_block
				if (st.size() == 0) {
					error("Closed bracket has no open counterpart.", *it);
				}
				std::list<ParseNode*>::iterator leftPar = st.top();
				std::list<ParseNode*>::iterator leftNonPar = ++st.top();
				std::list<ParseNode*>::iterator rightPar = it;
				std::list<ParseNode*>::iterator end = ++it;
				--it;
				st.pop();
				ParseNode* newTree = new ParseNode(bracket_block, (*leftPar)->token.lineNum, (*leftPar)->token.charNum);
				for (std::list<ParseNode*>::iterator it2=leftNonPar; it2 != rightPar; ++it2) {
					--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
					newTree->children.push_back(*it2);
				}
				--debug_counter; if (debug_counter <= 0) { mainTree->print(); error("", mainTree); }
				tree->children.insert(leftPar, newTree);
				tree->children.erase(leftPar, end);
			}
		}
	}
}