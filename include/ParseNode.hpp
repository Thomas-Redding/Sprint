#ifndef PARSENODE_H
#define PARSENODE_H

#include "Token.hpp"
#include <vector>

struct ParseNode {
	ParseNode(const Token* tokens, uint64_t tokenLength, TokenType type) : tokens(tokens), tokenLength(tokenLength), type(type) {};

	void addChild(ParseNode* child) { children.push_back(child); }

	uint64_t number_of_children() const { return children.size(); }

	const TokenType type;
	const Token* const tokens;
	const uint64_t tokenLength;

private:
	std::vector<ParseNode*> children;
};

#endif