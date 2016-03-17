//
//  Tokenizer.cpp
//  Sprint
//
//  Created by Thomas Redding on 10/21/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "../include/Tokenizer.hpp"

/*** PUBLIC ***/

Tokenizer::Tokenizer() {
	keywords.insert("abstract");
	keywords.insert("break");
	keywords.insert("case");
	keywords.insert("catch");
	keywords.insert("class");
	keywords.insert("const");
	keywords.insert("continue");
	keywords.insert("delete");
	keywords.insert("do");
	keywords.insert("else");
	keywords.insert("enum");
	keywords.insert("false");
	keywords.insert("for");
	keywords.insert("if");
	keywords.insert("in");
	keywords.insert("inline");
	keywords.insert("new");
	keywords.insert("NULL");
	keywords.insert("protected");
	keywords.insert("private");
	keywords.insert("ptr");
	keywords.insert("public");
	keywords.insert("ref");
	keywords.insert("return");
	keywords.insert("sizeof");
	keywords.insert("static");
	keywords.insert("struct");
	keywords.insert("switch");
	keywords.insert("this");
	keywords.insert("throw");
	keywords.insert("true");
	keywords.insert("try");
	keywords.insert("virtual");
	keywords.insert("while");
	keywords.insert("uint");
	keywords.insert("uint8");
	keywords.insert("uint16");
	keywords.insert("uint32");
	keywords.insert("uint64");
	keywords.insert("int");
	keywords.insert("int8");
	keywords.insert("int16");
	keywords.insert("int32");
	keywords.insert("int64");
	keywords.insert("and");
	keywords.insert("or");
	keywords.insert("not");
	keywords.insert("xor");
	keywords.insert("float");
	keywords.insert("double");

	punctuation.insert('.');
	punctuation.insert(':');
	punctuation.insert('+');
	punctuation.insert('-');
	punctuation.insert('*');
	punctuation.insert('/');
	punctuation.insert('&');
	punctuation.insert('#');
	punctuation.insert('<');
	punctuation.insert('=');
	punctuation.insert('>');
	punctuation.insert(',');
	punctuation.insert('|');
	punctuation.insert('%');
	punctuation.insert('!');
	punctuation.insert('^');
	punctuation.insert('?');
}

std::vector<Token> Tokenizer::process(std::string str) {
	tabs.clear();
	stack.clear();
	rtn.clear();
	
	// tokenizer each line
	std::vector<std::string> lines = split(str, '\n');
	lines.push_back("");

	for (int i=0; i<lines.size(); i++) {
		tokenizeLine(lines[i], i);
		if (!(stack.size() > 0 && stack.back() == paranthesis) && i > rtn.size() > 0 && rtn[rtn.size()-1].type != END_OF_LINE) {
			// if not in paranthese and not an empty statement, add an END_OF_LINE token
			rtn.push_back(Token(END_OF_LINE, "\n", i, lines[i].length()));
		}
		std::string out = "";
		for (int j=0; j<rtn.size(); j++)
			out += rtn[j].str + ", ";
	}

	return rtn;
}



/*** PRIVATE ***/

