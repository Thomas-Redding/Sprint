//
//  Compiler.cpp
//  Sprint
//
//  Created by Thomas Redding on 12/23/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "../include/Compiler.hpp"

void Compiler::compile(ParseTree* tree, std::string path) {
	std::string newFolderPath = path + "c++ code";
	mkdir(newFolderPath.c_str(), 0777);
	writeToFile(newFolderPath + "/test.txt", "Hello World!");
}

std::string Compiler::readFile(std::string path) {
	std::string line, contents;
	std::ifstream myfile(path);
	if(myfile.is_open()) {
		while(getline(myfile, line)) {
			contents += line;
			contents += "\n";
		}
		myfile.close();
	}
	return contents;
}

void Compiler::writeToFile(std::string path, std::string newContents) {
	std::ofstream myfile(path);
	if(myfile.is_open()) {
		myfile << newContents;
		myfile.close();
	}
}
