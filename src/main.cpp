#include <iostream>
#include <fstream>
#include <unistd.h>
#include <map>
#include "../include/Tokenizer.hpp"
#include "../include/Parser.hpp"
#include "../include/Sweetener.hpp"
#include "../include/ThomasParser.hpp"
#include <functional>


int main(int argc, const char * argv[]) {
	if (argc != 2) {
		std::cout << "Error: need to pass in one argument" << std::endl;
        return 0;
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
    else {
        std::cout << "Error: could not open file" << std::endl;
        return 0;
    }
    
    Tokenizer tokenizer;
    std::list<Token> list = tokenizer.process(contents);

    // http://stackoverflow.com/questions/5218713/one-liner-to-convert-from-listt-to-vectort
    std::vector<Token> tokenizedList{ std::begin(list), std::end(list) };

    // syntatic sugar to switch a.b(c) to b(a,c)
    if (!addFunctionSugar(tokenizedList)) {
        return 0;
    }

    if (tokenizedList.size() > 0) {
        std::cout << tokenizedList[0].toString();
        for (int i = 1; i < tokenizedList.size(); ++i) {
            if (tokenizedList[i].lineNum != tokenizedList[i - 1].lineNum) {
                std::cout << std::endl << std::endl << tokenizedList[i].toString();
            }
            else {
                std::cout << ", " << tokenizedList[i].toString();
            }
        }
        std::cout << std::endl;
    }

    ParseNode* tree = Parser::getParseTree(&tokenizedList[0], tokenizedList.size());
    std::cout << std::endl << "================================================" << std::endl << "================================================" << std::endl << std::endl;
    std::cout << (*tree) << std::endl << std::endl;
    // std::cout << Token::toString(tree->type) << ": " << tree->children.size() << std::endl;
    // std::cout << Token::toString(tree->children[0]->type) << ": " << tree->children[0]->children.size() << std::endl;
    // std::cout << Token::toString(tree->children[1]->type) << ": " << tree->children[1]->children.size() << std::endl;

    ThomasParser foo;

    return 0;
}