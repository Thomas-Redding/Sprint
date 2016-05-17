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
	static ParseNode* skimClassVariable(const Token* tokens, uint64_t n);
	static ParseNode* skimClass(const Token* tokens, uint64_t n);
	static uint64_t isValidFunctionName(const Token* tokens, uint64_t n);
	static ParseNode* skimFunction(const Token* tokens, uint64_t n);
	static ParseNode* getParseTree(const Token* tokens, uint64_t n);
};

#endif