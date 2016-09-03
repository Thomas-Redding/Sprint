#include "../include/ThomasParser.hpp"


void addParseRules(std::vector<bool> &leftToRight, std::vector<ThomasParseRule> &listOfRules) {
	leftToRight= {
		// TODO: document what 'true' and 'false' mean... or better yet, use an enum
		true, false, true, true, true, true, true, true, true, true, false, true, true, false, true
	};

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

	// class implementations
	listOfRules.push_back(ThomasParseRule(140, general, {T_KEYWORD_CLASS, T_IDENTIFIER, curly_brace_block}, class_implementation));					// class Foo { ... 
	listOfRules.push_back(ThomasParseRule(140, general, {T_KEYWORD_CLASS, T_IDENTIFIER, template_block, curly_brace_block}, class_implementation));	// class Foo <T> { ... 
	listOfRules.push_back(ThomasParseRule(140, general, {
		T_KEYWORD_CLASS, T_IDENTIFIER, T_COLON, T_IDENTIFIER, curly_brace_block												// class Foo<T> : Bar
	}, class_implementation));
	listOfRules.push_back(ThomasParseRule(140, general, {
		T_KEYWORD_CLASS, T_IDENTIFIER, template_block, T_COLON, T_IDENTIFIER, curly_brace_block												// class Foo<T> : Bar
	}, class_implementation));
	listOfRules.push_back(ThomasParseRule(140, general, {
		T_KEYWORD_CLASS, T_IDENTIFIER, T_COLON, T_IDENTIFIER, template_block, curly_brace_block								// class Foo: Bar<S>
	}, class_implementation));
	listOfRules.push_back(ThomasParseRule(140, general, {
		T_KEYWORD_CLASS, T_IDENTIFIER, template_block, T_COLON, T_IDENTIFIER, template_block, curly_brace_block								// class Foo<T, S> : Bar<T>
	}, class_implementation));

	// function implementations
	listOfRules.push_back(ThomasParseRule(140, general, {
		T_IDENTIFIER, parenthesis_block, T_ARROW, raw_type, curly_brace_block
	}, function_implementation));
	listOfRules.push_back(ThomasParseRule(140, general, {
		T_IDENTIFIER, template_block, parenthesis_block, T_ARROW, raw_type, curly_brace_block
	}, function_implementation));
	listOfRules.push_back(ThomasParseRule(140, general, {
		T_IDENTIFIER, parenthesis_block, T_ARROW, raw_type, template_block, curly_brace_block
	}, function_implementation));
	listOfRules.push_back(ThomasParseRule(140, general, {
		T_IDENTIFIER, template_block, parenthesis_block, T_ARROW, raw_type, template_block, curly_brace_block
	}, function_implementation)); // TODO: why doesn't this work

	listOfRules.push_back(ThomasParseRule(140, general, {T_KEYWORD_ENUM, T_IDENTIFIER, curly_brace_block}, enum_implementation));					// enum Foo { ... 
	listOfRules.push_back(ThomasParseRule(140, general, {T_KEYWORD_NAMESPACE, T_IDENTIFIER, curly_brace_block}, namespace_implementation));			// namespaces Foo { ... 
}




