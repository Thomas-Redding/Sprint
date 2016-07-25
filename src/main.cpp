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

	std::cout << "\n\n\n";
	std::vector<bool> leftToRight = {
		true, false, false
	};
	std::vector<ThomasParseRule> listOfRules;
	listOfRules.push_back(ThomasParseRule(10, general, {T_INTEGER_LITERAL}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_FLOAT_LITERAL}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_IDENTIFIER}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_STRING_LITERAL}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_INT}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_INT8}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_INT16}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_INT32}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_INT64}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_UINT}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_UINT8}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_UINT16}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_UINT32}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_UINT64}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_FLOAT}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_DOUBLE}, value));
	listOfRules.push_back(ThomasParseRule(10, general, {T_STRING_LITERAL}, value));

	listOfRules.push_back(ThomasParseRule(20, general, {value, T_ASTERISK, value}, mult_clause));
	listOfRules.push_back(ThomasParseRule(20, general, {value, T_ASTERISK, mult_clause}, mult_clause));
	listOfRules.push_back(ThomasParseRule(20, general, {value, T_ASTERISK, parenthesis_block}, mult_clause));
	listOfRules.push_back(ThomasParseRule(20, general, {mult_clause, T_ASTERISK, mult_clause}, mult_clause));
	listOfRules.push_back(ThomasParseRule(20, general, {parenthesis_block, T_ASTERISK, mult_clause}, mult_clause));
	listOfRules.push_back(ThomasParseRule(20, general, {parenthesis_block, T_ASTERISK, parenthesis_block}, mult_clause));

	listOfRules.push_back(ThomasParseRule(30, general, {value, T_PLUS, value}, add_clause));
	listOfRules.push_back(ThomasParseRule(30, general, {value, T_PLUS, mult_clause}, add_clause));
	listOfRules.push_back(ThomasParseRule(30, general, {value, T_PLUS, add_clause}, add_clause));
	listOfRules.push_back(ThomasParseRule(30, general, {value, T_PLUS, parenthesis_block}, add_clause));
	listOfRules.push_back(ThomasParseRule(30, general, {mult_clause, T_PLUS, mult_clause}, add_clause));
	listOfRules.push_back(ThomasParseRule(30, general, {mult_clause, T_PLUS, add_clause}, add_clause));
	listOfRules.push_back(ThomasParseRule(30, general, {mult_clause, T_PLUS, parenthesis_block}, add_clause));
	listOfRules.push_back(ThomasParseRule(30, general, {add_clause, T_PLUS, add_clause}, add_clause));
	listOfRules.push_back(ThomasParseRule(30, general, {add_clause, T_PLUS, parenthesis_block}, add_clause));
	listOfRules.push_back(ThomasParseRule(30, general, {parenthesis_block, T_PLUS, parenthesis_block}, add_clause));

	ThomasParser foo(leftToRight, listOfRules);
	ThomasNode* bar = foo.getParseTree(&tokenizedList[0], tokenizedList.size());
	bar->print();
	std::cout << "\n\n\n";
	return 0;
	
	ParseNode* tree = Parser::getParseTree(&tokenizedList[0], tokenizedList.size());
	std::cout << std::endl << "================================================" << std::endl << "================================================" << std::endl << std::endl;
	std::cout << (*tree) << std::endl << std::endl;
	// std::cout << Token::toString(tree->type) << ": " << tree->children.size() << std::endl;
	// std::cout << Token::toString(tree->children[0]->type) << ": " << tree->children[0]->children.size() << std::endl;
	// std::cout << Token::toString(tree->children[1]->type) << ": " << tree->children[1]->children.size() << std::endl;

	return 0;
}