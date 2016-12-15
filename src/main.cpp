#include <iostream>
#include <unistd.h>
#include <map>
#include <stack>
#include <unordered_map>
#include <functional>
#include <chrono>

#include "../include/Tokenizer.hpp"
#include "../include/Sweetener.hpp"
#include "../include/Parser.hpp"
#include "../include/FindModifiedFiles.hpp"
#include "../include/ParserVerifier.hpp"
#include "ParseRules.cpp"

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

	auto timeOpenedFile = std::chrono::high_resolution_clock::now();
	
	Tokenizer tokenizer;
	std::list<Token> list = tokenizer.process(contents);

	auto timeTokenized = std::chrono::high_resolution_clock::now();

	auto timeFindingClasses = std::chrono::high_resolution_clock::now();

	// syntatic sugar to switch a.b(c) to b(a,c)
	// if (!addFunctionSugar(tokenizedList)) {
	// 	return 0;
	// }

	auto timeAsteriskPtr = std::chrono::high_resolution_clock::now();

	// http://stackoverflow.com/questions/5218713/one-liner-to-convert-from-listt-to-vectort
	std::vector<Token> tokenizedList{ std::begin(list), std::end(list) };

	auto timeListToVector = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < tokenizedList.size(); ++i)
		std::cout << tokenizedList[i] << "\n";

	std::vector<bool> leftToRight;
	std::vector<ParseRule> listOfRules;
	addParseRules(leftToRight, listOfRules);

	Parser foo(leftToRight, listOfRules);

	auto timeParseRules = std::chrono::high_resolution_clock::now();

	ParseNode* bar = foo.getParseTree(&tokenizedList[0], tokenizedList.size());

	// timeStart, timeOpenedFile, timeTokenized, timeListToVector, timeAsteriskPtr, timeParsed
	auto timeParsed = std::chrono::high_resolution_clock::now();

	std::cout << "\n\n\n";
	bar->print();

	ParserVerifier pv(&foo);
	pv.verify(bar);

	std::cout << "Reading File(s): " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeOpenedFile - timeStart).count() / 1000 << " µs\n";
	std::cout << "Tokenizing: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeTokenized - timeOpenedFile).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(timeTokenized - timeOpenedFile).count() / list.size() << " ns per token)\n";
	std::cout << "Finding Classes: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeFindingClasses - timeTokenized).count() / 1000 << " µs\n";
	std::cout << "List-to-Vector: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeListToVector - timeFindingClasses).count() / 1000 << " µs\n";
	std::cout << "Asterisk-Ptr: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeAsteriskPtr - timeListToVector).count() / 1000 << " µs\n";
	std::cout << "Parse Rules: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeParseRules - timeAsteriskPtr).count() / 1000 << " µs\n";
	std::cout << "Parsing: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeParsed - timeParseRules).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(timeParsed - timeParseRules).count() / list.size() << " ns per token)\n";
	std::cout << "Tokens: " << list.size() << "\n";

	return 0;
}