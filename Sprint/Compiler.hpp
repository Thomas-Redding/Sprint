//
//  Compiler.hpp
//  Sprint
//
//  Created by Thomas Redding on 12/23/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Compiler_hpp
#define Compiler_hpp

#include <sys/stat.h>
#include <fstream>
#include "ParseTree.hpp"
#include "Token.hpp"

class Compiler {
public:
	void compile(ParseTree* tree, std::string path);
private:
	std::string readFile(std::string path);
	void writeToFile(std::string path, std::string newContents);
};

#endif /* Compiler_hpp */
