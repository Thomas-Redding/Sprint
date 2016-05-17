#pragma once

#include "ParseNode.hpp"
#include <assert.h>
#include <iostream>

struct Parser {
	static ParseNode* skimClass(const Token* tokens, uint64_t n);
	static uint64_t isValidFunctionName(const Token* tokens, uint64_t n);
	static ParseNode* skimFunction(const Token* tokens, uint64_t n);
	static ParseNode* getParseTree(const Token* tokens, uint64_t n);
};