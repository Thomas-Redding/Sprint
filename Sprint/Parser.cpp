//
//  Parser.cpp
//  Sprint
//
//  Created by Thomas Redding on 10/22/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "Parser.hpp"

ParseTree* Parser::match(const Token* A, size_t n, const Rule& rule) {
    ParseTree* rtn = new ParseTree(std::pair<const Token*, size_t>(new Token(rule.output), 0));
    const std::vector< TokenType >& components = rule.components;
    size_t i = 0;
    for (size_t j = 0; j < components.size(); ++j) {
        if (components[j] == TokenType::etc) {
            j++;
            if (components[j] == TokenType::etc) {
                throw std::runtime_error("What in Jesus' name is \"etc, etc\" supposed to mean?");
            }
            else if (components[j] == TokenType::etc_not) {
                throw std::runtime_error("What in Jesus' name is \"etc, etc_not\" supposed to mean?");
            }
            while (i < n) {
                ParseTree* x = match(A + i, n - i, components[j]);
                if (x == nullptr) {
                    break;
                }
                rtn->add_child(x);
                i += x->value.second;
            }
        }
        else if (components[j] == TokenType::etc_not) {
            j++;
            if (components[j] == TokenType::etc) {
                throw std::runtime_error("What in Jesus' name is \"etc_not, etc\" supposed to mean?");
            }
            else if (components[j] == TokenType::etc_not) {
                throw std::runtime_error("What in Jesus' name is \"etc_not, etc_not\" supposed to mean?");
            }
            while (i < n) {
                ParseTree* x = match(A + i, n - i, components[j]);
                if (x != nullptr) {
                    delete x;// added
                    break;
                }
                i++;
            }
        }
        else {
            ParseTree* x = match(A + i, n - i, components[j]);
            if (x == nullptr) {
                delete rtn;
                return nullptr;
            }
            rtn->add_child(x);
            i += x->value.second;
        }
    }
    rtn->value.second = i;
    return rtn;
}

ParseTree* Parser::match(const Token* A, size_t n, const Token& value) {
	
    if (value.type < token_threshold) {
        if (value.type == A[0].type) {
            return new ParseTree(std::pair<const Token*, size_t>(new Token(value), 1));
        }
        else {
            return nullptr;
        }
    }
    
    for (size_t i = 0; i < rules.size(); ++i) {
        if (rules[i].output != value.type) {
            continue;
        }
        ParseTree* x = match(A, n, rules[i]);
        if (x != nullptr) {
            return x;
        }
    }
    return nullptr;
}

