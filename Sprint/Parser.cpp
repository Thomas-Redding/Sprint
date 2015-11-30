//
//  Parser.cpp
//  Sprint
//
//  Created by Thomas Redding on 10/22/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "Parser.hpp"
#include "Token.hpp"

Tree< std::pair<Token, size_t> >* Parser::match(const Token* A, size_t n, const Rule& rule) {
    Tree< std::pair<Token, size_t> >* rtn = new Tree< std::pair<Token, size_t> >(std::pair<TokenType, size_t>(rule.output, 0));
    const std::vector< TokenType >& components = rule.components;
    size_t i = 0;
    for (size_t j = 0; j < components.size(); ++j) {
        if (components[j] == 0) {
            // etc
            j++;
            if (components[j] == 0) {
                throw std::runtime_error("What in Jesus' name is \"etc, etc\" supposed to mean?");
            }
            else if (components[j] == 1) {
                throw std::runtime_error("What in Jesus' name is \"etc, etc_not\" supposed to mean?");
            }
            while (i < n) {
                Tree< std::pair<Token, size_t> >* x = match(A + i, n - i, components[j]);
                if (x == nullptr) {
                    break;
                }
                rtn->add_child(x);
                i += x->value.second;
            }
        }
        else if (components[j] == 1) {
            // etc_not
            j++;
            if (components[j] == 0) {
                throw std::runtime_error("What in Jesus' name is \"etc_not, etc\" supposed to mean?");
            }
            else if (components[j] == 1) {
                throw std::runtime_error("What in Jesus' name is \"etc_not, etc_not\" supposed to mean?");
            }
            while (i < n) {
                Tree< std::pair<Token, size_t> >* x = match(A + i, n - i, components[j]);
                if (x != nullptr) {
                    delete x;// added
                    break;
                }
                i++;
            }
        }
        else {
            Tree< std::pair<Token, size_t> >* x = match(A + i, n - i, components[j]);
            if (x == nullptr) {
                return nullptr;
            }
            rtn->add_child(x);
            i += x->value.second;
        }
    }
    rtn->value.second = i;
    return rtn;
}

Tree< std::pair<Token, size_t> >* Parser::match(const Token* A, size_t n, const Token& value) {
    
    if (value.type < token_threshold) {
        if (value.type == A[0].type) {
            return new Tree< std::pair<Token, size_t> >( std::pair<Token, size_t>(value.type, 1));
        }
        else {
            return nullptr;
        }
    }
    
    for (size_t i = 0; i < rules.size(); ++i) {
        if (rules[i].output != value.type) {
            continue;
        }
        Tree< std::pair<Token, size_t> >* x = match(A, n, rules[i]);
        if (x != nullptr) {
            return x;
        }
    }
    return nullptr;
}

