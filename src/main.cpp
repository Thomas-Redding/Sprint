#include <iostream>
#include <fstream>
#include <unistd.h>
#include <map>
#include "../include/Tokenizer.hpp"
#include "../include/ParseNode.hpp"
#include "../include/Parser.hpp"
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
    std::list<Token> list = tokenizer.process(contents);

    
    // http://stackoverflow.com/questions/5218713/one-liner-to-convert-from-listt-to-vectort
    std::vector<Token> tokenizedList{ std::begin(list), std::end(list) };

    if (tokenizedList.size() > 0) {
        std::cout << tokenizedList[0].toString();
        for (int i = 1; i < tokenizedList.size(); ++i) {
            std::cout << ", " << tokenizedList[i].toString();
        }
        std::cout << std::endl;
    }

    // syntatic sugar to switch a.b(c) to b(a,c)
    if (!addFunctionSugar(tokenizedList)) {
        return 0;
    }

    ParseNode* tree = Parser::getParseTree(&tokenizedList[0], tokenizedList.size());

    return 0;
}
