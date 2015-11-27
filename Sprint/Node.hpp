//
//  Node.hpp
//  Sprint
//
//  Created by Morgan Redding on 11/23/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Node_h
#define Node_h

#include <vector>
#include "Token.hpp"

struct Node {
    Node() : is_token(false) {};
    Node(const Token& t) : is_token(true) {};
    Node(const Node& n);
    ~Node() {
        for (size_t i = 0; i < children.size(); i++) {
            delete children[i];
        }
    }
    void add_child(Node* c) {
        if (is_token) {
            throw std::runtime_error("Cannot add child to a terminal node/token");
        }
        children.push_back(c);
    }
    const Node* operator[](size_t i) const {
        if (i >= children.size()) {
            throw std::runtime_error("Index out of bounds");
        }
        return children[i];
    }
    inline size_t length() const {
        return children.size();
    }
private:
    std::vector<Node*> children;
    const bool is_token;
};


#endif /* Node_h */
