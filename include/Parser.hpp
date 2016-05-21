#ifndef PARSER_H
#define PARSER_H

#include "ParseNode.hpp"
#include <assert.h>
#include <iostream>

enum Exposure {
	PUBLIC,
	PROTECTED,
	READ_ONLY,
	PRIVATE,
};

struct Parser {

	static ParseNode* getParseTree(const Token* tokens, uint64_t n);

private:

	static ParseNode* parseClass(const Token* tokens, uint64_t n);
	static ParseNode* parseMemberVariable(const Token* tokens, uint64_t n);
	static ParseNode* parseFunction(const Token* tokens, uint64_t n);

	static uint64_t _isValidFunctionName(const Token* tokens, uint64_t n);
};

#endif