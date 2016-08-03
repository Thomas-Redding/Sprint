#include "../include/ThomasParser.hpp"


std::string treeTypeToString(TreeType t) {
	if (t == T_KEYWORD)
		return "T_KEYWORD";
	else if (t == T_IDENTIFIER)
		return "T_IDENTIFIER";
	else if (t == T_INTEGER_LITERAL)
		return "T_INTEGER_LITERAL";
	else if (t == T_FLOAT_LITERAL)
		return "T_FLOAT_LITERAL";
	else if (t == T_FLOAT_LITERAL2)
		return "T_FLOAT_LITERAL2";
	else if (t == T_CHARACTER_LITERAL)
		return "T_CHARACTER_LITERAL";
	else if (t == T_STRING_LITERAL)
		return "T_STRING_LITERAL";
	else if (t == T_PUNCTUATION)
		return "T_PUNCTUATION";
	else if (t == T_BRACKET)
		return "T_BRACKET";
	else if (t == T_UNKNOWN)
		return "T_UNKNOWN";
	else if (t == T_NULL_TOKEN_TYPE)
		return "T_NULL_TOKEN_TYPE";
	else if (t == T_OPEN_PARENTHESIS)
		return "T_OPEN_PARENTHESIS";
	else if (t == T_CLOSE_PARENTHESIS)
		return "T_CLOSE_PARENTHESIS";
	else if (t == T_OPEN_BRACKET)
		return "T_OPEN_BRACKET";
	else if (t == T_CLOSE_BRACKET)
		return "T_CLOSE_BRACKET";
	else if (t == T_OPEN_CURLY_BRACE)
		return "T_OPEN_CURLY_BRACE";
	else if (t == T_CLOSE_CURLY_BRACE)
		return "T_CLOSE_CURLY_BRACE";
	else if (t == T_KEYWORD_ABSTRACT)
		return "T_KEYWORD_ABSTRACT";
	else if (t == T_KEYWORD_BREAK)
		return "T_KEYWORD_BREAK";
	else if (t == T_KEYWORD_CASE)
		return "T_KEYWORD_CASE";
	else if (t == T_KEYWORD_CATCH)
		return "T_KEYWORD_CATCH";
	else if (t == T_KEYWORD_CLASS)
		return "T_KEYWORD_CLASS";
	else if (t == T_KEYWORD_CONST)
		return "T_KEYWORD_CONST";
	else if (t == T_KEYWORD_CONTINUE)
		return "T_KEYWORD_CONTINUE";
	else if (t == T_KEYWORD_DELETE)
		return "T_KEYWORD_DELETE";
	else if (t == T_KEYWORD_DO)
		return "T_KEYWORD_DO";
	else if (t == T_KEYWORD_ELSE)
		return "T_KEYWORD_ELSE";
	else if (t == T_KEYWORD_ENUM)
		return "T_KEYWORD_ENUM";
	else if (t == T_KEYWORD_FALSE)
		return "T_KEYWORD_FALSE";
	else if (t == T_KEYWORD_FOR)
		return "T_KEYWORD_FOR";
	else if (t == T_KEYWORD_IF)
		return "T_KEYWORD_IF";
	else if (t == T_KEYWORD_IN)
		return "T_KEYWORD_IN";
	else if (t == T_KEYWORD_INLINE)
		return "T_KEYWORD_INLINE";
	else if (t == T_KEYWORD_NEW)
		return "T_KEYWORD_NEW";
	else if (t == T_KEYWORD_NULL)
		return "T_KEYWORD_NULL";
	else if (t == T_KEYWORD_PROTECTED)
		return "T_KEYWORD_PROTECTED";
	else if (t == T_KEYWORD_PRIVATE)
		return "T_KEYWORD_PRIVATE";
	else if (t == T_KEYWORD_PTR)
		return "T_KEYWORD_PTR";
	else if (t == T_KEYWORD_REF)
		return "T_KEYWORD_REF";
	else if (t == T_KEYWORD_RETURN)
		return "T_KEYWORD_RETURN";
	else if (t == T_KEYWORD_SIZEOF)
		return "T_KEYWORD_SIZEOF";
	else if (t == T_KEYWORD_STATIC)
		return "T_KEYWORD_STATIC";
	else if (t == T_KEYWORD_STRUCT)
		return "T_KEYWORD_STRUCT";
	else if (t == T_KEYWORD_SWITCH) 
		return "T_KEYWORD_SWITCH"; 
	else if (t == T_KEYWORD_THIS)
		return "T_KEYWORD_THIS";
	else if (t == T_KEYWORD_THROW)
		return "T_KEYWORD_THROW";
	else if (t == T_KEYWORD_TRUE)
		return "T_KEYWORD_TRUE";
	else if (t == T_KEYWORD_TRY)
		return "T_KEYWORD_TRY";
	else if (t == T_KEYWORD_VIRTUAL)
		return "T_KEYWORD_VIRTUAL";
	else if (t == T_KEYWORD_WHILE)
		return "T_KEYWORD_WHILE";
	else if (t == T_KEYWORD_INT)
		return "T_KEYWORD_INT";
	else if (t == T_KEYWORD_INT8)
		return "T_KEYWORD_INT8";
	else if (t == T_KEYWORD_INT16)
		return "T_KEYWORD_INT16";
	else if (t == T_KEYWORD_INT32)
		return "T_KEYWORD_INT32";
	else if (t == T_KEYWORD_INT64)
		return "T_KEYWORD_INT64";
	else if (t == T_KEYWORD_UINT)
		return "T_KEYWORD_UINT";
	else if (t == T_KEYWORD_UINT8)
		return "T_KEYWORD_UINT8";
	else if (t == T_KEYWORD_UINT16)
		return "T_KEYWORD_UINT16";
	else if (t == T_KEYWORD_UINT32)
		return "T_KEYWORD_UINT32";
	else if (t == T_KEYWORD_UINT64)
		return "T_KEYWORD_UINT64";
	else if (t == T_KEYWORD_AND)
		return "T_KEYWORD_AND";
	else if (t == T_KEYWORD_OR)
		return "T_KEYWORD_OR";
	else if (t == T_KEYWORD_NOT)
		return "T_KEYWORD_NOT";
	else if (t == T_KEYWORD_XOR)
		return "T_KEYWORD_XOR";
	else if (t == T_KEYWORD_FLOAT)
		return "T_KEYWORD_FLOAT";
	else if (t == T_KEYWORD_DOUBLE)
		return "T_KEYWORD_DOUBLE";
	else if (t == T_KEYWORD_PUBLIC)
		return "T_KEYWORD_PUBLIC";
	else if (t == T_KEYWORD_VOID)
		return "T_KEYWORD_VOID";
	else if (t == T_PERIOD)
		return "T_PERIOD";
	else if (t == T_COLON)
		return "T_COLON";
	else if (t == T_SEMI_COLON)
		return "T_SEMI_COLON";
	else if (t == T_PLUS)
		return "T_PLUS";
	else if (t == T_MINUS)
		return "T_MINUS";
	else if (t == T_ASTERISK)
		return "T_ASTERISK";
	else if (t == T_SLASH)
		return "T_SLASH";
	else if (t == T_AMPERSAND)
		return "T_AMPERSAND";
	else if (t == T_POUND_SIGN)
		return "T_POUND_SIGN";
	else if (t == T_LESS_THAN)
		return "T_LESS_THAN";
	else if (t == T_EQUALS)
		return "T_EQUALS";
	else if (t == T_GREATER_THAN)
		return "T_GREATER_THAN";
	else if (t == T_COMMA)
		return "T_COMMA";
	else if (t == T_VERTICAL_BAR)
		return "T_VERTICAL_BAR";
	else if (t == T_PERCENT)
		return "T_PERCENT";
	else if (t == T_EXCLAMATION_POINT)
		return "T_EXCLAMATION_POINT";
	else if (t == T_CARROT)
		return "T_CARROT";
	else if (t == T_QUESTION_MARK)
		return "T_QUESTION_MARK";
	else if (t == T_BACK_SLASH)
		return "T_BACK_SLASH";
	else if (t == T_AT)
		return "T_AT";
	else if (t == T_TILDE)
		return "T_TILDE";
	else if (t == T_PLUS_EQUALS)
		return "T_PLUS_EQUALS";
	else if (t == T_MINUS_EQUALS)
		return "T_MINUS_EQUALS";
	else if (t == T_SLASH_EQUALS)
		return "T_SLASH_EQUALS";
	else if (t == T_ASTERISK_EQUALS)
		return "T_ASTERISK_EQUALS";
	else if (t == T_AMPERSAND_EQUALS)
		return "T_AMPERSAND_EQUALS";
	else if (t == T_CARROT_EQUALS)
		return "T_CARROT_EQUALS";
	else if (t == T_COLON_EQUALS)
		return "T_COLON_EQUALS";
	else if (t == T_VERTICAL_BAR_EQUALS)
		return "T_VERTICAL_BAR_EQUALS";
	else if (t == T_PERCENT_EQUALS)
		return "T_PERCENT_EQUALS";
	else if (t == T_PLUS_PLUS)
		return "T_PLUS_PLUS";
	else if (t == T_MINUS_MINUS)
		return "T_MINUS_MINUS";
	else if (t == T_SHIFT_LEFT)
		return "T_SHIFT_LEFT";
	else if (t == T_SHIFT_RIGHT)
		return "T_SHIFT_RIGHT";
	else if (t == T_GREATER_THAN_EQUALS)
		return "T_GREATER_THAN_EQUALS";
	else if (t == T_LESS_THAN_EQUALS)
		return "T_LESS_THAN_EQUALS";
	else if (t == T_SHIFT_LEFT_EQUALS)
		return "T_SHIFT_LEFT_EQUALS";
	else if (t == T_SHIFT_RIGHT_EQUALS)
		return "T_SHIFT_RIGHT_EQUALS";
	else if (t == T_EXCLAMATION_POINT_EQUALS)
		return "T_EXCLAMATION_POINT_EQUALS";
	else if (t == T_EQUAL_EQUALS)
		return "T_EQUAL_EQUALS";
	else if (t == T_BACK_SLASH_EQUALS)
		return "T_BACK_SLASH_EQUALS";
	else if (t == T_ARROW)
		return "T_ARROW";
	else if (t == T_EQUAL_EQUAL_EQUALS)
		return "T_EQUAL_EQUAL_EQUALS";
	else if (t == T_EXCLAMATION_POINT_EQUAL_EQUALS)
		return "T_EXCLAMATION_POINT_EQUAL_EQUALS";
	else if (t == curly_brace_block)
		return "curly_brace_block";
	else if (t == parenthesis_block)
		return "parenthesis_block";
	else if (t == bracket_block)
		return "bracket_block";
	else if (t == mult_clause)
		return "mult_clause";
	else if (t == plus_clause)
		return "plus_clause";
	else if (t == general)
		return "general";
	else if (t == unary_value)
		return "unary_value";
	else if (t == unary_clause)
		return "unary_clause";
	else if (t == mult_value)
		return "mult_value";
	else if (t == mult_clause)
		return "mult_clause";
	else if (t == plus_value)
		return "plus_value";
	else if (t == plus_clause)
		return "plus_clause";
	else if (t == shift_value)
		return "shift_value";
	else if (t == shift_clause)
		return "shift_clause";
	else if (t == inequality_value)
		return "inequality_value";
	else if (t == inequality_clause)
		return "inequality_clause";
	else if (t == equality_value)
		return "equality_value";
	else if (t == equality_clause)
		return "equality_clause";
	else if (t == bitwise_and_value)
		return "bitwise_and_value";
	else if (t == bitwise_and_clause)
		return "bitwise_and_clause";
	else if (t == bitwise_xor_value)
		return "bitwise_xor_value";
	else if (t == bitwise_xor_clause)
		return "bitwise_xor_clause";
	else if (t == bitwise_or_value)
		return "bitwise_or_value";
	else if (t == bitwise_or_clause)
		return "bitwise_or_clause";
	else if (t == setting_value)
		return "setting_value";
	else if (t == setting_clause)
		return "setting_clause";
	else if (t == ternary_clause)
		return "ternary_clause";
	else if (t == comma_value)
		return "comma_value";
	else if (t == comma_clause)
		return "comma_clause";
	else if (t == T_POSITIVE)
		return "T_POSITIVE";
	else if (t == T_NEGATIVE)
		return "T_NEGATIVE";
	else if (t == T_PTR)
		return "T_PTR";
	else if (t == T_KEYWORD_BOOL)
		return "T_KEYWORD_BOOL";
	else if (t == T_KEYWORD_CHAR)
		return "T_KEYWORD_CHAR";
	else if (t == T_KEYWORD_VAR)
		return "T_KEYWORD_VAR";
	else
		return std::to_string(static_cast<TreeType>(t));
}

