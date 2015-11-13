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
#include "Tree.hpp"
#include "Grammar.hpp"
#include "Tree.hpp"

class Parser {
public:
	Parser();
	void parse(std::vector<Token> *tokens);
private:
	int index;
	Grammar grammar;
	Tree<GrammarObject> *parseHelper(std::vector<Token> *tokens, int *index, GrammarType state);
	void exit(std::string message);
};

#endif /* Parser_cpp */
