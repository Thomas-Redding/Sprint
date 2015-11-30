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
#include "Tree.h"

struct Rule {
    Rule(TokenType output, std::initializer_list<TokenType> components) : output(output), components(components) {}
    TokenType output;
    std::vector< TokenType > components;
};

struct Parser {
    Parser(std::vector<Rule>& rules) : rules(rules) {};
    Tree< std::pair<Token, size_t> >* match(const Token* A, size_t n, const Token& value);
    Tree< std::pair<Token, size_t> >* match(const Token* A, size_t n, const Rule& rule);
    std::vector< Rule > rules;
};

template<class A, class B>
std::ostream& operator<<(std::ostream& o, const std::pair<A, B>& p) {
    o << "(" << p.first << ", " << p.second << ")";
    return o;
}



#endif /* Parser_cpp */
