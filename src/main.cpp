#include <iostream>
#include <unistd.h>
#include <map>
#include <stack>
#include <unordered_map>
#include <functional>
#include <chrono>

#include "../include/Compiler.hpp"
#include "../include/FindModifiedFiles.hpp"
#include "../include/Parser.hpp"
#include "../include/ParserVerifier.hpp"
#include "../include/Sweetener.hpp"
#include "../include/Tokenizer.hpp"
// #include "../include/ScopeVerifier.hpp"
#include "ParseRules.cpp"

bool is_int(TokenType type) {
	return type == KEYWORD_INT || type == KEYWORD_INT8 || type == KEYWORD_INT16 || type == KEYWORD_INT32 || type == KEYWORD_UINT || type == KEYWORD_UINT8 || type == KEYWORD_UINT16 || type == KEYWORD_UINT32 || type == KEYWORD_UINT || type == KEYWORD_BOOL || type == KEYWORD_CHAR;
}

int main(int argc, const char * argv[]) {
	auto timeStart = std::chrono::high_resolution_clock::now();

	if (argc != 2) {
		std::cout << "Error: need to pass in one argument" << std::endl;
		return 0;
	}

	std::vector<std::string> modifiedFiles = findModifiedFiles(argv[1]);

	std::cout << "modified files: " << modifiedFiles.size() << std::endl;

	// TODO: we'll use this list of modified files somehow...
	// TODO: after a successful compilation, modify 'last_hashes.txt' with the new hashes

	std::string fileLine;
	std::string contents = "";
	std::string pathToFile = argv[1];


	auto start_open_file = std::chrono::high_resolution_clock::now();
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
	auto end_open_file = std::chrono::high_resolution_clock::now();



	auto start_tokenize = std::chrono::high_resolution_clock::now();
	Tokenizer tokenizer;
	std::list<Token> tokenizedList = tokenizer.process(contents);
	auto end_tokenize = std::chrono::high_resolution_clock::now();

	// auto timeFindingClasses = std::chrono::high_resolution_clock::now();
	// syntatic sugar to switch a.b(c) to b(a,c)
	// if (!addFunctionSugar(tokenizedList)) {
	// 	return 0;
	// }
	// auto timeAsteriskPtr = std::chrono::high_resolution_clock::now();

	/*
	auto start_vectorization = std::chrono::high_resolution_clock::now();
	// http://stackoverflow.com/questions/5218713/one-liner-to-convert-from-listt-to-vectort
	std::vector<Token> tokenizedList{ std::begin(list), std::end(list) };
	auto end_vectorization = std::chrono::high_resolution_clock::now();
	*/

	// print out tokens
	// for (std::list<Token>::iterator it = tokenizedList.begin(); it != tokenizedList.end(); ++it)
	// 	std::cout << *it << "\n";

	auto start_parse_rules = std::chrono::high_resolution_clock::now();
	std::vector<bool> leftToRight;
	std::vector<ParseRule> listOfRules;
	addParseRules(leftToRight, listOfRules);
	Parser foo(leftToRight, listOfRules);
	auto end_parse_rules = std::chrono::high_resolution_clock::now();

	auto start_parse = std::chrono::high_resolution_clock::now();
	ParseNode* parse_tree = foo.getParseTree(&tokenizedList);
	auto end_parse = std::chrono::high_resolution_clock::now();

	std::cout << "\n\n\n";
	parse_tree->print();
	std::cout << "\n\nhash: " << parse_tree->to_hash() << "\n\n";

	// auto start_verification = std::chrono::high_resolution_clock::now();
	// ParserVerifier pv(&foo);
	// pv.verify(parse_tree);
	auto end_verification = std::chrono::high_resolution_clock::now();

	// auto start_duplication = std::chrono::high_resolution_clock::now();
	// ScopeVerifier scoper;
	// scoper.verify(parse_tree);
	// auto end_duplication = std::chrono::high_resolution_clock::now();

	// Compiler comp;
	// comp.compile(parse_tree);

	// std::cout << "Read Files          : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_open_file - start_open_file).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(end_open_file - start_open_file).count() / tokenizedList.size() << " ns per token)" << std::endl;
	// std::cout << "Tokenizing          : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_tokenize - start_tokenize).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(end_tokenize - start_tokenize).count() / tokenizedList.size() << " ns per token)" << std::endl;
	// std::cout << "List->Vect : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_vectorization - start_vectorization).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(end_vectorization - start_vectorization).count() / tokenizedList.size() << " ns per token)\n";
	// std::cout << "Parse Rules         : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_parse_rules - start_parse_rules).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(end_parse_rules - start_parse_rules).count() / tokenizedList.size() << " ns per token)" << std::endl;
	// std::cout << "Parser              : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_parse - start_parse).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(end_parse - start_parse).count() / tokenizedList.size() << " ns per token)\n";
	// std::cout << "Parse Ver.          : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_verification - start_verification).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(end_verification - start_verification).count() / tokenizedList.size() << " ns per token)" << std::endl;;
	// std::cout << "Template Duplication: " << std::chrono::duration_cast<std::chrono::nanoseconds>(start_duplication - end_duplication).count() / 1000 << " µs" << std::endl;
	std::cout << "Total Time          : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_verification - start_open_file).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(end_verification - start_open_file).count() / tokenizedList.size() << " ns per token)" << std::endl;;
	return 0;
}