ThomasNode::ThomasNode(Token t) {
	token = t;
	type = translateType(t.type);
}

TreeType translateType(TokenType t) {
	if (t == KEYWORD)
		return T_KEYWORD;
	else if (t == IDENTIFIER)
		return T_IDENTIFIER;
	else if (t == INTEGER_LITERAL)
		return T_INTEGER_LITERAL;
	else if (t == FLOAT_LITERAL)
		return T_FLOAT_LITERAL;
	else if (t == FLOAT_LITERAL2)
		return T_FLOAT_LITERAL2;
	else if (t == CHARACTER_LITERAL)
		return T_CHARACTER_LITERAL;
	else if (t == STRING_LITERAL)
		return T_STRING_LITERAL;
	else if (t == PUNCTUATION)
		return T_PUNCTUATION;
	else if (t == BRACKET)
		return T_BRACKET;
	else if (t == UNKNOWN)
		return T_UNKNOWN;
	else if (t == NULL_TOKEN_TYPE)
		return T_NULL_TOKEN_TYPE;
	else if (t == OPEN_PARENTHESIS)
		return T_OPEN_PARENTHESIS;
	else if (t == CLOSE_PARENTHESIS)
		return T_CLOSE_PARENTHESIS;
	else if (t == OPEN_BRACKET)
		return T_OPEN_BRACKET;
	else if (t == CLOSE_BRACKET)
		return T_CLOSE_BRACKET;
	else if (t == OPEN_CURLY_BRACE)
		return T_OPEN_CURLY_BRACE;
	else if (t == CLOSE_CURLY_BRACE)
		return T_CLOSE_CURLY_BRACE;
	else if (t == KEYWORD_ABSTRACT)
		return T_KEYWORD_ABSTRACT;
	else if (t == KEYWORD_BREAK)
		return T_KEYWORD_BREAK;
	else if (t == KEYWORD_CASE)
		return T_KEYWORD_CASE;
	else if (t == KEYWORD_CATCH)
		return T_KEYWORD_CATCH;
	else if (t == KEYWORD_CLASS)
		return T_KEYWORD_CLASS;
	else if (t == KEYWORD_CONST)
		return T_KEYWORD_CONST;
	else if (t == KEYWORD_CONTINUE)
		return T_KEYWORD_CONTINUE;
	else if (t == KEYWORD_DELETE)
		return T_KEYWORD_DELETE;
	else if (t == KEYWORD_DO)
		return T_KEYWORD_DO;
	else if (t == KEYWORD_ELSE)
		return T_KEYWORD_ELSE;
	else if (t == KEYWORD_ENUM)
		return T_KEYWORD_ENUM;
	else if (t == KEYWORD_FALSE)
		return T_KEYWORD_FALSE;
	else if (t == KEYWORD_FOR)
		return T_KEYWORD_FOR;
	else if (t == KEYWORD_IF)
		return T_KEYWORD_IF;
	else if (t == KEYWORD_IN)
		return T_KEYWORD_IN;
	else if (t == KEYWORD_INLINE)
		return T_KEYWORD_INLINE;
	else if (t == KEYWORD_NEW)
		return T_KEYWORD_NEW;
	else if (t == KEYWORD_NULL)
		return T_KEYWORD_NULL;
	else if (t == KEYWORD_PROTECTED)
		return T_KEYWORD_PROTECTED;
	else if (t == KEYWORD_PRIVATE)
		return T_KEYWORD_PRIVATE;
	else if (t == KEYWORD_PTR)
		return T_KEYWORD_PTR;
	else if (t == KEYWORD_REF)
		return T_KEYWORD_REF;
	else if (t == KEYWORD_RETURN)
		return T_KEYWORD_RETURN;
	else if (t == KEYWORD_SIZEOF)
		return T_KEYWORD_SIZEOF;
	else if (t == KEYWORD_STATIC)
		return T_KEYWORD_STATIC;
	else if (t == KEYWORD_STRUCT)
		return T_KEYWORD_STRUCT;
	else if (t == KEYWORD_SWITCH)
		return T_KEYWORD_SWITCH;
	else if (t == KEYWORD_THIS)
		return T_KEYWORD_THIS;
	else if (t == KEYWORD_THROW)
		return T_KEYWORD_THROW;
	else if (t == KEYWORD_TRUE)
		return T_KEYWORD_TRUE;
	else if (t == KEYWORD_TRY)
		return T_KEYWORD_TRY;
	else if (t == KEYWORD_VIRTUAL)
		return T_KEYWORD_VIRTUAL;
	else if (t == KEYWORD_WHILE)
		return T_KEYWORD_WHILE;
	else if (t == KEYWORD_INT)
		return T_KEYWORD_INT;
	else if (t == KEYWORD_INT8)
		return T_KEYWORD_INT8;
	else if (t == KEYWORD_INT16)
		return T_KEYWORD_INT16;
	else if (t == KEYWORD_INT32)
		return T_KEYWORD_INT32;
	else if (t == KEYWORD_INT64)
		return T_KEYWORD_INT64;
	else if (t == KEYWORD_UINT)
		return T_KEYWORD_UINT;
	else if (t == KEYWORD_UINT8)
		return T_KEYWORD_UINT8;
	else if (t == KEYWORD_UINT16)
		return T_KEYWORD_UINT16;
	else if (t == KEYWORD_UINT32)
		return T_KEYWORD_UINT32;
	else if (t == KEYWORD_UINT64)
		return T_KEYWORD_UINT64;
	else if (t == KEYWORD_AND)
		return T_KEYWORD_AND;
	else if (t == KEYWORD_OR)
		return T_KEYWORD_OR;
	else if (t == KEYWORD_NOT)
		return T_KEYWORD_NOT;
	else if (t == KEYWORD_XOR)
		return T_KEYWORD_XOR;
	else if (t == KEYWORD_FLOAT)
		return T_KEYWORD_FLOAT;
	else if (t == KEYWORD_DOUBLE)
		return T_KEYWORD_DOUBLE;
	else if (t == KEYWORD_PUBLIC)
		return T_KEYWORD_PUBLIC;
	else if (t == KEYWORD_VOID)
		return T_KEYWORD_VOID;
	else if (t == PERIOD)
		return T_PERIOD;
	else if (t == COLON)
		return T_COLON;
	else if (t == SEMI_COLON)
		return T_SEMI_COLON;
	else if (t == PLUS)
		return T_PLUS;
	else if (t == MINUS)
		return T_MINUS;
	else if (t == ASTERISK)
		return T_ASTERISK;
	else if (t == SLASH)
		return T_SLASH;
	else if (t == AMPERSAND)
		return T_AMPERSAND;
	else if (t == POUND_SIGN)
		return T_POUND_SIGN;
	else if (t == LESS_THAN)
		return T_LESS_THAN;
	else if (t == EQUALS)
		return T_EQUALS;
	else if (t == GREATER_THAN)
		return T_GREATER_THAN;
	else if (t == COMMA)
		return T_COMMA;
	else if (t == VERTICAL_BAR)
		return T_VERTICAL_BAR;
	else if (t == PERCENT)
		return T_PERCENT;
	else if (t == EXCLAMATION_POINT)
		return T_EXCLAMATION_POINT;
	else if (t == CARROT)
		return T_CARROT;
	else if (t == QUESTION_MARK)
		return T_QUESTION_MARK;
	else if (t == BACK_SLASH)
		return T_BACK_SLASH;
	else if (t == AT)
		return T_AT;
	else if (t == TILDE)
		return T_TILDE;
	else if (t == PLUS_EQUALS)
		return T_PLUS_EQUALS;
	else if (t == MINUS_EQUALS)
		return T_MINUS_EQUALS;
	else if (t == SLASH_EQUALS)
		return T_SLASH_EQUALS;
	else if (t == ASTERISK_EQUALS)
		return T_ASTERISK_EQUALS;
	else if (t == AMPERSAND_EQUALS)
		return T_AMPERSAND_EQUALS;
	else if (t == CARROT_EQUALS)
		return T_CARROT_EQUALS;
	else if (t == COLON_EQUALS)
		return T_COLON_EQUALS;
	else if (t == VERTICAL_BAR_EQUALS)
		return T_VERTICAL_BAR_EQUALS;
	else if (t == PERCENT_EQUALS)
		return T_PERCENT_EQUALS;
	else if (t == PLUS_PLUS)
		return T_PLUS_PLUS;
	else if (t == MINUS_MINUS)
		return T_MINUS_MINUS;
	else if (t == SHIFT_LEFT)
		return T_SHIFT_LEFT;
	else if (t == SHIFT_RIGHT)
		return T_SHIFT_RIGHT;
	else if (t == GREATER_THAN_EQUALS)
		return T_GREATER_THAN_EQUALS;
	else if (t == LESS_THAN_EQUALS)
		return T_LESS_THAN_EQUALS;
	else if (t == SHIFT_LEFT_EQUALS)
		return T_SHIFT_LEFT_EQUALS;
	else if (t == SHIFT_RIGHT_EQUALS)
		return T_SHIFT_RIGHT_EQUALS;
	else if (t == EXCLAMATION_POINT_EQUALS)
		return T_EXCLAMATION_POINT_EQUALS;
	else if (t == EQUAL_EQUALS)
		return T_EQUAL_EQUALS;
	else if (t == BACK_SLASH_EQUALS)
		return T_BACK_SLASH_EQUALS;
	else if (t == ARROW)
		return T_ARROW;
	else if (t == EQUAL_EQUAL_EQUALS)
		return T_EQUAL_EQUAL_EQUALS;
	else if (t == EXCLAMATION_POINT_EQUAL_EQUALS)
		return T_EXCLAMATION_POINT_EQUAL_EQUALS;
	else if (t == POSITIVE)
		return T_POSITIVE;
	else if (t == NEGATIVE)
		return T_NEGATIVE;
	else if (t == PTR)
		return T_PTR;
	else if (t == KEYWORD_BOOL)
		return T_KEYWORD_BOOL;
	else if (t == KEYWORD_CHAR)
		return T_KEYWORD_CHAR;
	else if (t == KEYWORD_VAR)
		return T_KEYWORD_VAR;
	else
		return T_UNKNOWN;
}

