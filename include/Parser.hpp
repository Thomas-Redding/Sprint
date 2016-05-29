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

struct Parser {
	static ParseNode* getParseTree(const Token* tokens, uint64_t n);

private:
	// static std::vector<Brace> braces;
	static ParseNode* parseClass(const Token* tokens, uint64_t n);
	static ParseNode* parseMemberVariable(const Token* tokens, uint64_t n);
	static ParseNode* parseFunction(const Token* tokens, uint64_t n);
	static uint64_t _isValidFunctionName(const Token* tokens, uint64_t n);
	static ParseNode* parseLine(const Token* tokens, uint64_t n);
};

#endif