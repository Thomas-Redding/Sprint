#include <iostream>
#include <unistd.h>
#include <map>
#include <stack>
#include <unordered_map>
#include <functional>
#include <chrono>

#include "../include/Tokenizer.hpp"
#include "../include/Sweetener.hpp"
#include "../include/ThomasParser.hpp"
#include "../include/FindModifiedFiles.hpp"

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

	for (auto it = list.begin(); it != list.end(); ++it) {
		std::cout << *it << "\n";
	}

	std::vector<bool> leftToRight = {
		// TODO: document what 'true' and 'false' mean... or better yet, use an enum
		true, false, true, true, true, true, true, true, true, true, false, true, true, false
	};
	std::vector<ThomasParseRule> listOfRules;
	// todo: templates
	// listOfRules.push_back(ThomasParseRule(-1, general, {unary_value, T_PLUS_PLUS}, unary_clause));												// <T>

	// true
	listOfRules.push_back(ThomasParseRule(  0, general, {unary1_value, T_PLUS_PLUS}, unary1_clause));													// x++
	listOfRules.push_back(ThomasParseRule(  0, general, {unary1_value, T_MINUS_MINUS}, unary1_clause));													// x--
	listOfRules.push_back(ThomasParseRule(  0, general, {unary1_value, parenthesis_block}, unary1_clause));												// x(y)
	listOfRules.push_back(ThomasParseRule(  0, general, {unary1_value, bracket_block}, unary1_clause));													// x[y]
	listOfRules.push_back(ThomasParseRule(  0, general, {unary1_value, T_PERIOD}, unary1_clause));														// x.y

	// false
	listOfRules.push_back(ThomasParseRule( 10, general, {T_POSITIVE, unary2_value}, unary2_clause));													// +x
	listOfRules.push_back(ThomasParseRule( 10, general, {T_NEGATIVE, unary2_value}, unary2_clause));													// -x
	listOfRules.push_back(ThomasParseRule( 10, general, {T_PLUS_PLUS, unary2_value}, unary2_clause));													// ++x
	listOfRules.push_back(ThomasParseRule( 10, general, {T_MINUS_MINUS, unary2_value}, unary2_clause));												// --x
	listOfRules.push_back(ThomasParseRule( 10, general, {T_TILDE, unary2_value}, unary2_clause));														// ~x
	listOfRules.push_back(ThomasParseRule( 10, general, {T_KEYWORD_NOT, unary2_value}, unary2_clause));												// not x
	listOfRules.push_back(ThomasParseRule( 10, general, {T_KEYWORD_NEW, unary2_value}, unary2_clause));												// new x

	// true
	listOfRules.push_back(ThomasParseRule( 20, general, {mult_value, T_ASTERISK, mult_value}, mult_clause));											// x * y
	listOfRules.push_back(ThomasParseRule( 20, general, {mult_value, T_SLASH, mult_value}, mult_clause));											// x / y
	listOfRules.push_back(ThomasParseRule( 20, general, {mult_value, T_PERCENT, mult_value}, mult_clause));											// x % y

	// true
	listOfRules.push_back(ThomasParseRule( 30, general, {plus_value, T_PLUS, plus_value}, plus_clause));												// x + y
	listOfRules.push_back(ThomasParseRule( 30, general, {plus_value, T_MINUS, plus_value}, plus_clause));											// x - y

	// true
	listOfRules.push_back(ThomasParseRule( 40, general, {shift_value, T_SHIFT_LEFT, shift_value}, shift_clause));									// x << y
	listOfRules.push_back(ThomasParseRule( 40, general, {shift_value, T_SHIFT_RIGHT, shift_value}, shift_clause));									// x >> y

	listOfRules.push_back(ThomasParseRule( 50, general, {inequality_value, T_LESS_THAN, inequality_value}, inequality_clause));						// x < y
	listOfRules.push_back(ThomasParseRule( 50, general, {inequality_value, T_GREATER_THAN, inequality_value}, inequality_clause));					// x > y
	listOfRules.push_back(ThomasParseRule( 50, general, {inequality_value, T_LESS_THAN_EQUALS, inequality_value}, inequality_clause));				// x <= y
	listOfRules.push_back(ThomasParseRule( 50, general, {inequality_value, T_GREATER_THAN_EQUALS, inequality_value}, inequality_clause));			// x >= y

	listOfRules.push_back(ThomasParseRule( 60, general, {equality_value, T_EQUAL_EQUALS, equality_value}, equality_clause));							// x == y
	listOfRules.push_back(ThomasParseRule( 60, general, {equality_value, T_EXCLAMATION_POINT_EQUALS, equality_value}, equality_clause));				// x != y
	listOfRules.push_back(ThomasParseRule( 60, general, {equality_value, T_EQUAL_EQUAL_EQUALS, equality_value}, equality_clause));					// x === y
	listOfRules.push_back(ThomasParseRule( 60, general, {equality_value, T_EXCLAMATION_POINT_EQUAL_EQUALS, equality_value}, equality_clause));		// x !== y

	listOfRules.push_back(ThomasParseRule( 70, general, {bitwise_and_value, T_KEYWORD_AND, bitwise_and_value}, bitwise_and_clause));					// x & y

	listOfRules.push_back(ThomasParseRule( 80, general, {bitwise_xor_value, T_KEYWORD_XOR, bitwise_xor_value}, bitwise_xor_clause));					// x ^ y

	listOfRules.push_back(ThomasParseRule( 90, general, {bitwise_or_value, T_KEYWORD_OR, bitwise_or_value}, bitwise_or_clause));						// x | y

	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_QUESTION_MARK, setting_value, T_COLON, setting_value}, ternary_clause));	// x ? y : z
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_EQUALS, setting_value}, setting_clause));									// x = y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_PLUS_EQUALS, setting_value}, setting_clause));							// x += y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_MINUS_EQUALS, setting_value}, setting_clause));							// x -= y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_ASTERISK_EQUALS, setting_value}, setting_clause));						// x *= y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_SLASH_EQUALS, setting_value}, setting_clause));							// x /= y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_PERCENT_EQUALS, setting_value}, setting_clause));							// x %= y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_AMPERSAND_EQUALS, setting_value}, setting_clause));						// x &= y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_CARROT_EQUALS, setting_value}, setting_clause));							// x ^= y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_VERTICAL_BAR_EQUALS, setting_value}, setting_clause));					// x |= y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_COLON_EQUALS, setting_value}, setting_clause));							// x := y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_SHIFT_LEFT_EQUALS, setting_value}, setting_clause));						// x <<= y
	listOfRules.push_back(ThomasParseRule(100, general, {setting_value, T_SHIFT_RIGHT_EQUALS, setting_value}, setting_clause));						// x >>= y

	listOfRules.push_back(ThomasParseRule(110, general, {comma_value, T_COMMA, comma_value}, comma_clause));										// x, y
	
	listOfRules.push_back(ThomasParseRule(120, general, {comma_value, T_SEMI_COLON}, statement));													// x;

	listOfRules.push_back(ThomasParseRule(130, general, {statement, statement}, statements));														// x y



	ThomasParser foo(leftToRight, listOfRules);

	auto timeParseRules = std::chrono::high_resolution_clock::now();

	ThomasNode* bar = foo.getParseTree(&tokenizedList[0], tokenizedList.size());

	// timeStart, timeOpenedFile, timeTokenized, timeListToVector, timeAsteriskPtr, timeParsed
	auto timeParsed = std::chrono::high_resolution_clock::now();

	bar->print();
	std::cout << "\n\n\n";

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