TokenType Tokenizer::identifierStringToEnum(std::string str) {
	if(keywords.find(str) != keywords.end()) {
		if(str == "abstract")
			return KEYWORD_ABSTRACT;
		else if(str == "break")
			return KEYWORD_BREAK;
		else if(str == "case")
			return KEYWORD_CASE;
		else if(str == "catch")
			return KEYWORD_CATCH;
		else if(str == "class")
			return KEYWORD_CLASS;
		else if(str == "const")
			return KEYWORD_CONST;
		else if(str == "continue")
			return KEYWORD_CONTINUE;
		else if(str == "delete")
			return KEYWORD_DELETE;
		else if(str == "do")
			return KEYWORD_DO;
		else if(str == "else")
			return KEYWORD_ELSE;
		else if(str == "enum")
			return KEYWORD_ENUM;
		else if(str == "false")
			return KEYWORD_FALSE;
		else if(str == "for")
			return KEYWORD_FOR;
		else if(str == "if")
			return KEYWORD_IF;
		else if(str == "in")
			return KEYWORD_IN;
		else if(str == "inline")
			return KEYWORD_INLINE;
		else if(str == "new")
			return KEYWORD_NEW;
		else if(str == "NULL")
			return KEYWORD_NULL;
		else if(str == "protected")
			return KEYWORD_PROTECTED;
		else if(str == "private")
			return KEYWORD_PRIVATE;
		else if(str == "ptr")
			return KEYWORD_PTR;
		else if(str == "public")
			return KEYWORD_PUBLIC;
		else if(str == "ref")
			return KEYWORD_REF;
		else if(str == "return")
			return KEYWORD_RETURN;
		else if(str == "sizeof")
			return KEYWORD_SIZEOF;
		else if(str == "static")
			return KEYWORD_STATIC;
		else if(str == "struct")
			return KEYWORD_STRUCT;
		else if(str == "switch")
			return KEYWORD_SWITCH;
		else if(str == "this")
			return KEYWORD_THIS;
		else if(str == "throw")
			return KEYWORD_THROW;
		else if(str == "true")
			return KEYWORD_THROW;
		else if(str == "try")
			return KEYWORD_TRY;
		else if(str == "virtual")
			return KEYWORD_VIRTUAL;
		else if(str == "while")
			return KEYWORD_WHILE;
		else if(str == "uint")
			return KEYWORD_UINT;
		else if(str == "uint8")
			return KEYWORD_UINT8;
		else if(str == "uint16")
			return KEYWORD_UINT16;
		else if(str == "uint32")
			return KEYWORD_UINT32;
		else if(str == "uint64")
			return KEYWORD_UINT64;
		else if(str == "int")
			return KEYWORD_INT;
		else if(str == "int8")
			return KEYWORD_INT8;
		else if(str == "int16")
			return KEYWORD_INT16;
		else if(str == "int32")
			return KEYWORD_INT32;
		else if(str == "int64")
			return KEYWORD_INT64;
		else if(str == "and")
			return KEYWORD_AND;
		else if(str == "or")
			return KEYWORD_OR;
		else if(str == "not")
			return KEYWORD_NOT;
		else if(str == "xor")
			return KEYWORD_XOR;
		else if(str == "float")
			return KEYWORD_FLOAT;
		else if(str == "double")
			return KEYWORD_DOUBLE;
			
	}
	return IDENTIFIER;
}

inline bool Tokenizer::isStackInComment() {
	return !stack.empty() && stack.back() == multi_line_comment;
}

inline bool Tokenizer::isIdentifierLetter(char c) {
	return isalnum(c);
}

inline bool Tokenizer::isStartOfIdentifierLetter(char c) {
	return isalnum(c);
}

void Tokenizer::popOffStack(StackState state, long lineNum, int it) {
	if (stack.size() == 0) {
		std::cout << "Tokenizer Error: a paranthesis, brackets, braces, or multi-line comment was closed without being opened (" << lineNum << " : " << it << ")\n";
		exit(1);
	}
	else {
		StackState back = stack.back();
		if (back == state)
			stack.pop_back();
		else {
			std::cout << "Tokenizer Error: a paranthesis, brackets, braces, or multi-line comment was closed without being opened (" << lineNum << " : " << it << ")\n";
			exit(1);
		}
	}
	
}

inline void Tokenizer::setToken(Token &token, TokenType type, std::string str, long lineNum, long charNum) {
	token.type = type;
	token.str = str;
	token.lineNum = lineNum;
	token.charNum = charNum;
}

std::vector<std::string> Tokenizer::split(std::string str, char delim) {
	std::vector<std::string> elems;
	std::string item;
	while(true) {
		unsigned long index = str.find(delim);
		if(index == -1) {
			elems.push_back(str);
			return elems;
		}
		if (index != 0)
			elems.push_back(str.substr(0, index));
		if(index+1 == str.length())
			return elems;
		str = str.substr(index+1);
	}
}

inline void Tokenizer::storeAndReset(Token &token, long lineNum, long charNum) {
	rtn.push_back(token);
	setToken(token, UNKNOWN, "", lineNum, charNum);
}

