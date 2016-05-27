#ifndef BRACE_H
#define BRACE_H
// #pragma once

#include "Token.hpp"
#include <stack>

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

std::vector<Brace> findBraces(const Token* tokens, const uint64_t n) {
	uint64_t count = 0;
	for (uint64_t i = 0; i < n; ++i) {
		if (tokens[i].type == OPEN_CURLY_BRACE || tokens[i].type == OPEN_PARENTHESIS || tokens[i].type == OPEN_BRACKET) {
			count += 2;
		}
	}

	std::vector<Brace> rtn;
	std::stack<uint64_t> unclosed;

	rtn.reserve(count);
	uint64_t i = -1;
	while (++i < n && tokens[i].type != OPEN_PARENTHESIS && tokens[i].type != OPEN_BRACKET && tokens[i].type != OPEN_CURLY_BRACE) {}
	rtn.push_back(Brace(tokens[i].type, i, 0));
	unclosed.push(i);

	while (++i < n) {
		if (tokens[i].type == OPEN_CURLY_BRACE || tokens[i].type == OPEN_PARENTHESIS || tokens[i].type == OPEN_BRACKET) {
			rtn.push_back(Brace(tokens[i].type, i, 0));
			unclosed.push(i);
		}
		else if (tokens[i].type == CLOSE_CURLY_BRACE || tokens[i].type == CLOSE_PARENTHESIS || tokens[i].type == CLOSE_BRACKET) {
			uint64_t x = unclosed.top();
			unclosed.pop();
			if (tokens[i].type == CLOSE_CURLY_BRACE) {
				if (tokens[x].type != OPEN_CURLY_BRACE) {
					throw std::runtime_error("Error: open curly brace on line " + std::to_string(tokens[x].lineNum) + ", character " + std::to_string(tokens[x].charNum) + " is not closed.");
				}
				rtn.push_back(Brace(tokens[i].type, i, x));
				rtn[x].index_of_pair = i;
			}
			else if (tokens[i].type == CLOSE_PARENTHESIS) {
				if (tokens[x].type != OPEN_PARENTHESIS) {
					throw std::runtime_error("Error: open parenthesis on line " + std::to_string(tokens[x].lineNum) + ", character " + std::to_string(tokens[x].charNum) + " is not closed.");
				}
				rtn.push_back(Brace(tokens[i].type, i, x));
				rtn[x].index_of_pair = i;
			}
			else if (tokens[i].type == CLOSE_BRACKET) {
				if (tokens[x].type != OPEN_BRACKET) {
					throw std::runtime_error("Error: open bracket on line " + std::to_string(tokens[x].lineNum) + ", character " + std::to_string(tokens[x].charNum) + " is not closed.");
				}
				rtn.push_back(Brace(tokens[i].type, i, x));
				rtn[x].index_of_pair = i;
			}
			else {
				std::cout << Token::toString(tokens[i].type) << std::endl;
				throw std::runtime_error("????");
			}
		}
	}

	return rtn;
}

#endif