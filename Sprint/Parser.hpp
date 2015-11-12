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
#include "ParserTree.hpp"

class Parser {
public:
	Parser();
	void parse(std::vector<Token> *tokens);
private:
	int index;
	void exit(std::string message);
	ParserTree* parseHelper(std::vector<Token> *tokens, ParserState parserState);
};

#endif /* Parser_cpp */