void Tokenizer::tokenizeLine(std::string str, long lineNum) {
	int it = 0;
	if(str.length() == 0) {
		if (isStackInComment()) {
			// do nothing
		}
		else {
			if (tabs.size() != 0 && tabs.back() > 0) {
				for (int i=0; i<tabs.back(); i++) {
					rtn.push_back(Token(END_OF_LINE, "\n", lineNum, 0));
					rtn.push_back(Token(END_OF_BLOCK, "", lineNum, 0));
				}
			}
			tabs.push_back(0);
		}
		return;
	}
	
	if (isStackInComment()) {
		// If we're currently in a multi-line comment, search for the end of that comment
		for (it=it; it<str.length(); it++) {
			if (str[it] == '*' && it+1 < str.length() && str[it+1] == '/') {
				it += 2;
				popOffStack(multi_line_comment, lineNum, it);
				break;
			}
		}
	}
	
	// Count indents
	int startIt = it;
	for (it=it; it<str.length(); it++) {
		if (str[it] != '\t') {
			if (lineNum != 0) {
				if (it > tabs.back()) {
					for (int i=0; i<it-tabs.back(); i++) {
						rtn.push_back(Token(END_OF_LINE, "\n", lineNum, 0));
						rtn.push_back(Token(START_OF_BLOCK, "\t", lineNum, 0));
					}
				}
				else if (it < tabs.back()) {
					for (int i=0; i<tabs.back()-it; i++) {
						rtn.push_back(Token(END_OF_LINE, "\n", lineNum, 0));
						rtn.push_back(Token(END_OF_BLOCK, "\t", lineNum, 0));
					}
				}
			}
			tabs.push_back(it-startIt);
			break;
		}
	}

	/* parse remaidner of line using a modified finite state machine */
	Token token = Token(UNKNOWN, "", lineNum, it);
	for (it=it; it<str.length(); it++) {
		if (isStackInComment()) {
			if (str[it] == '*' && it+1 < str.length() && str[it+1] == '/') {
				stack.pop_back();
			}
		}
		else if (token.type == UNKNOWN) {
			if(isdigit(str[it]))
				setToken(token, INTEGER_LITERAL, str.substr(it, 1), lineNum, it);
			else if(str[it] == '\"')
				setToken(token, STRING_LITERAL, "\"", lineNum, it);
			else if(str[it] == '\'')
				setToken(token, CHARACTER_LITERAL, "'", lineNum, it);
			else if(isStartOfIdentifierLetter(str[it]))
				setToken(token, IDENTIFIER, str.substr(it, 1), lineNum, it);
			else if(str[it] == '/' && it+1 < str.length() && str[it+1] == '/') {
				return; // reached end of line (rest is a comment)
			}
			else if(str[it] == '/' && it+1 < str.length() && str[it+1] == '*') {
				stack.push_back(multi_line_comment);
			}
			else if(str[it] == ';') {
				setToken(token, END_OF_LINE, ";", lineNum, it);
				storeAndReset(token, lineNum, it);
			}
			else if(str[it] == '{') {
				setToken(token, OPEN_CURLY_BRACE, "{", lineNum, it);
				stack.push_back(curly_bracket);
				storeAndReset(token, lineNum, it);
			}
			else if(str[it] == '}') {
				setToken(token, CLOSE_CURLY_BRACE, "}", lineNum, it);
				popOffStack(curly_bracket, lineNum, it);
				storeAndReset(token, lineNum, it);
			}
			else if(str[it] == '(') {
				setToken(token, OPEN_PARENTHESIS, "(", lineNum, it);
				stack.push_back(paranthesis);
				storeAndReset(token, lineNum, it);
			}
			else if(str[it] == ')') {
				setToken(token, CLOSE_PARENTHESIS, ")", lineNum, it);
				popOffStack(paranthesis, lineNum, it);
				storeAndReset(token, lineNum, it);
			}
			else if(str[it] == '[') {
				setToken(token, OPEN_BRACKET, "[", lineNum, it);
				stack.push_back(square_brace);
				storeAndReset(token, lineNum, it);
			}
			else if(str[it] == ']') {
				setToken(token, CLOSE_BRACKET, "]", lineNum, it);
				popOffStack(square_brace, lineNum, it);
				storeAndReset(token, lineNum, it);
			}
			else if (punctuation.find(str[it]) != punctuation.end()) {
				setToken(token, PUNCTUATION, str.substr(it, 1), lineNum, it);
			}
			else if (str[it] == ' ') {
				// ignore
			}
			else {
				std::cout << "Tokenizer Error: unrecognized character (" << lineNum << " : " << it << ")\n";
				exit(1);
			}
		}
		else if (token.type == INTEGER_LITERAL) {
			if(isdigit(str[it]))
				token.str += str[it];
			else if(str[it] == '.' || str[it] == 'e' || str[it] == 'E') {
				token.type = FLOAT_LITERAL;
				token.str += str[it];
			}
			else {
				storeAndReset(token, lineNum, it);
				it--;
			}
		}
		else if (token.type == FLOAT_LITERAL) {
			if(isdigit(str[it]) || str[it] == '.' || str[it] == 'e' || str[it] == 'E')
				token.str += str[it];
			else {
				storeAndReset(token, lineNum, it);
				it--;
			}
		}
		else if (token.type == CHARACTER_LITERAL) {
			if(str[it] == '\'' && str[it-1] != '\\') {
				storeAndReset(token, lineNum, it);
			}
			else
				token.str += str[it];
		}
		else if (token.type == IDENTIFIER) {
			if(isIdentifierLetter(str[it]))
				token.str += str[it];
			else {
				token.type = identifierStringToEnum(token.str);
				storeAndReset(token, lineNum, it);
				it--;
			}
		}
		else if (token.type == PUNCTUATION) {
			if (punctuation.find(str[it]) != punctuation.end() || str[it] == ' ') {
				token.str += str[it];
			}
			else {
				// end of series of punctuation marks
				std::vector<std::string> arr = split(token.str, ' ');
				setToken(token, UNKNOWN, "", lineNum, it);
				bool isFirstObject = true; // TODO
				bool isLastObject = true; // TODO
				for (int i=0; i<arr.size(); i++) {
					bool first = false;
					bool last = false;
					if (i == 0)
						first = isFirstObject;
					if (i == arr.size()-1)
						last = isLastObject;
					parseSpacelessTokens(arr[i], lineNum, it, first, last);
				}
				it--;
			}
		}
		else {
			std::cout << "Tokenizer Erro: unkown error #2 (" << lineNum << " : " << it << ")\n";
			exit(1);
		}
	}
	if (token.type != UNKNOWN) {
		storeAndReset(token, -1, -1);
	}
}

