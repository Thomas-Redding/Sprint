//
//  Parser.hpp
//  Sprint
//
//  Created by Thomas Redding on 10/22/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Parser_cpp
#define Parser_cpp


#include <stdio.h>
#include <vector>
#include <iostream>
#include "Token.hpp"
#include "Grammar.hpp"


class Parser {
public:
	Parser();
    
    Node* parse(const std::vector<Token>& tokens);
    Node* parse(const Token* tokens, const size_t n, const Rule& grammarRule);
    
    // returns the rule and the length of tokens used to satisfy it
    std::pair<Rule, size_t> findGrammarMatch(const Token* tokens, const size_t n) const;
    std::pair<Rule, size_t> findGrammarMatch(const Token* tokens, const size_t n, WordType output) const;
    
private:
    Grammar grammar;
	int index;
	void exit(std::string message);
};

#endif /* Parser_cpp */
