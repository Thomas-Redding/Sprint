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
        
        o << "(" << *t.value.first << ", " << t.value.second << ")" << std::endl;
        if (t.number_of_children() == 0) {
            return o;
        }
        o << "children:" << std::endl;
        o << "(" << *(t._children[0]->value.first) << ", " << t._children[0]->value.second << ")";
        for (size_t i = 1; i < t._children.size(); ++i) {
            o << "      (" << *t._children[i]->value.first << ", " << t._children[i]->value.second << ")";
        }
        o << std::endl;
        o << "grand children:" << std::endl;
        for (size_t i = 0; i < t._children.size(); ++i) {
            if (i != 0) {
                o << " | ";
            }
            if (t._children[i]->number_of_children() == 0) {
                continue;
            }
            o << "(" << *t._children[i]->_children[0]->value.first << ", " << t._children[i]->_children[0]->value.second << ")";
            for (size_t j = 1; j < t._children[i]->_children.size(); ++j) {
                o << "      (" << *t._children[i]->_children[j]->value.first << ", " << t._children[i]->_children[j]->value.second << ")";
            }
        }
        return o;
    }
    const std::vector<ParseTree*>& children;
private:
    
    size_t _number_of_leaves;
    std::vector<ParseTree*> _children;
    ParseTree* parent;
};


#endif /* ParseTree_h */
