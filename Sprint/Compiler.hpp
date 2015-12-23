//
//  Compiler.hpp
//  Sprint
//
//  Created by Thomas Redding on 12/23/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Compiler_hpp
#define Compiler_hpp

#include "Tree.h"
#include "Token.hpp"

class Compiler {
public:
	void compile(Tree< std::pair< Token, size_t> >* tree);
};

#endif /* Compiler_hpp */
