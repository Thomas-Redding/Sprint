//
//  main.cpp
//  Sprint
//
//  Created by Thomas Redding on 9/28/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "ResourcePath.hpp"
#include "Compiler.hpp"

int main(int argc, const char * argv[]) {
	std::string fileLine;
	std::string contents = "";
	std::string pathToDesktop = desktopPath();
	std::string pathToFile;
	if(pathToDesktop == "/Users/thomasredding/Desktop") {
		// Thomas
		pathToFile = "/Users/thomasredding/Desktop/Sprint/Gold Standard Code/";
	}
	else {
		// Morgan
		pathToFile = "/Users/mredding/Sprint/Gold Standard Code/";
	}
	std::ifstream myfile(pathToFile+"List.hpp");
	if(myfile.is_open()) {
		while(getline(myfile, fileLine)) {
			contents += fileLine;
			contents += "\n";
		}
		myfile.close();
	}
	
	Tokenizer tokenizer;
	std::vector<Token> tokenizedList = tokenizer.process(contents);
	
	for(int i=0; i<tokenizedList.size(); i++) {
		std::cout << "<" << tokenizer.tokenTypeToString(tokenizedList[i].type) << " : " << tokenizedList[i].str << " : " << tokenizedList[i].lineNum << " : " << tokenizedList[i].charNum << ">\n";
	}
	
    std::vector< Rule > rules =
    {
//		Rule(general, {klass, NEWLINE}),
//		Rule(general, {method_declaration_and_implementation, NEWLINE}),
		Rule(general, {method_declaration, NEWLINE}),
		
		// class
//		Rule(klass, {KEYWORD_CLASS}),
//		Rule(in_class, {method_declaration, NEWLINE}),
//		Rule(in_class, {method_declaration_and_implementation, NEWLINE}),
//		Rule(in_class, {member_variable_declaration, NEWLINE}),
		
		// functions, methods & member variables
		Rule(method_declaration, { IDENTIFIER, IDENTIFIER, OPEN_PARENTHESIS, IDENTIFIER, IDENTIFIER, CLOSE_PARENTHESIS }),
//		Rule(method_declaration_and_implementation, {}),
//		Rule(member_variable_declaration, {}),
		
		// loops
		Rule(while_loop, {KEYWORD_WHILE, logical_or_expression, NEWLINE, block, NEWLINE}),
		Rule(for_in_loop, {KEYWORD_FOR, type_name, IDENTIFIER, KEYWORD_IN, IDENTIFIER, NEWLINE, block, NEWLINE}),
		
		// if (-else)
		Rule(if_else_statement, {if_statement, NEWLINE, KEYWORD_ELSE, NEWLINE, block, NEWLINE}),
		Rule(if_statement, {KEYWORD_IF, logical_or_expression, NEWLINE, block}),
		
		// blocks
		Rule(block_components, {line}),
		Rule(block_components, {block}),
		Rule(block, {INDENT, etc, block_components, DEDENT}),
		
		// declarations, assignments
		Rule(line, {type_name, IDENTIFIER, etc, simple_assign, NEWLINE}),
		Rule(line, {type_name, IDENTIFIER, etc, simple_assign, assignment_rightmost_expression, NEWLINE}),
		Rule(simple_assign, {assignment_set, IDENTIFIER}),
		
		Rule(expression, {IDENTIFIER, etc, simple_assign}),
		Rule(expression, {IDENTIFIER, etc, simple_assign, assignment_rightmost_expression}),
		Rule(expression, {IDENTIFIER, assignment_rightmost_expression}),
		Rule(expression, {logical_or_expression}),
		
		Rule(assignment_set, {EQUALS}),
		Rule(assignment_set, {PLUS, EQUALS}),
		Rule(assignment_set, {MINUS, EQUALS}),
		Rule(assignment_set, {ASTERISK, EQUALS}),
		Rule(assignment_set, {SLASH, EQUALS}),
		Rule(assignment_set, {PERCENT, EQUALS}),
		Rule(assignment_set, {LESS_THAN, EQUALS}),
		Rule(assignment_set, {GREATER_THAN, EQUALS}),
		Rule(assignment_rightmost_expression, {assignment_set, logical_or_expression}),
		
		Rule(or_expression, {or_expression, KEYWORD_OR, and_expression}),
		Rule(or_expression, {and_expression}),
		
		Rule(and_expression, {and_expression, KEYWORD_AND, xor_expression}),
		Rule(and_expression, {xor_expression}),
		
		Rule(xor_expression, {xor_expression, KEYWORD_XOR, equality_expression}),
		Rule(xor_expression, {and_expression}),
		
		Rule(equality_set, {EQUALS, EQUALS}), Rule(equality_set, {EXCLAMATION_POINT, EQUALS}),
		Rule(equality_expression, {equality_expression, equality_set, relational_expression}),
		Rule(equality_expression, {relational_expression}),
		
		Rule(relational_set, {LESS_THAN}), Rule(relational_set, {GREATER_THAN}), Rule(relational_set, {LESS_THAN, EQUALS}), Rule(relational_set, {GREATER_THAN, EQUALS}),
		Rule(relational_expression, {relational_expression, relational_set, shift_expression}),
		Rule(relational_expression, {shift_expression}),
		
		Rule(shift_set, {LESS_THAN, LESS_THAN}), Rule(shift_set, {GREATER_THAN, GREATER_THAN}),
		Rule(shift_expression, {shift_expression, shift_set, plus_expression}),
		Rule(shift_expression, {plus_expression}),
		
		Rule(plus_set, {PLUS}), Rule(plus_set, {MINUS}),
		Rule(plus_expression, {plus_expression, plus_set, times_expression}),
		Rule(plus_expression, {times_expression}),
		
		Rule(times_set, {ASTERISK}), Rule(times_set, {SLASH}), Rule(times_set, {PERCENT}),
		Rule(times_expression, {times_expression, plus_set, times_expression}),
		Rule(times_expression, {simple_value}),
		
		Rule(simple_value, {literal}),
		Rule(simple_value, {IDENTIFIER, etc, pointer_value}),
		Rule(simple_value, {IDENTIFIER, etc, access_value}),
		
		Rule(access_value, {PERIOD, IDENTIFIER}),
		Rule(pointer_access, {MINUS, GREATER_THAN, IDENTIFIER}),
		Rule(access_value, {PERIOD, function_call}),
		Rule(pointer_access, {MINUS, GREATER_THAN, function_call}),
		
		Rule(literal, {INTEGER_LITERAL}),
		Rule(literal, {FLOAT_LITERAL}),
		Rule(literal, {CHARACTER_LITERAL}),
		Rule(literal, {STRING_LITERAL}),
		
		Rule(function_call, {IDENTIFIER, OPEN_PARENTHESIS, expression, etc, function_arg , CLOSE_PARENTHESIS}),
		Rule(function_arg, {COMMA, expression}),
    };

    Parser parser(rules);
    
    Rule program_rule(program, {etc, general});
    ParseTree* tree = parser.match(&tokenizedList[0], tokenizedList.size(), program_rule);

	if(tree != NULL)
	    std::cout << *tree << std::endl;
	else
		std::cout << "null tree" << std::endl;
	
	Compiler compiler;
	compiler.compile(tree, pathToFile);
	return 0;
}
