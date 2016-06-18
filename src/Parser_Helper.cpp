// #include "../include/Parser.hpp"

/*
 * expression, assignment-expression
 * conditional-expression
 * ? logical-or-expression : expression : assignment-expression
 * logical-or-expression or logical-and-expression
 * logical-and-expression and inclusive-and-expression
 * inclusive-or-expression | exclusive-or-expression
 * exclusive-or-expression ^ and-expression
 * and-expression & equality-expression
 * equality-expression == relational-expression						equality-expression != relational-expression		equality-expression === relational-expression		equality-expression !== relational-expression
 * relational-expression < shift-expression							relational-expression > shift-expression			relational-expression <= shift-expression			relational-expression >= shift-expression
 * shift-expression << additive-expression							shift-expression >> additive-expression
 * additive-expression + multiplicative-expression					additive-expression - multiplicative-expression
 * multiplicative-expression * obj-expression						multiplicative-expression / obj-expression		multiplicative-expression % obj-expression
 * obj-expression . IDENTIFIER
 * INTEGER_LITERAL, STRING_LITERAL, CHAR_LITERAL, FLOAT_LITERAL
 TODO
 new, delete, sizeof, typeof, ++, --, -, [], (), ::
 */
/*
static ParseNode* parseLine(const Token* tokens, uint64_t n) {
	std::cout << "foo";
	return new ParseNode(nullptr, 0, UNKNOWN);
}
*/