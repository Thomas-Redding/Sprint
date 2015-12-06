//
//  main.cpp
//  Sprint
//
//  Created by Thomas Redding on 9/28/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "ResourcePath.hpp"

int main(int argc, const char * argv[]) {
	std::string line;
	std::string contents = "";
	std::string pathToDesktop = desktopPath();
	std::string pathToFile;
	if(pathToDesktop == "/Users/thomasredding/Desktop") {
		// Thomas
		pathToFile = pathToDesktop + "/test.txt";
	}
	else {
		// Morgan
		pathToFile = "/Users/mredding/Sprint/test.txt";
	}
	std::ifstream myfile(pathToFile);
	if(myfile.is_open()) {
		while(getline(myfile, line)) {
			contents += line;
			contents += "\n";
		}
		myfile.close();
	}
	
	Tokenizer tokenizer;
	std::vector<Token> tokenizedList = tokenizer.process(contents);
	
	for(int i=0; i<tokenizedList.size(); i++) {
		std::cout << "<" << tokenizer.tokenTypeToString(tokenizedList[i].type) << " : " << tokenizedList[i].str << " : " << tokenizedList[i].lineNum << " : " << tokenizedList[i].charNum << ">\n";
	}
	
    std::vector< Rule > rules =
    {
        Rule(bar, { etc, IDENTIFIER }),
        Rule(foo, { INDENT, bar, DEDENT }),
    };
    Rule program_rule(program, {etc, foo});
    
    Parser parser(rules);
    
    Tree< std::pair< Token, size_t> >* tree = parser.match(&tokenizedList[0], tokenizedList.size(), foo);

    std::cout << *tree << std::endl;
    
    return 0;
}
