#include <iostream>
#include <fstream>
#include <unistd.h>
#include <map>
#include <stack>
#include <unordered_map>
#include <functional>
#include <chrono>

#include "../include/Tokenizer.hpp"
#include "../include/Sweetener.hpp"
#include "../include/ThomasParser.hpp"
#include "../include/Class.hpp"

void convert_LESSTHANs_to_TEMPLATE_OPENs_and_asterisks_to_PTR(std::list<Token>& list) {

	std::unordered_map<std::string, Class> currentClasses;
	std::stack<uint64_t> numberOfClassesAddedAtCurrentDepth;
	std::stack<uint64_t> numberOfClassesOverwritten;
	std::stack<std::string> classesAdded;
	std::stack<Class> classesOverwritten;

	numberOfClassesAddedAtCurrentDepth.push(0);
	numberOfClassesOverwritten.push(0);
	uint64_t template_depth = 0;
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (it->type == OPEN_CURLY_BRACE) {
			numberOfClassesAddedAtCurrentDepth.push(0);
			numberOfClassesOverwritten.push(0);
		}
		else if (it->type == CLOSE_CURLY_BRACE) {

			// remove added classes
			for (uint64_t i = 0; i < numberOfClassesAddedAtCurrentDepth.top(); ++i) {
				currentClasses.erase(classesAdded.top());
				classesAdded.pop();
			}
			numberOfClassesAddedAtCurrentDepth.pop();

			// re-overwrite overwritten classes
			for (uint64_t i = 0; i < numberOfClassesOverwritten.top(); ++i) {
				currentClasses[classesOverwritten.top().base_name] = classesOverwritten.top();
				classesOverwritten.pop();
			}
			numberOfClassesOverwritten.pop();
		}
		else if (it->type == KEYWORD_CLASS) {
			Class c(list, it);
			if (currentClasses.count(c.base_name) > 0) {
				classesOverwritten.push(currentClasses.find(c.base_name)->second);
				currentClasses[c.base_name] = c;
				numberOfClassesOverwritten.top()++;
			}
			else {
				currentClasses.insert(std::pair<std::string, Class>(c.base_name, c));
				numberOfClassesAddedAtCurrentDepth.top()++;
				classesAdded.push(c.base_name);
			}
		}
		else if (it->type == LESS_THAN) {
			--it;
			if (currentClasses.count(it->str) > 0) {
				++it;
				it->type = OPEN_TEMPLATE;
				++template_depth;
			}
			++it;
		}
		else if (it->type == GREATER_THAN) {
			if (template_depth > 0) {
				it->type = CLOSE_TEMPLATE;
				--template_depth;
			}
		}
	}

	// TODO: something like "convert_asterisk_to_PTR(list, classes);"
}

void convert_asterisk_to_PTR(std::list<Token>& list, const std::unordered_map<std::string, Class>& classes) {
	for (auto it = ++list.begin(); it != list.end(); ++it) {
		if (it->type == CLOSE_PARENTHESIS || it->type == CLOSE_BRACKET) {
			continue;
		}
		Token t = *(it++);
		if (it == list.end()) {
			break;
		}
		if (it->type != ASTERISK) {
			continue;
		}
		if (t.isPrimitive()) {
			it->type = PTR;
			continue;
		}
		if (classes.count(t.str) != 0) {
			it->type = PTR;
			continue;
		}
		if (t.type != GREATER_THAN) {
			continue;
		}

		// store for later
		auto currentIterator = it;
		uint64_t lineNum = it->lineNum;

		uint64_t depth = 1;
		--it;
		while (--it != list.begin() && depth > 0) {
			if (it->type == GREATER_THAN) ++depth;
			else if (it->type == LESS_THAN) --depth;
		}
		if (it == list.begin()) {
			throw std::runtime_error("Error determining type of templated class on line " + std::to_string(lineNum));
		}
		--it;
		if (it == list.begin()) {
			throw std::runtime_error("Error determining type of templated class on line " + std::to_string(lineNum));
		}
		if (classes.count(it->str) != 0) {
			currentIterator->type = PTR;
		}
		it = currentIterator;
	}
}