ThomasNode* ThomasParser::getParseTree(const Token* t, uint64_t n) {
	auto timeStart = std::chrono::high_resolution_clock::now();
	tokens = t;
	len = n;
	mainTree = new ThomasNode(general);
	for (int i=0; i<len; ++i) {
		mainTree->children.push_back(new ThomasNode(tokens[i]));
	}
	auto timeSetUp = std::chrono::high_resolution_clock::now();
	doCurlyBracePass(mainTree);
	doParenthesesPass(mainTree);
	doBracketPass(mainTree);
	auto timeBrackets = std::chrono::high_resolution_clock::now();
	parse();
	auto timeEnd = std::chrono::high_resolution_clock::now();
	std::cout << "Parser Set Up: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeSetUp - timeStart).count() / 1000 << " µs\n";
	std::cout << "Parser Brackets: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeBrackets - timeSetUp).count() / 1000 << " µs\n";
	std::cout << "Parser Grammar: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeBrackets).count() / 1000 << " µs\n";
	/*
	 * set-up: 6%
	 * brackets: 5%
	 * adding new nodes to the tree: 27%
	 * other parsing: 62%
	 */
	return mainTree;
}

bool ThomasParser::thomasParserPrecedenceSorter(ThomasParseRule r1, ThomasParseRule r2) {
	return r1.precedence < r2.precedence;
}

