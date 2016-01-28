//
//  ParseTree.hpp
//  Sprint
//
//  Created by Morgan Redding on 12/23/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef ParseTree_h
#define ParseTree_h

#include "Token.hpp"
#include <vector>

struct ParseTree {
    ParseTree(const std::pair<const Token*, size_t>& value) : value(value), parent(nullptr), _number_of_leaves(0), children(_children) {};
    ParseTree(ParseTree* parent) : parent(parent), _number_of_leaves(0), children(_children) {};
    ParseTree() : children(_children) {
        delete value.first;
        for (size_t i = 0; i < _children.size(); ++i) {
            delete _children[i];
        }
    }
    void add_child(ParseTree* child) {
        child->parent = this;
        _children.push_back(child);
        _number_of_leaves++;
        if (_children.size() > 1) {
            ParseTree* p = parent;
            while (p != nullptr) {
                p->_number_of_leaves++;
                p = p->parent;
            }
        }
    }
    void add_child(const std::pair<const Token*, size_t>& value) {
        add_child(new ParseTree(value));
    }
    size_t number_of_children() const {
        return _children.size();
    }
    size_t number_of_leaves() const {
        return _number_of_leaves;
    }
    ParseTree* operator[](size_t i) {
        return _children[i];
    }
    std::pair<const Token*, size_t> value;
    
    friend std::ostream& operator<<(std::ostream& o, const ParseTree& t) {
        return o << t.toString();
    }
    
    std::string toString(int max_print_depth = 12, int depth = 0) const {
        std::string rtn = "";
        for (int i = 0; i < depth; ++i) {
            rtn += "    ";
        }
        rtn += valueToString();
        if (children.size() == 0) {
            return rtn;
        }
        if (depth < max_print_depth) {
            for (int i = 0; i < children.size(); ++i) {
                rtn += '\n';
                rtn += children[i]->toString(max_print_depth, depth + 1);
            }
        }
        return rtn;
    }
    
    std::string valueToString() const {
        return "  " + Token::tokenTypeToString(value.first->type) + "  ";
    }
    
    const std::vector<ParseTree*>& children;
    
private:
    
    int width() const {
        int x = int(valueToString().size());
        int y = 0;
        for (int i = 0; i < children.size(); ++i) {
            y += children[i]->width();
        }
        return x > y ? x : y;
    }
    
    size_t _number_of_leaves;
    std::vector<ParseTree*> _children;
    ParseTree* parent;
};


#endif /* ParseTree_h */
