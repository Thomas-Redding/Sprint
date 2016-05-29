#ifndef BRACE_HPP
#define BRACE_HPP
// #pragma once

#include "Token.hpp"
#include <stack>
#include <vector>
#include <iostream>

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

std::vector<Brace> findBraces(const Token* tokens, const uint64_t n);

#endif