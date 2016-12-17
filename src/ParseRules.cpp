#include "../include/Parser.hpp"

void addParseRules(std::vector<bool> &leftToRight, std::vector<ParseRule> &listOfRules) {
	leftToRight= {
		// TODO: document what 'true' and 'false' mean... or better yet, use an enum
		// -10, 0 , 10   , 20  , 30  , 40  , 50  , 60  , 70  , 80  , 90  , 100 , 105 , 107 , 110  , 120  , 130
		true, true, false, true, true, true, true, true, true, true, true, true, true, false, false, false, false
	};

	// class implementations
	// class Foo { ... 
	listOfRules.push_back(ParseRule(-10, {}, {}, {
		P_KEYWORD_CLASS,
		P_IDENTIFIER,
		block_of_statements
	}, class_implementation));
	// class Foo <T> { ... 
	listOfRules.push_back(ParseRule(-10, {}, {}, {
		P_KEYWORD_CLASS,
		P_IDENTIFIER,
		templates,
		block_of_statements
	},class_implementation));
	// class Foo<T> : Bar
	listOfRules.push_back(ParseRule(-10, {}, {}, {
		P_KEYWORD_CLASS,
		P_IDENTIFIER,
		P_COLON,
		P_IDENTIFIER,
		block_of_statements
	}, class_implementation));
	// class Foo<T> : Bar<U>
	listOfRules.push_back(ParseRule(-10, {}, {}, {
		P_KEYWORD_CLASS,
		P_IDENTIFIER,
		templates,
		P_COLON,
		P_IDENTIFIER,
		templates,
		block_of_statements
	}, class_implementation));
	// class Foo: Bar<S>
	listOfRules.push_back(ParseRule(-10, {}, {}, {
		P_KEYWORD_CLASS,
		P_IDENTIFIER,
		P_COLON,
		P_IDENTIFIER,
		templates,
		block_of_statements
	}, class_implementation));
	// class Foo<T, S> : Bar<T>
	listOfRules.push_back(ParseRule(-10, {}, {}, {
		P_KEYWORD_CLASS,
		P_IDENTIFIER,
		templates,
		P_EXTENDS,
		P_IDENTIFIER,
		templates,
		block_of_statements
	}, class_implementation));

	// function_head
	// foo(...) -> int
	listOfRules.push_back(ParseRule(-10, {}, {}, {
		function_name_candidate,
		parenthesis,
		P_ARROW,
	}, function_head));
	// foo<...>(...) ->
	listOfRules.push_back(ParseRule(-10, {}, {}, {
		function_name_candidate,
		templates,
		parenthesis,
		P_ARROW,
	}, function_head));
	
	// function declaration
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, raw_type_or_void, P_SEMI_COLON }, function_declaration));
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, P_IDENTIFIER, P_SEMI_COLON }, function_declaration));
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, P_IDENTIFIER, templates, P_SEMI_COLON }, function_declaration));
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, block_of_statements, P_SEMI_COLON }, function_declaration));
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, bracket_block, P_SEMI_COLON }, function_declaration));

	// function implementation
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, raw_type_or_void, block_of_statements }, function_declaration));
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, P_IDENTIFIER, block_of_statements }, function_declaration));
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, P_IDENTIFIER, templates, block_of_statements }, function_declaration));
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, block_of_statements, block_of_statements }, function_declaration));
	listOfRules.push_back(ParseRule(-10, {}, {}, { function_head, bracket_block, block_of_statements }, function_declaration));

	// enum Foo {...}
	listOfRules.push_back(ParseRule(-10, {}, {}, {P_KEYWORD_ENUM, P_IDENTIFIER, block_of_statements}, enum_implementation));
	// namespaces Foo {...}
	listOfRules.push_back(ParseRule(-10, {}, {}, {P_KEYWORD_NAMESPACE, P_IDENTIFIER, block_of_statements}, namespace_implementation));

	listOfRules.push_back(ParseRule(  0, {}, {}, {unary1_value, P_PLUS_PLUS}, unary1_clause));													// x++
	listOfRules.push_back(ParseRule(  0, {}, {}, {unary1_value, P_MINUS_MINUS}, unary1_clause));													// x--
	listOfRules.push_back(ParseRule(  0, {}, {function_head}, {unary1_value, parenthesis}, unary1_clause));												// x(y)
	listOfRules.push_back(ParseRule(  0, {}, {}, {unary1_value, bracket_block}, unary1_clause));													// x[y]
	listOfRules.push_back(ParseRule(  0, {}, {}, {unary1_value, P_PERIOD}, unary1_clause));														// x.y

	// false
	listOfRules.push_back(ParseRule( 10, {}, {function_declaration}, {P_POSITIVE, unary2_value}, unary2_clause));													// +x
	listOfRules.push_back(ParseRule( 10, {}, {function_declaration}, {P_NEGATIVE, unary2_value}, unary2_clause));													// -x
	listOfRules.push_back(ParseRule( 10, {}, {function_declaration}, {P_PLUS_PLUS, unary2_value}, unary2_clause));													// ++x
	listOfRules.push_back(ParseRule( 10, {}, {function_declaration}, {P_MINUS_MINUS, unary2_value}, unary2_clause));												// --x
	listOfRules.push_back(ParseRule( 10, {}, {function_declaration}, {P_TILDE, unary2_value}, unary2_clause));														// ~x
	listOfRules.push_back(ParseRule( 10, {}, {function_declaration}, {P_KEYWORD_NOT, unary2_value}, unary2_clause));												// not x
	listOfRules.push_back(ParseRule( 10, {}, {function_declaration}, {P_KEYWORD_NEW, unary2_value}, unary2_clause));												// new x

	// true
	listOfRules.push_back(ParseRule( 20, {}, {}, {mult_value, P_ASTERISK, mult_value}, mult_clause));											// x * y
	listOfRules.push_back(ParseRule( 20, {}, {}, {mult_value, P_SLASH, mult_value}, mult_clause));											// x / y
	listOfRules.push_back(ParseRule( 20, {}, {}, {mult_value, P_PERCENT, mult_value}, mult_clause));											// x % y

	// true
	listOfRules.push_back(ParseRule( 30, {}, {}, {plus_value, P_PLUS, plus_value}, plus_clause));												// x + y
	listOfRules.push_back(ParseRule( 30, {}, {}, {plus_value, P_MINUS, plus_value}, plus_clause));											// x - y

	// true
	listOfRules.push_back(ParseRule( 40, {}, {}, {shift_value, P_SHIFT_LEFT, shift_value}, shift_clause));									// x << y
	listOfRules.push_back(ParseRule( 40, {}, {}, {shift_value, P_SHIFT_RIGHT, shift_value}, shift_clause));									// x >> y

	listOfRules.push_back(ParseRule( 50, {}, {}, {inequality_value, P_LESS_THAN, inequality_value}, inequality_clause));						// x < y
	listOfRules.push_back(ParseRule( 50, {}, {}, {inequality_value, P_GREATER_THAN, inequality_value}, inequality_clause));					// x > y
	listOfRules.push_back(ParseRule( 50, {}, {}, {inequality_value, P_LESS_THAN_EQUALS, inequality_value}, inequality_clause));				// x <= y
	listOfRules.push_back(ParseRule( 50, {}, {}, {inequality_value, P_GREATER_THAN_EQUALS, inequality_value}, inequality_clause));			// x >= y

	listOfRules.push_back(ParseRule( 60, {}, {}, {equality_value, P_EQUAL_EQUALS, equality_value}, equality_clause));							// x == y
	listOfRules.push_back(ParseRule( 60, {}, {}, {equality_value, P_EXCLAMATION_POINT_EQUALS, equality_value}, equality_clause));				// x != y
	listOfRules.push_back(ParseRule( 60, {}, {}, {equality_value, P_EQUAL_EQUAL_EQUALS, equality_value}, equality_clause));					// x === y
	listOfRules.push_back(ParseRule( 60, {}, {}, {equality_value, P_EXCLAMATION_POINT_EQUAL_EQUALS, equality_value}, equality_clause));		// x !== y

	listOfRules.push_back(ParseRule( 70, {}, {}, {bitwise_and_value, P_KEYWORD_AND, bitwise_and_value}, bitwise_and_clause));					// x & y

	listOfRules.push_back(ParseRule( 80, {}, {}, {bitwise_xor_value, P_KEYWORD_XOR, bitwise_xor_value}, bitwise_xor_clause));					// x ^ y

	listOfRules.push_back(ParseRule( 90, {}, {}, {bitwise_or_value, P_KEYWORD_OR, bitwise_or_value}, bitwise_or_clause));						// x | y

	listOfRules.push_back(ParseRule(100, {}, {}, {ternary_value, P_QUESTION_MARK, ternary_value, P_COLON, ternary_value}, ternary_clause));	// x ? y : z

	
	listOfRules.push_back(ParseRule(105, {}, {}, {comma_value, P_COLON, comma_value}, colon_clause));											// x : z
	listOfRules.push_back(ParseRule(105, {}, {}, {raw_type, P_COLON, raw_type}, colon_type_clause));
	listOfRules.push_back(ParseRule(105, {}, {}, {raw_type, P_COLON, comma_value}, colon_type_clause));
	listOfRules.push_back(ParseRule(105, {}, {}, {comma_value, P_COLON, raw_type}, colon_type_clause));

	listOfRules.push_back(ParseRule(107, {}, {}, {comma_value, P_COMMA, comma_value}, comma_clause));											// x, y
	listOfRules.push_back(ParseRule(107, {}, {}, {colon_clause, P_COMMA, colon_clause}, colon_list));										// a : b, c : d
	listOfRules.push_back(ParseRule(107, {}, {}, {colon_clause, P_COMMA, colon_list}, colon_list));										// a : b, c : d


	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_EQUALS, setting_value}, setting_clause));									// x = y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_PLUS_EQUALS, setting_value}, setting_clause));								// x += y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_MINUS_EQUALS, setting_value}, setting_clause));							// x -= y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_ASTERISK_EQUALS, setting_value}, setting_clause));							// x *= y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_SLASH_EQUALS, setting_value}, setting_clause));							// x /= y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_PERCENT_EQUALS, setting_value}, setting_clause));							// x %= y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_AMPERSAND_EQUALS, setting_value}, setting_clause));						// x &= y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_CARROT_EQUALS, setting_value}, setting_clause));							// x ^= y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_VERTICAL_BAR_EQUALS, setting_value}, setting_clause));						// x |= y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_EQUAL_EQUALS, setting_value}, setting_clause));							// x <- y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_SHIFT_LEFT_EQUALS, setting_value}, setting_clause));						// x <<= y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_SHIFT_RIGHT_EQUALS, setting_value}, setting_clause));						// x >>= y
	listOfRules.push_back(ParseRule(110, {}, {}, {setting_value, P_KEYWORD_IS, setting_value}, setting_clause));	

	//  variable declarations
	listOfRules.push_back(ParseRule(110, {}, {}, {raw_type, P_IDENTIFIER, P_SEMI_COLON}, variable_dec));
	listOfRules.push_back(ParseRule(110, {}, {}, {raw_type, setting_value, P_SEMI_COLON}, variable_dec));
	listOfRules.push_back(ParseRule(110, {}, {}, {P_IDENTIFIER, setting_value, P_SEMI_COLON}, variable_dec));
	listOfRules.push_back(ParseRule(110, {}, {}, {raw_type, templates, P_IDENTIFIER, P_SEMI_COLON}, variable_dec));
	listOfRules.push_back(ParseRule(110, {}, {}, {raw_type, templates, setting_value, P_SEMI_COLON}, variable_dec));
	listOfRules.push_back(ParseRule(110, {}, {}, {P_IDENTIFIER, templates, setting_value, P_SEMI_COLON}, variable_dec));

	listOfRules.push_back(ParseRule(110, {}, {}, {P_KEYWORD_RETURN, setting_value, P_SEMI_COLON}, return_statement));
	listOfRules.push_back(ParseRule(110, {}, {}, {P_KEYWORD_CASE, setting_value, P_COLON}, case_statement));
	listOfRules.push_back(ParseRule(110, {}, {}, {P_KEYWORD_DEFAULT, P_COLON}, case_statement));
	
	listOfRules.push_back(ParseRule(120, {}, {}, {setting_value, P_SEMI_COLON}, statement));													// x;
	listOfRules.push_back(ParseRule(120, {}, {}, {P_KEYWORD_BREAK, P_SEMI_COLON}, break_statement));
	listOfRules.push_back(ParseRule(120, {}, {}, {P_KEYWORD_CONTINUE, P_SEMI_COLON}, continue_statement));

	listOfRules.push_back(ParseRule(130, {}, {}, {P_KEYWORD_IF, parenthesis, structure, P_KEYWORD_ELSE, structure}, if_else_statement));
	listOfRules.push_back(ParseRule(130, {}, {}, {P_KEYWORD_IF, parenthesis, structure}, if_statement));
	listOfRules.push_back(ParseRule(130, {}, {}, {P_KEYWORD_WHILE, parenthesis, structure}, while_loop));
	listOfRules.push_back(ParseRule(130, {}, {}, {P_KEYWORD_FOR, parenthesis, structure}, for_loop));
	listOfRules.push_back(ParseRule(130, {}, {}, {P_KEYWORD_DO, structure, P_KEYWORD_WHILE, parenthesis}, do_while_loop));
	listOfRules.push_back(ParseRule(130, {}, {}, {P_KEYWORD_SWITCH, parenthesis, block_of_statements}, switch_statement));
	listOfRules.push_back(ParseRule(130, {}, {}, {P_KEYWORD_TRY, block_of_statements}, try_block));
	listOfRules.push_back(ParseRule(130, {}, {}, {P_KEYWORD_CATCH, parenthesis, block_of_statements}, catch_block));
}




