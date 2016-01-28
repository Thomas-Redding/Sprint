//
//  Tokenizer.cpp
//  Sprint
//
//  Created by Thomas Redding on 10/21/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "Tokenizer.hpp"

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
}

std::vector<Token> Tokenizer::process(std::string str) {
	tabs.clear();
	stack.clear();
	rtn.clear();
	
	// tokenizer each line
	std::vector<std::string> lines = split(str, '\n');
	for (int i=0; i<lines.size(); i++) {
		tokenizeLine(lines[i], i);
		if (!(stack.size() > 0 && stack.back() == paranthesis)) {
			rtn.push_back(Token(NEWLINE, "\n", i, lines[i].length()));
		}
	}
	
	// remove excessive new lines
	for (int i=1; i<rtn.size(); i++) {
		if (rtn[i-1].type == NEWLINE && rtn[i].type == NEWLINE) {
			rtn.erase(rtn.begin()+i);
			i--;
		}
	}
	return rtn;
}

void Tokenizer::tokenizeLine(std::string str, long lineNum) {
	int it = 0;
	
	if(str.length() == 0) {
		if (inComment()) {
			// do nothing
		}
		else {
			if (tabs.size() != 0 && tabs.back() > 0) {
				for (int i=0; i<tabs.back(); i++) {
					rtn.push_back(Token(DEDENT, "", lineNum, 0));
				}
			}
			tabs.push_back(0);
		}
		return;
	}
	
	if (inComment()) {
		// If we're currently in a multi-line comment, search for the end of that comment
		for (it=it; it<str.length(); it++) {
			if (str[it] == '*' && it+1 < str.length() && str[it+1] == '/') {
				it += 2;
				popOffStack(multi_line_comment);
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
						rtn.push_back(Token(INDENT, "\t", lineNum, 0));
					}
				}
				else if (it < tabs.back()) {
					for (int i=0; i<tabs.back()-it; i++) {
						rtn.push_back(Token(DEDENT, "\t", lineNum, 0));
					}
				}
			}
			tabs.push_back(it-startIt);
			break;
		}
	}
	
	Token token = Token(UNKNOWN, "", lineNum, it);
	for (it=it; it<str.length(); it++) {
		
		if (inComment()) {
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
				return;
			}
			else if(str[it] == '/' && it+1 < str.length() && str[it+1] == '*') {
				stack.push_back(multi_line_comment);
			}
			else if(str[it] == ' ') {
				// do nothing
			}
			else {
				// single-character tokens
				if(str[it] == ';')
					setToken(token, SEMI_COLON, ";", lineNum, it);
				else if(str[it] == '{') {
					setToken(token, OPEN_CURLY_BRACE, "{", lineNum, it);
					stack.push_back(curly_bracket);
				}
				else if(str[it] == '}') {
					setToken(token, CLOSE_CURLY_BRACE, "}", lineNum, it);
					popOffStack(curly_bracket);
				}
				else if(str[it] == '(') {
					setToken(token, OPEN_PARENTHESIS, "(", lineNum, it);
					stack.push_back(paranthesis);
				}
				else if(str[it] == ')') {
					setToken(token, CLOSE_PARENTHESIS, ")", lineNum, it);
					popOffStack(paranthesis);
				}
				else if(str[it] == '[') {
					setToken(token, OPEN_BRACKET, "[", lineNum, it);
					stack.push_back(square_brace);
				}
				else if(str[it] == ']') {
					setToken(token, CLOSE_BRACKET, "]", lineNum, it);
					popOffStack(square_brace);
				}
				else if(str[it] == '.')
					setToken(token, PERIOD, ".", lineNum, it);
				else if(str[it] == ':')
					setToken(token, COLON, ":", lineNum, it);
				else if(str[it] == '+')
					setToken(token, PLUS, "+", lineNum, it);
				else if(str[it] == '-')
					setToken(token, MINUS, "-", lineNum, it);
				else if(str[it] == '*')
					setToken(token, ASTERISK, "*", lineNum, it);
				else if(str[it] == '/')
					setToken(token, SLASH, "/", lineNum, it);
				else if(str[it] == '&')
					setToken(token, AMPERSAND, "&", lineNum, it);
				else if(str[it] == '#')
					setToken(token, POUND_SIGN, "#", lineNum, it);
				else if(str[it] == '<')
					setToken(token, LESS_THAN, "<", lineNum, it);
				else if(str[it] == '=')
					setToken(token, EQUALS, "=", lineNum, it);
				else if(str[it] == '>')
					setToken(token, GREATER_THAN, ">", lineNum, it);
				else if(str[it] == ',')
					setToken(token, COMMA, ",", lineNum, it);
				else if(str[it] == '|')
					setToken(token, VERTICAL_BAR, "|", lineNum, it);
				else if(str[it] == '%')
					setToken(token, PERCENT, "%", lineNum, it);
				else if(str[it] == '!')
					setToken(token, EXCLAMATION_POINT, "!", lineNum, it);
				else if(str[it] == '^')
					setToken(token, CARROT, "^", lineNum, it);
				else if(str[it] == '?')
					setToken(token, QUESTION_MARK, "?", lineNum, it);
				else {
					std::cout << "TOKERNIZER ERROR #1 (" << str[it] << ")\n";
				}
				storeAndReset(token, lineNum, it);
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
		else {
			std::cout << "TOKERNIZER ERROR #2\n";
		}
	}
	if (token.type != UNKNOWN) {
		storeAndReset(token, -1, -1);
	}
}

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

inline void Tokenizer::popOffStack(StackState state) {
	if (stack.size() == 0) {
		std::cout << "TOKERNIZER ERROR #3\n";
		exit(1);
	}
	else {
		StackState back = stack.back();
		if (back == state)
			stack.pop_back();
		else
			std::cout << "TOKERNIZER ERROR #4\n";
	}
	
}

inline void Tokenizer::storeAndReset(Token &token, long lineNum, long charNum) {
	rtn.push_back(token);
	setToken(token, UNKNOWN, "", lineNum, charNum);
}

inline void Tokenizer::setToken(Token &token, TokenType type, std::string str, long lineNum, long charNum) {
	token.type = type;
	token.str = str;
	token.lineNum = lineNum;
	token.charNum = charNum;
}

inline bool Tokenizer::isStartOfIdentifierLetter(char c) {
	return isalnum(c);
}

inline bool Tokenizer::isIdentifierLetter(char c) {
	return isalnum(c);
}

inline bool Tokenizer::inComment() {
	return !stack.empty() && stack.back() == multi_line_comment;
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
		elems.push_back(str.substr(0, index));
		if(index+1 == str.length())
			return elems;
		str = str.substr(index+1);
	}
}