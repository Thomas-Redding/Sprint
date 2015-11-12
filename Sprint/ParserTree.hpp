//
//  ParserTree.hpp
//  Sprint
//
//  Created by Thomas Redding on 11/6/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef ParserTree_cpp
#define ParserTree_cpp

#include <stdio.h>
#include <vector>
#include "Token.hpp"

enum ParserState {
	NONE, CLASS
};

struct ParserToken {
	Token token;
	ParserState state;
};

class ParserTree {
public:
	void addChild(ParserTree *tree) {
		children.push_back(tree);
	};
	ParserTree* copy() {
		ParserTree *rtn = new ParserTree();
		rtn->setParserToken(rtn->getParserToken());
		for(int i=0; i<children.size(); i++) {
			rtn->addChild(children[i]->copy());
		}
		return rtn;
	}
	ParserTree* getChild(int i) {
		return children[i];
	}
	unsigned long getChildrenLength() {
		return children.size();
	}
	void setParserToken(ParserToken newState) {
		state = newState;
	}
	ParserToken getParserToken() {
		return state;
	}
	void cleanUp() {
		for(int i=0; i<children.size(); i++) {
			children[i]->cleanUp();
			delete children[i];
		}
	}
private:
	ParserToken state;
	std::vector<ParserTree*> children;
	ParserTree* parent;
};


#endif /* ParserTree_cpp */
