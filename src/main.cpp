#include <iostream>
#include <fstream>
#include <unistd.h>
#include "../include/Tokenizer.hpp"
#include "../include/Sweetener.hpp"

int main(int argc, const char * argv[]) {

	if (argc != 2) {
		std::cout << "Need to pass in one argument" << std::endl;
	}
    
    std::string fileLine;
    std::string contents = "";
    std::string pathToFile = argv[1];
    
    std::ifstream myfile(pathToFile);
    if (myfile.is_open()) {
        while(getline(myfile, fileLine)) {
            contents += fileLine;
            contents += "\n";
        }
        myfile.close();
    }
    
    Tokenizer tokenizer;
    std::vector<Token> tokenizedList = tokenizer.process(contents);
    
    // syntatic sugar to switch a.b(c) to b(a,c)
    if (!addFunctionSugar(tokenizedList)) {
        return 0;
    }
    
    std::cout << "NUMBER OF TOKENS: " << tokenizedList.size() << std::endl << std::endl;
    for (size_t i = 0; i < tokenizedList.size(); ++i) {
        std::cout << i << ": <" << Token::tokenTypeToString(tokenizedList[i].type) << "    " << tokenizedList[i].str << "    >\n";
    }

    return 0;
}
