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
#include "ParseTree.hpp"

/*
 *
 * The pair<Token, size_t> contains a value for the node of the tree and the
 * number of tokens it is long
 *
*/

struct Rule {
    Rule(TokenType output, std::initializer_list<TokenType> components) : output(output), components(components) {}
    TokenType output;
    std::vector< TokenType > components;
};

struct Parser {
    Parser(std::vector<Rule>& rules) : rules(rules) {};
    ParseTree* match(const Token* A, size_t n, const Rule& rule);
private:
    ParseTree* match(const Token* A, size_t n, const Token& value);
    std::vector< Rule > rules;
};



#endif /* Parser_cpp */