int main(int argc, const char * argv[]) {

	auto timeStart = std::chrono::high_resolution_clock::now();

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

	auto timeOpenedFile = std::chrono::high_resolution_clock::now();
	
	Tokenizer tokenizer;
	std::list<Token> list = tokenizer.process(contents);

	auto timeTokenized = std::chrono::high_resolution_clock::now();

	convert_LESSTHANs_to_TEMPLATE_OPENs_and_asterisks_to_PTR(list);

	auto timeFindingClasses = std::chrono::high_resolution_clock::now();

	// syntatic sugar to switch a.b(c) to b(a,c)
	// if (!addFunctionSugar(tokenizedList)) {
	// 	return 0;
	// }

	auto timeAsteriskPtr = std::chrono::high_resolution_clock::now();

	// http://stackoverflow.com/questions/5218713/one-liner-to-convert-from-listt-to-vectort
	std::vector<Token> tokenizedList{ std::begin(list), std::end(list) };

	auto timeListToVector = std::chrono::high_resolution_clock::now();

	for (auto it = list.begin(); it != list.end(); ++it) {
		std::cout << *it << "\n";
	}


	// if (tokenizedList.size() > 0) {
	// 	std::cout << tokenizedList[0].toString();
	// 	for (int i = 1; i < tokenizedList.size(); ++i) {
	// 		if (tokenizedList[i].lineNum != tokenizedList[i - 1].lineNum) {
	// 			std::cout << std::endl << std::endl << tokenizedList[i].toString();
	// 		}
	// 		else {
	// 			std::cout << ", " << tokenizedList[i].toString();
	// 		}
	// 	}
	// 	std::cout << std::endl;
	// }

	// int y = true ? 5 : 3;
	// std::cout << y;

	// std::cout << "\n\n\n";
	// std::vector<bool> leftToRight = {
	// 	true, false, true, true, true, true, true, true, true, true, false, true, false
	// };
	// std::vector<ThomasParseRule> listOfRules;
	// // todo: templates
	// // listOfRules.push_back(ThomasParseRule(-1, general, {unary_value, T_PLUS_PLUS}, unary_clause));												// <T>

	// listOfRules.push_back(ThomasParseRule(0, general, {unary1_value, T_PLUS_PLUS}, unary1_clause));													// x++
	// listOfRules.push_back(ThomasParseRule(0, general, {unary1_value, T_MINUS_MINUS}, unary1_clause));													// x--
	// listOfRules.push_back(ThomasParseRule(0, general, {unary1_value, parenthesis_block}, unary1_clause));												// x(y)
	// listOfRules.push_back(ThomasParseRule(0, general, {unary1_value, bracket_block}, unary1_clause));													// x[y]
	// listOfRules.push_back(ThomasParseRule(0, general, {unary1_value, T_PERIOD}, unary1_clause));														// x.y

	// listOfRules.push_back(ThomasParseRule(10, general, {T_POSITIVE, unary2_value}, unary2_clause));													// +x
	// listOfRules.push_back(ThomasParseRule(10, general, {T_NEGATIVE, unary2_value}, unary2_clause));													// -x
	// listOfRules.push_back(ThomasParseRule(10, general, {T_PLUS_PLUS, unary2_value}, unary2_clause));													// ++x
	// listOfRules.push_back(ThomasParseRule(10, general, {T_MINUS_MINUS, unary2_value}, unary2_clause));												// --x
	// listOfRules.push_back(ThomasParseRule(10, general, {T_TILDE, unary2_value}, unary2_clause));														// ~x
	// listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_NOT, unary2_value}, unary2_clause));												// not x
	// listOfRules.push_back(ThomasParseRule(10, general, {T_PTR, unary2_value}, unary2_clause));														// *x
	// listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_NEW, unary2_value}, unary2_clause));												// new x

	// listOfRules.push_back(ThomasParseRule(20, general, {mult_value, T_ASTERISK, mult_value}, mult_clause));											// x * y
	// listOfRules.push_back(ThomasParseRule(20, general, {mult_value, T_SLASH, mult_value}, mult_clause));											// x / y
	// listOfRules.push_back(ThomasParseRule(20, general, {mult_value, T_PERCENT, mult_value}, mult_clause));											// x % y

	// listOfRules.push_back(ThomasParseRule(30, general, {plus_value, T_PLUS, plus_value}, plus_clause));												// x + y
	// listOfRules.push_back(ThomasParseRule(30, general, {plus_value, T_MINUS, plus_value}, plus_clause));											// x - y

	// listOfRules.push_back(ThomasParseRule(40, general, {shift_value, T_SHIFT_LEFT, shift_value}, shift_clause));									// x << y
	// listOfRules.push_back(ThomasParseRule(40, general, {shift_value, T_SHIFT_RIGHT, shift_value}, shift_clause));									// x >> y

	// listOfRules.push_back(ThomasParseRule(50, general, {inequality_value, T_LESS_THAN, inequality_value}, inequality_clause));						// x < y
	// listOfRules.push_back(ThomasParseRule(50, general, {inequality_value, T_GREATER_THAN, inequality_value}, inequality_clause));					// x > y
	// listOfRules.push_back(ThomasParseRule(50, general, {inequality_value, T_LESS_THAN_EQUALS, inequality_value}, inequality_clause));				// x <= y
	// listOfRules.push_back(ThomasParseRule(50, general, {inequality_value, T_GREATER_THAN_EQUALS, inequality_value}, inequality_clause));			// x >= y

	// listOfRules.push_back(ThomasParseRule(60, general, {equality_value, T_EQUAL_EQUALS, equality_value}, equality_clause));							// x == y
	// listOfRules.push_back(ThomasParseRule(60, general, {equality_value, T_EXCLAMATION_POINT_EQUALS, equality_value}, equality_clause));				// x != y
	// listOfRules.push_back(ThomasParseRule(60, general, {equality_value, T_EQUAL_EQUAL_EQUALS, equality_value}, equality_clause));					// x === y
	// listOfRules.push_back(ThomasParseRule(60, general, {equality_value, T_EXCLAMATION_POINT_EQUAL_EQUALS, equality_value}, equality_clause));		// x !== y

	// listOfRules.push_back(ThomasParseRule(70, general, {bitwise_and_value, T_KEYWORD_AND, bitwise_and_value}, bitwise_and_clause));					// x & y

	// listOfRules.push_back(ThomasParseRule(80, general, {bitwise_xor_value, T_KEYWORD_XOR, bitwise_xor_value}, bitwise_xor_clause));					// x ^ y

	// listOfRules.push_back(ThomasParseRule(90, general, {bitwise_or_value, T_KEYWORD_OR, bitwise_or_value}, bitwise_or_clause));						// x | y

	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_QUESTION_MARK, setting_value, T_COLON, setting_value}, ternary_clause));	// x ? y : z
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_EQUALS, setting_value}, setting_clause));									// x = y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_PLUS_EQUALS, setting_value}, setting_clause));							// x += y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_MINUS_EQUALS, setting_value}, setting_clause));							// x -= y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_ASTERISK_EQUALS, setting_value}, setting_clause));						// x *= y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_SLASH_EQUALS, setting_value}, setting_clause));							// x /= y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_PERCENT_EQUALS, setting_value}, setting_clause));							// x %= y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_AMPERSAND_EQUALS, setting_value}, setting_clause));						// x &= y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_CARROT_EQUALS, setting_value}, setting_clause));							// x ^= y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_VERTICAL_BAR_EQUALS, setting_value}, setting_clause));					// x |= y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_COLON_EQUALS, setting_value}, setting_clause));							// x := y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_SHIFT_LEFT_EQUALS, setting_value}, setting_clause));						// x <<= y
	// listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_SHIFT_RIGHT_EQUALS, setting_value}, setting_clause));						// x >>= y

	// listOfRules.push_back(ThomasParseRule(110, general, {comma_value, T_COMMA, comma_value}, comma_clause));										// x, y
	// listOfRules.push_back(ThomasParseRule(110, general, {comma_value, T_SEMI_COLON}, statement));													// x;

	// listOfRules.push_back(ThomasParseRule(120, general, {statement, statement}, statements));														// x y

	// // listOfRules.push_back(ThomasParseRule(120, general, {unary_clause, statement}, statements));														// x y

	// ThomasParser foo(leftToRight, listOfRules);

	// auto timeParseRules = std::chrono::high_resolution_clock::now();

	// ThomasNode* bar = foo.getParseTree(&tokenizedList[0], tokenizedList.size());

	// // timeStart, timeOpenedFile, timeTokenized, timeListToVector, timeAsteriskPtr, timeParsed
	// auto timeParsed = std::chrono::high_resolution_clock::now();

	// bar->print();
	// std::cout << "\n\n\n";

	// std::cout << "Reading File(s): " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeOpenedFile - timeStart).count() / 1000 << " µs\n";
	// std::cout << "Tokenizing: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeTokenized - timeOpenedFile).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(timeTokenized - timeOpenedFile).count() / list.size() << " ns per token)\n";
	// std::cout << "Finding Classes: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeFindingClasses - timeTokenized).count() / 1000 << " µs\n";
	// std::cout << "List-to-Vector: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeListToVector - timeFindingClasses).count() / 1000 << " µs\n";
	// std::cout << "Asterisk-Ptr: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeAsteriskPtr - timeListToVector).count() / 1000 << " µs\n";
	// std::cout << "Parse Rules: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeParseRules - timeAsteriskPtr).count() / 1000 << " µs\n";
	// std::cout << "Parsing: " << std::chrono::duration_cast<std::chrono::nanoseconds>(timeParsed - timeParseRules).count() / 1000 << " µs (" << std::chrono::duration_cast<std::chrono::nanoseconds>(timeParsed - timeParseRules).count() / list.size() << " ns per token)\n";
	// std::cout << "Tokens: " << list.size() << "\n";

	return 0;
}