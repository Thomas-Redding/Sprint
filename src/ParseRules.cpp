#include "../include/ThomasParser.hpp"


void addParseRules(std::vector<bool> &leftToRight, std::vector<ThomasParseRule> &listOfRules) {
	leftToRight= {
		// TODO: document what 'true' and 'false' mean... or better yet, use an enum
		// -10, 0 , 10   , 20  , 30  , 40  , 50  , 60  , 70  , 80  , 90  , 100 , 105 , 110 , 120  , 130
		true, true, false, true, true, true, true, true, true, true, true, true, true, false, false, false
	};

	listOfRules.push_back(ThomasParseRule(  0, {}, {}, {unary1_value, T_PLUS_PLUS}, unary1_clause));													// x++
	listOfRules.push_back(ThomasParseRule(  0, {}, {}, {unary1_value, T_MINUS_MINUS}, unary1_clause));													// x--
	listOfRules.push_back(ThomasParseRule(  0, {}, {function_declaration}, {unary1_value, parenthesis_block}, unary1_clause));												// x(y)
	listOfRules.push_back(ThomasParseRule(  0, {}, {}, {unary1_value, bracket_block}, unary1_clause));													// x[y]
	listOfRules.push_back(ThomasParseRule(  0, {}, {}, {unary1_value, T_PERIOD}, unary1_clause));														// x.y

	// false
	listOfRules.push_back(ThomasParseRule( 10, {}, {}, {T_POSITIVE, unary2_value}, unary2_clause));													// +x
	listOfRules.push_back(ThomasParseRule( 10, {}, {}, {T_NEGATIVE, unary2_value}, unary2_clause));													// -x
	listOfRules.push_back(ThomasParseRule( 10, {}, {}, {T_PLUS_PLUS, unary2_value}, unary2_clause));													// ++x
	listOfRules.push_back(ThomasParseRule( 10, {}, {}, {T_MINUS_MINUS, unary2_value}, unary2_clause));												// --x
	listOfRules.push_back(ThomasParseRule( 10, {}, {}, {T_TILDE, unary2_value}, unary2_clause));														// ~x
	listOfRules.push_back(ThomasParseRule( 10, {}, {}, {T_KEYWORD_NOT, unary2_value}, unary2_clause));												// not x
	listOfRules.push_back(ThomasParseRule( 10, {}, {}, {T_KEYWORD_NEW, unary2_value}, unary2_clause));												// new x

	// true
	listOfRules.push_back(ThomasParseRule( 20, {}, {}, {mult_value, T_ASTERISK, mult_value}, mult_clause));											// x * y
	listOfRules.push_back(ThomasParseRule( 20, {}, {}, {mult_value, T_SLASH, mult_value}, mult_clause));											// x / y
	listOfRules.push_back(ThomasParseRule( 20, {}, {}, {mult_value, T_PERCENT, mult_value}, mult_clause));											// x % y

	// true
	listOfRules.push_back(ThomasParseRule( 30, {}, {}, {plus_value, T_PLUS, plus_value}, plus_clause));												// x + y
	listOfRules.push_back(ThomasParseRule( 30, {}, {}, {plus_value, T_MINUS, plus_value}, plus_clause));											// x - y

	// true
	listOfRules.push_back(ThomasParseRule( 40, {}, {}, {shift_value, T_SHIFT_LEFT, shift_value}, shift_clause));									// x << y
	listOfRules.push_back(ThomasParseRule( 40, {}, {}, {shift_value, T_SHIFT_RIGHT, shift_value}, shift_clause));									// x >> y

	listOfRules.push_back(ThomasParseRule( 50, {}, {}, {inequality_value, T_LESS_THAN, inequality_value}, inequality_clause));						// x < y
	listOfRules.push_back(ThomasParseRule( 50, {}, {}, {inequality_value, T_GREATER_THAN, inequality_value}, inequality_clause));					// x > y
	listOfRules.push_back(ThomasParseRule( 50, {}, {}, {inequality_value, T_LESS_THAN_EQUALS, inequality_value}, inequality_clause));				// x <= y
	listOfRules.push_back(ThomasParseRule( 50, {}, {}, {inequality_value, T_GREATER_THAN_EQUALS, inequality_value}, inequality_clause));			// x >= y

	listOfRules.push_back(ThomasParseRule( 60, {}, {}, {equality_value, T_EQUAL_EQUALS, equality_value}, equality_clause));							// x == y
	listOfRules.push_back(ThomasParseRule( 60, {}, {}, {equality_value, T_EXCLAMATION_POINT_EQUALS, equality_value}, equality_clause));				// x != y
	listOfRules.push_back(ThomasParseRule( 60, {}, {}, {equality_value, T_EQUAL_EQUAL_EQUALS, equality_value}, equality_clause));					// x === y
	listOfRules.push_back(ThomasParseRule( 60, {}, {}, {equality_value, T_EXCLAMATION_POINT_EQUAL_EQUALS, equality_value}, equality_clause));		// x !== y

	listOfRules.push_back(ThomasParseRule( 70, {}, {}, {bitwise_and_value, T_KEYWORD_AND, bitwise_and_value}, bitwise_and_clause));					// x & y

	listOfRules.push_back(ThomasParseRule( 80, {}, {}, {bitwise_xor_value, T_KEYWORD_XOR, bitwise_xor_value}, bitwise_xor_clause));					// x ^ y

	listOfRules.push_back(ThomasParseRule( 90, {}, {}, {bitwise_or_value, T_KEYWORD_OR, bitwise_or_value}, bitwise_or_clause));						// x | y

	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_QUESTION_MARK, setting_value, T_COLON, setting_value}, ternary_clause));	// x ? y : z
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_EQUALS, setting_value}, setting_clause));									// x = y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_PLUS_EQUALS, setting_value}, setting_clause));								// x += y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_MINUS_EQUALS, setting_value}, setting_clause));							// x -= y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_ASTERISK_EQUALS, setting_value}, setting_clause));							// x *= y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_SLASH_EQUALS, setting_value}, setting_clause));							// x /= y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_PERCENT_EQUALS, setting_value}, setting_clause));							// x %= y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_AMPERSAND_EQUALS, setting_value}, setting_clause));						// x &= y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_CARROT_EQUALS, setting_value}, setting_clause));							// x ^= y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_VERTICAL_BAR_EQUALS, setting_value}, setting_clause));						// x |= y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_LEFT_ARROW, setting_value}, setting_clause));								// x <- y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_SHIFT_LEFT_EQUALS, setting_value}, setting_clause));						// x <<= y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_SHIFT_RIGHT_EQUALS, setting_value}, setting_clause));						// x >>= y
	listOfRules.push_back(ThomasParseRule(100, {}, {}, {setting_value, T_KEYWORD_IS, setting_value}, setting_clause));								// x is y

	listOfRules.push_back(ThomasParseRule(105, {}, {}, {colon_value, T_COLON, colon_value}, colon_clause));	// x : z

	listOfRules.push_back(ThomasParseRule(110, {}, {}, {raw_type, T_IDENTIFIER, T_SEMI_COLON}, variable_dec));
	listOfRules.push_back(ThomasParseRule(110, {}, {}, {raw_type, template_block, T_IDENTIFIER, T_SEMI_COLON}, variable_dec));
	listOfRules.push_back(ThomasParseRule(110, {}, {}, {raw_type, setting_value, T_SEMI_COLON}, variable_dec));
	listOfRules.push_back(ThomasParseRule(110, {}, {}, {raw_type, template_block, setting_value, T_SEMI_COLON}, variable_dec));
	listOfRules.push_back(ThomasParseRule(110, {}, {}, {T_KEYWORD_RETURN, setting_value, T_SEMI_COLON}, return_statement));
	listOfRules.push_back(ThomasParseRule(110, {}, {}, {comma_value, T_COMMA, comma_value}, comma_clause));											// x, y
	listOfRules.push_back(ThomasParseRule(110, {}, {}, {T_KEYWORD_CASE, setting_value, T_COLON}, case_statement));
	listOfRules.push_back(ThomasParseRule(110, {}, {}, {T_KEYWORD_DEFAULT, T_COLON}, case_statement));
	
	listOfRules.push_back(ThomasParseRule(120, {}, {}, {comma_value, T_SEMI_COLON}, statement));													// x;
	listOfRules.push_back(ThomasParseRule(120, {}, {}, {T_KEYWORD_BREAK, T_SEMI_COLON}, break_statement));
	listOfRules.push_back(ThomasParseRule(120, {}, {}, {T_KEYWORD_CONTINUE, T_SEMI_COLON}, continue_statement));

	listOfRules.push_back(ThomasParseRule(130, {}, {}, {T_KEYWORD_IF, parenthesis_block, structure, T_KEYWORD_ELSE, structure}, if_else_statement));
	listOfRules.push_back(ThomasParseRule(130, {}, {}, {T_KEYWORD_IF, parenthesis_block, structure}, if_statement));
	listOfRules.push_back(ThomasParseRule(130, {}, {}, {T_KEYWORD_WHILE, parenthesis_block, structure}, while_loop));
	listOfRules.push_back(ThomasParseRule(130, {}, {}, {T_KEYWORD_FOR, parenthesis_block, structure}, for_loop));
	listOfRules.push_back(ThomasParseRule(130, {}, {}, {T_KEYWORD_DO, structure, T_KEYWORD_WHILE, parenthesis_block}, do_while_loop));
	listOfRules.push_back(ThomasParseRule(130, {}, {}, {T_KEYWORD_SWITCH, parenthesis_block, curly_brace_block}, switch_statement));
	listOfRules.push_back(ThomasParseRule(130, {}, {}, {T_KEYWORD_TRY, curly_brace_block}, try_block));
	listOfRules.push_back(ThomasParseRule(130, {}, {}, {T_KEYWORD_CATCH, parenthesis_block, curly_brace_block}, catch_block));

	// class implementations
	// class Foo { ... 
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_KEYWORD_CLASS,
		T_IDENTIFIER,
		curly_brace_block
	}, class_implementation));
	// class Foo <T> { ... 
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_KEYWORD_CLASS,
		T_IDENTIFIER,
		template_block,
		curly_brace_block
	},class_implementation));
	// class Foo<T> : Bar
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_KEYWORD_CLASS,
		T_IDENTIFIER,
		T_COLON,
		T_IDENTIFIER,
		curly_brace_block
	}, class_implementation));
	// class Foo<T> : Bar<U>
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_KEYWORD_CLASS,
		T_IDENTIFIER,
		template_block,
		T_COLON,
		T_IDENTIFIER,
		template_block,
		curly_brace_block
	}, class_implementation));
	// class Foo: Bar<S>
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_KEYWORD_CLASS,
		T_IDENTIFIER,
		T_COLON,
		T_IDENTIFIER,
		template_block,
		curly_brace_block
	}, class_implementation));
	// class Foo<T, S> : Bar<T>
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_KEYWORD_CLASS,
		T_IDENTIFIER,
		template_block,
		T_EXTENDS,
		T_IDENTIFIER,
		template_block,
		curly_brace_block								
	}, class_implementation));

	// function implementations
	// foo(...) -> int {...}
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER,
		parenthesis_block,
		T_ARROW,
		raw_type_or_void,
		curly_brace_block
	}, function_implementation));
	// foo<...>(...) -> int {...}
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER,
		template_block,
		parenthesis_block,
		T_ARROW,
		raw_type_or_void,
		curly_brace_block
	}, function_implementation));
	// foo(...) -> Foo {...}
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER,
		parenthesis_block,
		T_ARROW,
		T_IDENTIFIER,
		curly_brace_block
	}, function_implementation));
	// foo<...>(...) -> Foo {...}
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER,
		template_block,
		parenthesis_block,
		T_ARROW,
		T_IDENTIFIER,
		curly_brace_block
	}, function_implementation));
	// foo<...>(...) -> Foo<...> {...}
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER,
		template_block,
		parenthesis_block,
		T_ARROW,
		T_IDENTIFIER,
		template_block,
		curly_brace_block
	}, function_implementation));

	
	// function declaration
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER,
		parenthesis_block,
		T_ARROW,
		raw_type_or_void,
		T_SEMI_COLON
	}, function_declaration));
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER, template_block, parenthesis_block, T_ARROW, raw_type_or_void, T_SEMI_COLON
	}, function_declaration));
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER, parenthesis_block, T_ARROW, T_IDENTIFIER, T_SEMI_COLON
	}, function_declaration));
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER, template_block, parenthesis_block, T_ARROW, T_IDENTIFIER, T_SEMI_COLON
	}, function_declaration));
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		T_IDENTIFIER, template_block, parenthesis_block, T_ARROW, T_IDENTIFIER, template_block, T_SEMI_COLON
	}, function_declaration));

	// operator-function declarations
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		parenthesis_block,
		parenthesis_block,
		T_ARROW,
		raw_type_or_void,
		curly_brace_block
	}, function_declaration));
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {
		bracket_block,
		parenthesis_block,
		T_ARROW,
		raw_type_or_void,
		curly_brace_block
	}, function_declaration));

	// enum Foo {...}
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {T_KEYWORD_ENUM, T_IDENTIFIER, curly_brace_block}, enum_implementation));
	// namespaces Foo {...}
	listOfRules.push_back(ThomasParseRule(-10, {}, {}, {T_KEYWORD_NAMESPACE, T_IDENTIFIER, curly_brace_block}, namespace_implementation));
}




