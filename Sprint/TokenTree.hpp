//
//  TokenTree.hpp
//  Sprint
//
//  Created by Thomas Redding on 10/22/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef TokenTree_cpp
#define TokenTree_cpp

#include <vector>
#include "Token.hpp"

class TokenTree {
public:
	Token token;
	std::vector<TokenTree*> children;
	TokenTree* parent;
	void addChild(Token token) {
		children.push_back(new TokenTree);
		children[children.size()-1]->token = token;
	};
	void cleanUp() {
		for(int i=0; i<children.size(); i++) {
			children[i]->cleanUp();
			delete children[i];
		}
	}
};

#endif /* TokenTree_cpp */
