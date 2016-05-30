#pragma once

#include <assert.h>
#include <iostream>
#include <vector>

#include "../include/ParseNode.hpp"

// returns the index "i" of the value that satisfies "A[i] <= value"
// uses binary search and operator<(const T&, const T&)
template<class T>
uint64_t find(const T* A, uint64_t len, const T& value) {
    uint64_t a = 0;
    uint64_t b = len - 1;
    while (a + 1 < b) {
        (value < A[(a + b) / 2] ? b : a) = (a + b) / 2;
    }
    if (A[a + 1] <= value) {
        return a + 1;
    }
    return a;
}

struct Brace {
	Brace() {};
	Brace(TokenType type, uint64_t index_in_tokens, uint64_t index_of_pair) : type(type), index_in_tokens(index_in_tokens), index_of_pair(index_of_pair) {};
	TokenType type;
	uint64_t index_in_tokens;
	uint64_t index_of_pair;
	friend std::ostream& operator<<(std::ostream& stream, const Brace& brace) {
		return stream << Token::toString(brace.type) << " : " << brace.index_in_tokens << " : " << brace.index_of_pair;
	}
};

struct Parser {

	static ParseNode* getParseTree(const Token* tokens, uint64_t n);

private:

	static std::vector<Brace> braces;
	static ParseNode* parseClass(const Token* tokens, uint64_t n);
	static ParseNode* parseMemberVariable(const Token* tokens, uint64_t n);
	static ParseNode* parseFunction(const Token* tokens, uint64_t n);
	static uint64_t _isValidFunctionName(const Token* tokens, uint64_t n);
	static ParseNode* parseLine(const Token* tokens, uint64_t n);
	static std::vector<Brace> findBraces(const Token* tokens, const uint64_t n);
};