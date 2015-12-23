//
//  Tree.h
//  Sprint
//
//  Created by Morgan Redding on 11/30/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include <vector>
#include <ostream>

#ifndef Tree_h
#define Tree_h

template<class F>
struct Tree {
    Tree(const F& value) : value(value), parent(nullptr), _number_of_leaves(0) {};
    Tree(Tree* parent) : parent(parent), _number_of_leaves(0) {};
    ~Tree() {
        for (size_t i = 0; i < _children.size(); ++i) {
            delete _children[i];
        }
    }
    void add_child(Tree* child) {
        child->parent = this;
        _children.push_back(child);
        _number_of_leaves++;
        if (_children.size() > 1) {
            Tree* p = parent;
            while (p != nullptr) {
                p->_number_of_leaves++;
                p = p->parent;
            }
        }
    }
    void add_child(const F& value) {
        add_child(new Tree(value));
    }
    size_t number_of_children() const {
        return _children.size();
    }
    size_t number_of_leaves() const {
        return _number_of_leaves;
    }
    Tree* operator[](size_t i) {
        return _children[i];
    }
    F value;
    friend std::ostream& operator<<(std::ostream& o, const Tree& t) {
        o << t.value << std::endl;
        if (t.number_of_children() == 0) {
            return o;
        }
        o << t._children[0]->value;
        for (size_t i = 1; i < t._children.size(); ++i) {
            o << ", " << t._children[i]->value;
        }
        o << std::endl;
        for (size_t i = 0; i < t._children.size(); ++i) {
            if (i != 0) {
                o << " | ";
            }
            if (t._children[i]->number_of_children() == 0) {
                continue;
            }
            o << t._children[i]->_children[0]->value;
            for (size_t j = 1; j < t._children[i]->_children.size(); ++j) {
                o << ", " << t._children[i]->_children[j]->value;
            }
        }
        return o;
    }
private:
    
    size_t _number_of_leaves;
    std::vector<Tree*> _children;
    Tree* parent;
};

#endif /* Tree_h */
