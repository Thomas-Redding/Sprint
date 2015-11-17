//
//  Parser.cpp
//  Sprint
//
//  Created by Thomas Redding on 10/22/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "Parser.hpp"

Parser::Parser() {
	//
}

void Parser::parse(std::vector<Token> *tokens) {
	int index = 0;
	Tree<GrammarObject> *tree = parseHelper(tokens, &index, GENERAL);
	std::cout << tree->toString() << "\n";
	if(index != tokens->size()) {
		
		exit("Error #3 (" + std::to_string(index) + " / " + std::to_string(tokens->size()) + ")\n");
	}
}

Tree<GrammarObject> *Parser::parseHelper(std::vector<Token> *tokens, int *index, GrammarType state) {
	Tree<GrammarObject> *rtn = new Tree<GrammarObject>(GrammarObject(state));
	std::vector<GrammarObject> decomp = grammar.getRule(state, *tokens, *index);	
	for(int j=0; j<decomp.size(); j++) {
		if(decomp[j].type == TERMINAL) {
			if(decomp[j].token.type != tokens->at(*index).type) {
				exit("Error #1\n");
			}
			if(decomp[j].token.str != "" || tokens->at(*index).str != "" || decomp[j].token.str == tokens->at(*index).str) {
				// they are the same
				Tree<GrammarObject> *child = new Tree<GrammarObject>(GrammarObject(TERMINAL, tokens->at(*index)));
				rtn->addChild(child);
				(*index)++;
			}
			else {
				exit("Error #2\n");
			}
		}
		else {
			rtn->addChild(parseHelper(tokens, index, decomp[j].type));
		}
	}
	return rtn;
}

void Parser::exit(std::string message) {
	std::cout << message << "\n";
	_exit(1);
}
