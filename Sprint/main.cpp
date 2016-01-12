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
	if (pathToDesktop == "/Users/thomasredding/Desktop") {
		// Thomas
		pathToFile = "/Users/thomasredding/Desktop/Sprint/Gold Standard Code/";
	}
	else {
		// Morgan
		pathToFile = "/Users/mredding/Sprint/Gold Standard Code/";
	}
	std::ifstream myfile(pathToFile+"List.hpp");
	if (myfile.is_open()) {
		while(getline(myfile, fileLine)) {
			contents += fileLine;
			contents += "\n";
		}
		myfile.close();
	}
	
	Tokenizer tokenizer;
	std::vector<Token> tokenizedList = tokenizer.process(contents);
    
    std::cout << "NUMBER OF TOKENS: " << tokenizedList.size() << std::endl << std::endl;
	
	for (int i=0; i<tokenizedList.size(); i++) {
		std::cout << "<" << tokenizer.tokenTypeToString(tokenizedList[i].type) << " : " << tokenizedList[i].str << " : " << tokenizedList[i].lineNum << " : " << tokenizedList[i].charNum << ">\n";
	}
	
    std::vector< Rule > rules =
    {
		Rule(general, {klass, NEWLINE}),
		Rule(general, {method_declaration_and_implementation, NEWLINE}),
		
		// class
		Rule(klass, {KEYWORD_CLASS, IDENTIFIER, LESS_THAN, etc, template_parameter, GREATER_THAN, NEWLINE, INDENT, etc, in_class, DEDENT}),
		Rule(klass, {KEYWORD_CLASS, IDENTIFIER, NEWLINE, INDENT, etc, in_class, DEDENT}),
        Rule(in_class, {method_declaration_and_implementation}),
		Rule(in_class, {method_declaration}),
//		Rule(in_class, {member_variable_declaration, NEWLINE}),
        
        
		// functions, methods & member variables
        Rule(method_declaration, {type, IDENTIFIER, LESS_THAN, etc, template_parameter, GREATER_THAN, OPEN_PARENTHESIS, etc, function_parameter, CLOSE_PARENTHESIS, NEWLINE}),
        Rule(method_declaration, {type, IDENTIFIER, OPEN_PARENTHESIS, etc, function_parameter, CLOSE_PARENTHESIS, NEWLINE}),
		Rule(method_declaration_and_implementation, {type, IDENTIFIER, LESS_THAN, etc, template_parameter, GREATER_THAN, OPEN_PARENTHESIS, etc, function_parameter, CLOSE_PARENTHESIS, NEWLINE, block }),
		Rule(method_declaration_and_implementation, {type, IDENTIFIER, OPEN_PARENTHESIS, etc, function_parameter, CLOSE_PARENTHESIS, NEWLINE, block }),
//		Rule(member_variable_declaration, {}),
        
        Rule(template_parameter, {COMMA, template_parameter}),
        Rule(template_parameter, {template_parameter_child, IDENTIFIER}),

        Rule(template_parameter_child, {IDENTIFIER, LESS_THAN, template_parameter_child, GREATER_THAN, ASTERISK}),
        Rule(template_parameter_child, {IDENTIFIER, ASTERISK}),
        Rule(template_parameter_child, {KEYWORD_INT}), // TODO: make this a set of all int types
        Rule(template_parameter_child, {KEYWORD_CLASS}),
        
        Rule(function_parameter, {COMMA, function_parameter}),
        Rule(function_parameter, {type, IDENTIFIER}),
        Rule(type, {IDENTIFIER, ASTERISK}),
        Rule(type, {IDENTIFIER, LESS_THAN, type, GREATER_THAN, ASTERISK}),
        Rule(type, {IDENTIFIER, LESS_THAN, type, GREATER_THAN}),
        Rule(type, {IDENTIFIER}),
        Rule(type, {KEYWORD_INT}),
        Rule(type, {KEYWORD_INT8}),
        Rule(type, {KEYWORD_INT16}),
        Rule(type, {KEYWORD_INT32}),
        Rule(type, {KEYWORD_INT64}),
        Rule(type, {KEYWORD_UINT}),
        Rule(type, {KEYWORD_UINT8}),
        Rule(type, {KEYWORD_UINT16}),
        Rule(type, {KEYWORD_UINT32}),
        Rule(type, {KEYWORD_UINT64}),
        
		
//		// loops
//		Rule(while_loop, {KEYWORD_WHILE, or_expression, NEWLINE, block, NEWLINE}),
//		Rule(for_in_loop, {KEYWORD_FOR, type_name, IDENTIFIER, KEYWORD_IN, IDENTIFIER, NEWLINE, block, NEWLINE}),
//		
//		// if (-else)
//		Rule(if_else_statement, {if_statement, NEWLINE, KEYWORD_ELSE, NEWLINE, block, NEWLINE}),
//		Rule(if_statement, {KEYWORD_IF, or_expression, NEWLINE, block}),
//		
//		// blocks
//		Rule(block_components, {line}),
//		Rule(block_components, {block}),
//		Rule(block, {INDENT, etc, block_components, DEDENT}),
//		
//		// declarations, assignments
//		Rule(line, {type, IDENTIFIER, assignment_rightmost_expression, NEWLINE}),
//        Rule(line, {type, IDENTIFIER, etc, simple_assign, NEWLINE}),
//		Rule(line, {type, IDENTIFIER, etc, simple_assign, assignment_rightmost_expression, NEWLINE}),
//		Rule(simple_assign, {assignment_set, IDENTIFIER}),
//		
//		Rule(expression, {IDENTIFIER, etc, simple_assign}),
//		Rule(expression, {IDENTIFIER, etc, simple_assign, assignment_rightmost_expression}),
//		Rule(expression, {IDENTIFIER, assignment_rightmost_expression}),
//		Rule(expression, {or_expression}),
//		
//		Rule(assignment_set, {EQUALS}),
//		Rule(assignment_set, {PLUS, EQUALS}),
//		Rule(assignment_set, {MINUS, EQUALS}),
//		Rule(assignment_set, {ASTERISK, EQUALS}),
//		Rule(assignment_set, {SLASH, EQUALS}),
//		Rule(assignment_set, {PERCENT, EQUALS}),
//		Rule(assignment_set, {LESS_THAN, EQUALS}),
//		Rule(assignment_set, {GREATER_THAN, EQUALS}),
//		Rule(assignment_rightmost_expression, {assignment_set, or_expression}),
//		
//		Rule(or_expression, {and_expression, KEYWORD_OR, or_expression}),
//		Rule(or_expression, {and_expression}),
//		
//		Rule(and_expression, {xor_expression, KEYWORD_AND, and_expression}),
//		Rule(and_expression, {xor_expression}),
//		
//		Rule(xor_expression, {equality_expression, KEYWORD_XOR, xor_expression}),
//		Rule(xor_expression, {equality_expression}),
//		
//		Rule(equality_set, {EQUALS, EQUALS}), Rule(equality_set, {EXCLAMATION_POINT, EQUALS}),
//		Rule(equality_expression, {relational_expression, equality_set, equality_expression}),
//		Rule(equality_expression, {relational_expression}),
//		
//		Rule(relational_set, {LESS_THAN}), Rule(relational_set, {GREATER_THAN}), Rule(relational_set, {LESS_THAN, EQUALS}), Rule(relational_set, {GREATER_THAN, EQUALS}),
//		Rule(relational_expression, {shift_expression, relational_set, relational_expression}),
//		Rule(relational_expression, {shift_expression}),
//		
//		Rule(shift_set, {LESS_THAN, LESS_THAN}), Rule(shift_set, {GREATER_THAN, GREATER_THAN}),
//		Rule(shift_expression, {plus_expression, shift_set, shift_expression}),
//		Rule(shift_expression, {plus_expression}),
//		
//		Rule(plus_set, {PLUS}), Rule(plus_set, {MINUS}),
//		Rule(plus_expression, {times_expression, plus_set, plus_expression}),
//		Rule(plus_expression, {times_expression}),
//		
//		Rule(times_set, {ASTERISK}), Rule(times_set, {SLASH}), Rule(times_set, {PERCENT}),
//		Rule(times_expression, {simple_value, plus_set, times_expression}),
//		Rule(times_expression, {unary_expression}),
//		
//		Rule(unary_expression, {KEYWORD_NEW, type, OPEN_PARENTHESIS, etc, function_parameter, CLOSE_PARENTHESIS}),
//		Rule(unary_expression, {KEYWORD_NEW, type, OPEN_BRACKET, or_expression, CLOSE_BRACKET}),
//		Rule(unary_expression, {KEYWORD_DELETE, simple_value}),
//		Rule(unary_expression, {simple_value}),
//		
//		Rule(simple_value, {literal}),
//		Rule(simple_value, {IDENTIFIER, etc, pointer_value}),
//		Rule(simple_value, {IDENTIFIER, etc, access_value}),
//		
//		Rule(access_value, {PERIOD, IDENTIFIER}),
//		Rule(pointer_access, {MINUS, GREATER_THAN, IDENTIFIER}),
//		Rule(access_value, {PERIOD, function_call}),
//		Rule(pointer_access, {MINUS, GREATER_THAN, function_call}),
//		
//		Rule(literal, {INTEGER_LITERAL}),
//		Rule(literal, {FLOAT_LITERAL}),
//		Rule(literal, {CHARACTER_LITERAL}),
//		Rule(literal, {STRING_LITERAL}),
//		
//		Rule(function_call, {IDENTIFIER, OPEN_PARENTHESIS, expression, etc, function_arg , CLOSE_PARENTHESIS}),
//		Rule(function_arg, {COMMA, expression}),
//		
//		Rule(access_modifier_set, {KEYWORD_PUBLIC}),
//		Rule(access_modifier_set, {KEYWORD_PROTECTED}),
//		Rule(access_modifier_set, {KEYWORD_PRIVATE})
    };

    Parser parser(rules);
    
//    Rule program_rule(program, {etc, general});
    Rule program_rule(klass, {KEYWORD_CLASS, IDENTIFIER, NEWLINE, INDENT, etc, in_class, DEDENT, NEWLINE});
    ParseTree* tree = parser.match(&tokenizedList[0], tokenizedList.size(), program_rule);

    if (tree != NULL) {
        std::cout << *tree << std::endl << std::endl;
        std::cout << tree->children.size() << std::endl << std::endl;
    }
    else {
		std::cout << "null tree" << std::endl;
    }
	
	Compiler compiler;
	compiler.compile(tree, pathToFile);
	return 0;
}
