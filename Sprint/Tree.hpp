//
//  Tree.hpp
//  Sprint
//
//  Created by Thomas Redding on 11/12/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Tree_cpp
#define Tree_cpp

#include <stdio.h>
#include <vector>
#include "Token.hpp"

template <class V>
class Tree {
public:
	Tree(V v) : value(v){
		value = v;
	};
	void addChild(Tree<V> *tree) {
		children.push_back(tree);
	};
	Tree<V>* copy() {
		Tree<V> *rtn = new Tree<V>();
		rtn->setParserToken(rtn->getParserToken());
		for(int i=0; i<children.size(); i++) {
			rtn->addChild(children[i]->copy());
		}
		return rtn;
	}
	Tree<V>* getChild(int i) {
		return children[i];
	}
	unsigned long getChildrenLength() {
		return children.size();
	}
	void setValue(V newState) {
		value = newState;
	}
	V getValue() {
		return value;
	}
	void cleanUp() {
		for(int i=0; i<children.size(); i++) {
			children[i]->cleanUp();
			delete children[i];
		}
		delete children;
	}
private:
	V value;
	std::vector<Tree<V>*> children;
	Tree<V>* parent;
};

#endif /* Tree_cpp */
