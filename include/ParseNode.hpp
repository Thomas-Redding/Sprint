#pragma once

#include "Token.hpp"
#include <vector>

struct ParseNode {
	ParseNode(const Token* tokens, uint64_t tokenLength, TokenType type) : tokens(tokens), tokenLength(tokenLength), type(type) {};

	void addChild(ParseNode* child) { _children.push_back(child); }

	const TokenType type;
	const Token* const tokens;
	const uint64_t tokenLength;
	const std::vector<ParseNode*>& children = _children;

private:
	std::vector<ParseNode*> _children;
};