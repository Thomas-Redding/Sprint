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
	std::ifstream myfile("/Users/mredding/Desktop/abab.txt");
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
	
	Parser parser;
	parser.parse(tokenizedList);
	
    return 0;
}
