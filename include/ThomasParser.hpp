#ifndef PARSER_HPP
#define PARSER_HPP

#include <assert.h>
#include <iostream>

#include "ParseNode.hpp"
#include "Brace.hpp"

enum Exposure {
	PUBLIC,
	PROTECTED,
	READ_ONLY,
	PRIVATE,
};

struct ThomasNode {
	const Token* tokens;
	uint64_t len;
	std::vector<Token*> children;
	ThomasNode(const Token* t, uint64_t n)
	{
		tokens = t;
		len = n;
	}
}

struct ThomasParser {
	static ParseNode* getParseTree(const Token* t, uint64_t n) {
		this.tokens = t;
		this.len = n;
		for (int i=0; i<this.len; i++) {
			this.tree.push_back(ThomasNode(i, 1));
		}
		doParenthesesPass();
	}
private:
	const Token* tokens;
	uint64_t len;
	std::vector<ParseNode*> trees;
	void doParenthesesPass() {
		int depth = 0;
		for (int i=0; i<trees.size(); i++) {
			if (trees[i].tokens[0].type == OPEN_PARENTHESIS) {
				depth++;
			}
			else if (trees[i].tokens[0].type == CLOSE_PARENTHESIS) {
				depth++;
			}
		}
	}
};

#endif