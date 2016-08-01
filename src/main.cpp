#include <iostream>
#include <fstream>
#include <unistd.h>
#include <map>
#include <unordered_map>
#include "../include/Tokenizer.hpp"
#include "../include/Sweetener.hpp"
#include "../include/ThomasParser.hpp"
#include "MorganRules.cpp"
#include <functional>

struct Class {
	Class() {};
	Class(std::string base_name) : base_name(base_name) {};
	Class(Token* tokens, const uint64_t n) {
		base_name = tokens[1].str;

		if (tokens[2].type != LESS_THAN) return;

		uint64_t i = 3;
		while (i < n && tokens[i].type != GREATER_THAN) {
			if (tokens[i].isIntKeyword()) {
				if (i + 1 >= n) {
					throw std::runtime_error("Error: while parsing template arguments for class " + base_name + ", found 'EOF' but expected IDENTIFIER");
				}
				template_parameters.push_back(std::pair<TokenType, std::string>(tokens[i].type, tokens[i + 1].str));
				if (tokens[i + 1].type != IDENTIFIER) {
					throw std::runtime_error("Error: while parsing template arguments for class " + base_name + ", found '" + tokens[i + 1].str + "' but expected IDENTIFIER");
				}
				if (i + 2 >= n) {
					throw std::runtime_error("Error: while parsing template arguments for class " + base_name + ", found 'EOF' but expected ',' or '>'");
				}
				if (tokens[i + 2].type == GREATER_THAN) {
					break;
				}
				if (tokens[i + 2].type != COMMA) {
					throw std::runtime_error("Error: while parsing template arguments for class " + base_name + ", found '" + tokens[i + 2].str + "' but expected ',' or '>'");
				}
				i += 3;
			}
			else {
				if (i + 1 >= n) {
					throw std::runtime_error("Error: while parsing template arguments for class " + base_name + ", found 'EOF' but expected IDENTIFIER");
				}
				template_parameters.push_back(std::pair<TokenType, std::string>(KEYWORD_CLASS, tokens[i].str));
				if (tokens[i + 1].type != COMMA && tokens[i + 1].type != GREATER_THAN) {
					throw std::runtime_error("Error: while parsing template arguments for class " + base_name + ", found '" + tokens[i + 1].str + "' but expected ',' or '>'");
				}
				if (tokens[i + 1].type == GREATER_THAN) {
					break;
				}
				i += 2;
			}
		}
	}
	friend std::ostream& operator<<(std::ostream& stream, const Class& c) {
		if (c.template_parameters.size() == 0) {
			return stream << c.base_name;
		}
		stream << c.base_name;

		if (c.template_parameters.size() > 0) {
			stream << "<" << Token(c.template_parameters[0].first) << " " << c.template_parameters[0].second;
			for (uint64_t i = 1; i < c.template_parameters.size(); ++i) {
				stream << ", " << Token(c.template_parameters[i].first) << " " << c.template_parameters[i].second;
			}
			stream << ">";
		}
		return stream;
	}
	std::string base_name;
	std::vector<std::pair<TokenType, std::string>> template_parameters;
};

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
	// if (!addFunctionSugar(tokenizedList)) {
	// 	return 0;
	// }

	std::unordered_map<std::string, Class> classes;
	for (uint64_t i = 0; i < tokenizedList.size(); ++i) {
		if (tokenizedList[i].type == KEYWORD_CLASS) {
			Class c(&tokenizedList[i], tokenizedList.size() - i);
			classes.insert(std::pair<std::string, Class>(c.base_name, c));
			std::cout << c << std::endl;
		}
	}

	// determine whether an ASTERISK is "really" a pointer (PTR) token
	for (uint64_t i = 1; i < tokenizedList.size(); ++i) {
		// ')' ']' or 'value' before --> multiplication
		if (tokenizedList[i].type != ASTERISK) {
			continue;
		}
		if (tokenizedList[i - 1].type == CLOSE_PARENTHESIS || tokenizedList[i].type == CLOSE_BRACKET) {
			continue;
		}
		if (tokenizedList[i - 1].isIntKeyword() || tokenizedList[i].type == KEYWORD_DOUBLE || tokenizedList[i].type == KEYWORD_FLOAT || tokenizedList[i].type == KEYWORD_BOOL || tokenizedList[i].type == KEYWORD_CHAR || tokenizedList[i].type == KEYWORD_VAR) {
			tokenizedList[i].type = PTR;
		}
		if (classes.count(tokenizedList[i - 1].str) != 0) {
			tokenizedList[i].type = PTR;
			continue;
		}
		if (tokenizedList[i - 1].type == GREATER_THAN) {
			uint64_t lineNum = tokenizedList[i].lineNum;;
			while (--i < tokenizedList.size() && tokenizedList[i].type != LESS_THAN) {}
			if (i >= tokenizedList.size()) {
				throw std::runtime_error("Error determining type of templated class on line " + std::to_string(lineNum));
			}
			if (classes.count(tokenizedList[i - 1].str) != 0) {
				tokenizedList[i].type = PTR;
			}
		}
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

	std::cout << "\n\n\n";
	std::vector<bool> leftToRight = {
		false, true, false, false, false, false, false, false, false, false, true, false
	};
	std::vector<ThomasParseRule> listOfRules;
	listOfRules.push_back(ThomasParseRule(0, general, {unary_value, T_PLUS_PLUS}, unary_clause));													// x++
	listOfRules.push_back(ThomasParseRule(0, general, {unary_value, T_MINUS_MINUS}, unary_clause));													// x--
	listOfRules.push_back(ThomasParseRule(0, general, {unary_value, parenthesis_block}, unary_clause));												// x(y)
	listOfRules.push_back(ThomasParseRule(0, general, {unary_value, bracket_block}, unary_clause));													// x[y]
	listOfRules.push_back(ThomasParseRule(0, general, {unary_value, T_PERIOD}, unary_clause));														// x.y

	listOfRules.push_back(ThomasParseRule(10, general, {T_POSITIVE, unary_value}, unary_clause));													// +x
	listOfRules.push_back(ThomasParseRule(10, general, {T_NEGATIVE, unary_value}, unary_clause));													// -x
	listOfRules.push_back(ThomasParseRule(10, general, {T_PLUS_PLUS, unary_value}, unary_clause));													// ++x
	listOfRules.push_back(ThomasParseRule(10, general, {T_MINUS_MINUS, unary_value}, unary_clause));												// --x
	listOfRules.push_back(ThomasParseRule(10, general, {T_TILDE, unary_value}, unary_clause));														// ~x
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_NOT, unary_value}, unary_clause));												// not x
	listOfRules.push_back(ThomasParseRule(10, general, {T_PTR, unary_value}, unary_clause));														// *x
	listOfRules.push_back(ThomasParseRule(10, general, {T_KEYWORD_NEW, unary_value}, unary_clause));												// new x

	listOfRules.push_back(ThomasParseRule(20, general, {mult_value, T_ASTERISK, mult_value}, mult_clause));											// x * y
	listOfRules.push_back(ThomasParseRule(20, general, {mult_value, T_SLASH, mult_value}, mult_clause));											// x / y
	listOfRules.push_back(ThomasParseRule(20, general, {mult_value, T_PERCENT, mult_value}, mult_clause));											// x % y

	listOfRules.push_back(ThomasParseRule(30, general, {plus_value, T_PLUS, plus_value}, plus_clause));												// x + y
	listOfRules.push_back(ThomasParseRule(30, general, {plus_value, T_MINUS, plus_value}, plus_clause));											// x - y

	listOfRules.push_back(ThomasParseRule(40, general, {shift_value, T_SHIFT_LEFT, shift_value}, shift_clause));									// x << y
	listOfRules.push_back(ThomasParseRule(40, general, {shift_value, T_SHIFT_RIGHT, shift_value}, shift_clause));									// x >> y

	listOfRules.push_back(ThomasParseRule(50, general, {inequality_value, T_LESS_THAN, inequality_value}, inequality_clause));						// x < y
	listOfRules.push_back(ThomasParseRule(50, general, {inequality_value, T_GREATER_THAN, inequality_value}, inequality_clause));					// x > y
	listOfRules.push_back(ThomasParseRule(50, general, {inequality_value, T_LESS_THAN_EQUALS, inequality_value}, inequality_clause));				// x <= y
	listOfRules.push_back(ThomasParseRule(50, general, {inequality_value, T_GREATER_THAN_EQUALS, inequality_value}, inequality_clause));			// x >= y

	listOfRules.push_back(ThomasParseRule(60, general, {equality_value, T_EQUAL_EQUALS, equality_value}, equality_clause));							// x == y
	listOfRules.push_back(ThomasParseRule(60, general, {equality_value, T_EXCLAMATION_POINT_EQUALS, equality_value}, equality_clause));				// x != y
	listOfRules.push_back(ThomasParseRule(60, general, {equality_value, T_EQUAL_EQUAL_EQUALS, equality_value}, equality_clause));					// x === y
	listOfRules.push_back(ThomasParseRule(60, general, {equality_value, T_EXCLAMATION_POINT_EQUAL_EQUALS, equality_value}, equality_clause));		// x !== y

	listOfRules.push_back(ThomasParseRule(70, general, {bitwise_and_value, T_KEYWORD_AND, bitwise_and_value}, bitwise_and_clause));					// x & y

	listOfRules.push_back(ThomasParseRule(80, general, {bitwise_xor_value, T_KEYWORD_XOR, bitwise_xor_value}, bitwise_xor_clause));					// x ^ y

	listOfRules.push_back(ThomasParseRule(90, general, {bitwise_or_value, T_KEYWORD_OR, bitwise_or_value}, bitwise_or_clause));						// x | y

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
	
	addMorganRules(listOfRules);

	ThomasParser foo(leftToRight, listOfRules);
	ThomasNode* bar = foo.getParseTree(&tokenizedList[0], tokenizedList.size());
	bar->print();
	std::cout << "\n\n\n";
	return 0;
}