void ThomasParser::parse() {
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
			parseLeftRight(mainTree, from, to);
		else
			parseRightLeft(mainTree, from, to);
	}
}

void ThomasParser::parseLeftRight(ThomasNode *tree, int from, int to) {
	bool skipRecursion = false;
	for (std::list<ThomasNode*>::iterator it = tree->children.begin(); it != tree->children.end();) {
		if ((*it)->children.size() > 0 && !skipRecursion) {
			parseLeftRight(*it, from, to);
			if ((*it)->children.size() == 1) {
				if ((*it)->type == parenthesis_block) {
					if (parCollapse.find((*((*it)->children.begin()))->type) != parCollapse.end()) {
						(*it) = *((*it)->children.begin());
						break;
					}
				}
			}
		}

		int ruleToApply = -1;
		int ruleSize = 0;
		for (int i = from; i < to; ++i) {
			if (rules[i].parent == general || rules[i].parent == tree->type) {
				int j;
				std::list<ThomasNode*>::iterator it2 = it;
				for (j = 0; j < rules[i].from.size(); ++j) {
					if (rules[i].from[j] < unary_value) {
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
			std::list<ThomasNode*>::iterator it2 = it;
			ThomasNode *newTree = new ThomasNode(rules[ruleToApply].to);
			for (int i= 0; i < ruleSize; ++i) {
				newTree->children.push_back(*it2);
				++it2;
			}
			std::list<ThomasNode*>::iterator before = it;
			--before;
			tree->children.erase(it, it2);
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

std::string nodeToStr(ThomasNode node) {
	std::string rtn = "(";
	rtn += treeTypeToString(node.type);
	rtn += ", ";
	rtn += node.token.str;
	rtn += ")";
	return rtn;
}

void ThomasParser::parseRightLeft(ThomasNode *tree, int from, int to) {
	for (std::list<ThomasNode*>::iterator it = --tree->children.end(); true; --it) {
		if ((*it)->children.size() > 0) {
			parseRightLeft(*it, from, to);
			if ((*it)->children.size() == 1) {
				if ((*it)->type == parenthesis_block) {
					if (parCollapse.find((*((*it)->children.begin()))->type) != parCollapse.end()) {
						(*it) = *((*it)->children.begin());
						break;
					}
				}
			}
		}

		int ruleToApply = -1;
		int ruleSize = 0;
		for (int i = from; i < to; ++i) {
			if (rules[i].parent == general || rules[i].parent == tree->type) {
				std::list<ThomasNode*>::iterator it2 = it;
				int j;
				for (j = 0; j < rules[i].from.size(); ++j) {
					if (rules[i].from[j] < unary_value) {
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
			std::list<ThomasNode*>::iterator it2 = it;
			ThomasNode *newTree = new ThomasNode(rules[ruleToApply].to);
			for (int i= 0; i < ruleSize; ++i) {
				newTree->children.push_back(*it2);
				++it2;
			}
			std::list<ThomasNode*>::iterator before = it;
			--before;
			tree->children.erase(it, it2);
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

void ThomasParser::doCurlyBracePass(ThomasNode* tree) {
	std::stack<std::list<ThomasNode*>::iterator> st;
	for (std::list<ThomasNode*>::iterator it=++tree->children.begin(); it != tree->children.end(); ++it) {
		if ((*it)->children.size() > 0) {
			doParenthesesPass(*it);
		}
		else {
			if ((*it)->type == T_OPEN_CURLY_BRACE) {
				st.push(it);
			}
			else if ((*it)->type == T_CLOSE_CURLY_BRACE) {
				// create new parenthesis_block
				ThomasNode* newTree = new ThomasNode(curly_brace_block);
				std::list<ThomasNode*>::iterator leftPar = st.top();
				std::list<ThomasNode*>::iterator leftNonPar = ++st.top();
				std::list<ThomasNode*>::iterator rightPar = it;
				std::list<ThomasNode*>::iterator end = ++it;
				--it;
				st.pop();
				for (std::list<ThomasNode*>::iterator it2=leftNonPar; it2 != rightPar; ++it2)
					newTree->children.push_back(*it2);
				tree->children.insert(leftPar, newTree);
				tree->children.erase(leftPar, end);
			}
		}
	}
}

void ThomasParser::doParenthesesPass(ThomasNode* tree) {
	std::stack<std::list<ThomasNode*>::iterator> st;
	for (std::list<ThomasNode*>::iterator it=++tree->children.begin(); it != tree->children.end(); ++it) {
		if ((*it)->children.size() > 0) {
			doParenthesesPass(*it);
		}
		else {
			if ((*it)->type == T_OPEN_PARENTHESIS) {
				st.push(it);
			}
			else if ((*it)->type == T_CLOSE_PARENTHESIS) {
				// create new parenthesis_block
				ThomasNode* newTree = new ThomasNode(parenthesis_block);
				std::list<ThomasNode*>::iterator leftPar = st.top();
				std::list<ThomasNode*>::iterator leftNonPar = ++st.top();
				std::list<ThomasNode*>::iterator rightPar = it;
				std::list<ThomasNode*>::iterator end = ++it;
				--it;
				st.pop();
				for (std::list<ThomasNode*>::iterator it2=leftNonPar; it2 != rightPar; ++it2)
					newTree->children.push_back(*it2);
				tree->children.insert(leftPar, newTree);
				tree->children.erase(leftPar, end);
			}
		}
	}
}

void ThomasParser::doBracketPass(ThomasNode* tree) {
	std::stack<std::list<ThomasNode*>::iterator> st;
	for (std::list<ThomasNode*>::iterator it=++tree->children.begin(); it != tree->children.end(); ++it) {
		if ((*it)->children.size() > 0) {
			doParenthesesPass(*it);
		}
		else {
			if ((*it)->type == T_OPEN_BRACKET) {
				st.push(it);
			}
			else if ((*it)->type == T_CLOSE_BRACKET) {
				// create new parenthesis_block
				ThomasNode* newTree = new ThomasNode(bracket_block);
				std::list<ThomasNode*>::iterator leftPar = st.top();
				std::list<ThomasNode*>::iterator leftNonPar = ++st.top();
				std::list<ThomasNode*>::iterator rightPar = it;
				std::list<ThomasNode*>::iterator end = ++it;
				--it;
				st.pop();
				for (std::list<ThomasNode*>::iterator it2=leftNonPar; it2 != rightPar; ++it2)
					newTree->children.push_back(*it2);
				tree->children.insert(leftPar, newTree);
				tree->children.erase(leftPar, end);
			}
		}
	}
}