void Tokenizer::parseSpacelessTokens(std::string str, long lineNum, int it, bool isPreviousObject, bool isNextObject) {
	std::vector<std::string> arr = equalitySplit(str);
	for (int i=0; i<arr.size(); i++) {
		bool first = false;
		bool last = false;
		if (i == 0)
			first = isPreviousObject;
		if (i == arr.size()-1)
			last = isNextObject;
		parseSpacelessAndEquallessTokens(arr[i], lineNum, it, first, last);
	}
}

void Tokenizer::parseSpacelessAndEquallessTokens(std::string str, long lineNum, int it, bool isPreviousObject, bool isNextObject) {
	/*
	 * Here's what we know about str.
	 * 1. there are no spaces
	 * 2. all '=', '<', '>' are at the end (if there are any)
	 */
	if (str.length() == 0) {
		std::cout << "Tokenizer Error: unkown punctuation issue" << " (" << lineNum << " : " << it << ")\n";
		exit(1);
	}
	else if (str.length() == 1) {
		Token token;
		setToken(token, UNKNOWN, str, lineNum, it);
		if (str == ".")
			token.type = PERIOD;
		else if (str == ":")
			token.type = COLON;
		else if (str == "+")
			token.type = PLUS;
		else if (str == "-")
			token.type = MINUS;
		else if (str == "*")
			token.type = ASTERISK;
		else if (str == "/")
			token.type = SLASH;
		else if (str == "&")
			token.type = AMPERSAND;
		else if (str == "#")
			token.type = POUND_SIGN;
		else if (str == "<")
			token.type = LESS_THAN;
		else if (str == "=")
			token.type = EQUALS;
		else if (str == ">")
			token.type = GREATER_THAN;
		else if (str == ",")
			token.type = COMMA;
		else if (str == "|")
			token.type = VERTICAL_BAR;
		else if (str == "%")
			token.type = PERCENT;
		else if (str == "!")
			token.type = EXCLAMATION_POINT;
		else if (str == "^")
			token.type = CARROT;
		else if (str == "?")
			token.type = QUESTION_MARK;
		else {
			std::cout << "Tokenizer Error: unkown punctuation issue: " << str << " (" << lineNum << " : " << it << ")\n";
			exit(1);
		}
		storeAndReset(token, lineNum, it);
	}
	else if (str.length() == 2) {
		if (str[1] == '=') {
			Token token;
			setToken(token, UNKNOWN, str, lineNum, it);
			if (str[0] == '+')
				token.type = PLUS_EQUALS;
			else if (str[0] == '-')
				token.type = MINUS_EQUALS;
			else if (str[0] == '/')
				token.type = SLASH_EQUALS;
			else if (str[0] == '*')
				token.type = ASTERISK_EQUALS;
			else if (str[0] == '&')
				token.type = AMPERSAND_EQUALS;
			else if (str[0] == '^')
				token.type = CARROT_EQUALS;
			else if (str[0] == '|')
				token.type = VERTICAL_BAR_EQUALS;
			else if (str[0] == '>')
				token.type = GREATER_THAN_EQUALS;
			else if (str[0] == '<')
				token.type = LESS_THAN_EQUALS;
			else {
				std::cout << "Tokenizer Error: invalid punctuation before '=' sign: " << str << " (" << lineNum << " : " << it << ")\n";
				exit(1);
			}
			storeAndReset(token, lineNum, it);
		}
		else if (str == "<<") {
			Token token;
			setToken(token, SHIFT_LEFT, str, lineNum, it);
			storeAndReset(token, lineNum, it);
		}
		else if (str == ">>") {
			Token token;
			setToken(token, SHIFT_LEFT, str, lineNum, it);
			storeAndReset(token, lineNum, it);
		}
		else if (str == "++") {
			Token token;
			setToken(token, UNKNOWN, str, lineNum, it);
			if ((isPreviousObject && !isNextObject) || (!isPreviousObject && isNextObject)) {
				// x++ or ++x
				token.type = PLUS_PLUS;
				storeAndReset(token, lineNum, it);
			}
			else {
				std::cout << "Tokenizer Error: invalid use of ++ (" << lineNum << " : " << it << ")\n";
				exit(1);
			}
		}
		else if (str == "+-") {
			Token token;
			setToken(token, PLUS, "+", lineNum, it);
			storeAndReset(token, lineNum, it);
			setToken(token, MINUS, "-", lineNum, it);
			storeAndReset(token, lineNum, it);
		}
		else if (str == "--") {
			Token token;
			setToken(token, UNKNOWN, str, lineNum, it);
			if ((isPreviousObject && !isNextObject) || (!isPreviousObject && isNextObject)) {
				// x++ or ++x
				token.type = MINUS_MINUS;
				storeAndReset(token, lineNum, it);
			}
			else if (isPreviousObject && isNextObject) {
				// x--y
				token.str = "-";
				token.type = MINUS;
				storeAndReset(token, lineNum, it);
				token.str = "-";
				token.type = MINUS;
				storeAndReset(token, lineNum, it);
			}
			else {
				std::cout << "Tokenizer Error: invalid use of -- (" << lineNum << " : " << it << ")\n";
				exit(1);
			}
		}
		else {
			std::cout << "Tokenizer Error: invalid string of two punctuation symbols: " << str << " (" << lineNum << " : " << it << ")\n";
			exit(1);
		}
	}
	else if (str.length() == 3) {
		if (str == ">>=") {
			Token token;
			setToken(token, SHIFT_RIGHT_EQUALS, str, lineNum, it);
			storeAndReset(token, lineNum, it);
		}
		else if (str == "<<=") {
			Token token;
			setToken(token, SHIFT_LEFT_EQUALS, str, lineNum, it);
			storeAndReset(token, lineNum, it);
		}
		else {
			std::cout << "Tokenizer Error: too many punctuation symbols (may be fixed in later versions): " << str << "(" << lineNum << " : " << it << ")\n";
			exit(1);
		}
	}
	else {
		std::cout << "Tokenizer Error: too many punctuation symbols (may be fixed in later versions): " << str << "(" << lineNum << " : " << it << ")\n";
		exit(1);
	}
}

std::vector<std::string> Tokenizer::equalitySplit(std::string str) {
	std::vector<std::string> rtn;
	int lastEqual = -1;
	for (int i=0; i<str.length()-1; i++) {
		if (str[i] == '=' || str[i] == '<' || str[i] == '>') {
			if (str[i+1] == '=' || str[i+1] == '<' || str[i+1] == '>') {
				rtn.push_back(str.substr(lastEqual+1, i-lastEqual));
				lastEqual = i;
			}
		}
	}
	rtn.push_back(str.substr(lastEqual+1));
	return rtn;
}


