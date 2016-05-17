#pragma once

#include "Token.hpp"
#include <vector>
#include <iostream>

struct ParseNode {
	ParseNode(const Token* tokens, uint64_t tokenLength, TokenType type) : tokens(tokens), tokenLength(tokenLength), type(type) {};

	void addChild(ParseNode* child) { _children.push_back(child); }

	friend std::ostream& operator<<(std::ostream& stream, const ParseNode& node) {
		node.print(stream);
		return stream;
	}

	const TokenType type;
	const Token* const tokens;
	const uint64_t tokenLength;
	const std::vector<ParseNode*>& children = _children;

private:

	void print(std::ostream& stream, uint64_t depth = 0) const {
		for (uint64_t i = 0; i < depth; ++i) {
			stream << "   ";
		}
		stream << Token::toString(type) << "[" << tokenLength << "]  " << Token::toString(tokens[0].type);
		if (tokenLength > 1) {
			stream << "  " << Token::toString(tokens[1].type);
		}
		stream << " ..." << std::endl;
		for (uint64_t i = 0; i < children.size(); ++i) {
			children[i]->print(stream, depth + 1);
		}
	}

	std::vector<ParseNode*> _children;
};