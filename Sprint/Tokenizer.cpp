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
	std::vector<std::string> lines = split(str, '\n');
	std::vector<Token> rtn;
	tabs.clear();
	stack.clear();
	rtn.clear();
	for (int i=0; i<lines.size(); i++) {
		tokenizeLine(lines[i], i);
	}
	return rtn;
}

std::string Tokenizer::tokenTypeToString(TokenType t) {
	if( t == INDENT) {
		return "INDENT";
	}
	else if( t == DEDENT) {
		return "DEDENT";
	}
	else if( t == NEWLINE) {
		return "NEWLINE";
	}
	else if( t == KEYWORD) {
		return "KEYWORD";
	}
	else if( t == IDENTIFIER) {
		return "IDENTIFIER";
	}
	else if( t == INTEGER_LITERAL) {
		return "INTEGER_LITERAL";
	}
	else if( t == FLOAT_LITERAL) {
		return "FLOAT_LITERAL";
	}
	else if( t == CHARACTER_LITERAL) {
		return "CHARACTER_LITERAL";
	}
	else if( t == STRING_LITERAL) {
		return "STRING_LITERAL";
	}
	else if( t == PUNCTUATION) {
		return "PUNCTUATION";
	}
	else if( t == BRACKET) {
		return "BRACKET";
	}
	else if( t == UNKNOWN) {
		return "UNKNOWN";
	}
	else if( t == NULL_TOKEN_TYPE) {
		return "NULL_TOKEN_TYPE";
	}
	else if( t == OPEN_PARENTHESIS) {
		return "OPEN_PARENTHESIS";
	}
	else if( t == CLOSE_PARENTHESIS) {
		return "CLOSE_PARENTHESIS";
	}
	else if( t == OPEN_BRACKET) {
		return "OPEN_BRACKET";
	}
	else if( t == CLOSE_BRACKET) {
		return "CLOSE_BRACKET";
	}
	else if( t == OPEN_CURLY_BRACE) {
		return "OPEN_CURLY_BRACE";
	}
	else if( t == CLOSE_CURLY_BRACE) {
		return "CLOSE_CURLY_BRACE";
	}
	else if( t == KEYWORD_ABSTRACT) {
		return "KEYWORD_ABSTRACT";
	}
	else if( t == KEYWORD_BREAK) {
		return "KEYWORD_BREAK";
	}
	else if( t == KEYWORD_CASE) {
		return "KEYWORD_CASE";
	}
	else if( t == KEYWORD_CATCH) {
		return "KEYWORD_CATCH";
	}
	else if( t == KEYWORD_CLASS) {
		return "KEYWORD_CLASS";
	}
	else if( t == KEYWORD_CONST) {
		return "KEYWORD_CONST";
	}
	else if( t == KEYWORD_CONTINUE) {
		return "KEYWORD_CONTINUE";
	}
	else if( t == KEYWORD_DELETE) {
		return "KEYWORD_DELETE";
	}
	else if( t == KEYWORD_DO) {
		return "KEYWORD_DO";
	}
	else if( t == KEYWORD_ELSE) {
		return "KEYWORD_ELSE";
	}
	else if( t == KEYWORD_ENUM) {
		return "KEYWORD_ENUM";
	}
	else if( t == KEYWORD_FALSE) {
		return "KEYWORD_FALSE";
	}
	else if( t == KEYWORD_FOR) {
		return "KEYWORD_FOR";
	}
	else if( t == KEYWORD_IF) {
		return "KEYWORD_IF";
	}
	else if( t == KEYWORD_IN) {
		return "KEYWORD_IN";
	}
	else if( t == KEYWORD_INLINE) {
		return "KEYWORD_INLINE";
	}
	else if( t == KEYWORD_NEW) {
		return "KEYWORD_NEW";
	}
	else if( t == KEYWORD_NULL) {
		return "KEYWORD_NULL";
	}
	else if( t == KEYWORD_PROTECTED) {
		return "KEYWORD_PROTECTED";
	}
	else if( t == KEYWORD_PRIVATE) {
		return "KEYWORD_PRIVATE";
	}
	else if( t == KEYWORD_PUBLIC) {
		return "KEYWORD_PUBLIC";
	}
	else if( t == KEYWORD_PTR) {
		return "KEYWORD_PTR";
	}
	else if( t == KEYWORD_REF) {
		return "KEYWORD_REF";
	}
	else if( t == KEYWORD_RETURN) {
		return "KEYWORD_RETURN";
	}
	else if( t == KEYWORD_SIZEOF) {
		return "KEYWORD_SIZEOF";
	}
	else if( t == KEYWORD_STATIC) {
		return "KEYWORD_STATIC";
	}
	else if( t == KEYWORD_STRUCT) {
		return "KEYWORD_STRUCT";
	}
	else if( t == KEYWORD_THIS) {
		return "KEYWORD_THIS";
	}
	else if( t == KEYWORD_THROW) {
		return "KEYWORD_THROW";
	}
	else if( t == KEYWORD_TRUE) {
		return "KEYWORD_TRUE";
	}
	else if( t == KEYWORD_TRY) {
		return "KEYWORD_TRY";
	}
	else if( t == KEYWORD_VIRTUAL) {
		return "KEYWORD_VIRTUAL";
	}
	else if( t == KEYWORD_WHILE) {
		return "KEYWORD_WHILE";
	}
	else if( t == KEYWORD_INT) {
		return "KEYWORD_INT";
	}
	else if( t == KEYWORD_INT8) {
		return "KEYWORD_INT8";
	}
	else if( t == KEYWORD_INT16) {
		return "KEYWORD_INT16";
	}
	else if( t == KEYWORD_INT32) {
		return "KEYWORD_INT32";
	}
	else if( t == KEYWORD_INT64) {
		return "KEYWORD_INT64";
	}
	else if( t == KEYWORD_UINT) {
		return "KEYWORD_UINT";
	}
	else if( t == KEYWORD_UINT8) {
		return "KEYWORD_UINT8";
	}
	else if( t == KEYWORD_UINT16) {
		return "KEYWORD_UINT16";
	}
	else if( t == KEYWORD_UINT32) {
		return "KEYWORD_UINT32";
	}
	else if( t == KEYWORD_UINT64) {
		return "KEYWORD_UINT64";
	}
	else if( t == KEYWORD_AND) {
		return "KEYWORD_AND";
	}
	else if( t == KEYWORD_OR) {
		return "KEYWORD_OR";
	}
	else if( t == KEYWORD_NOT) {
		return "KEYWORD_NOT";
	}
	else if( t == KEYWORD_XOR) {
		return "KEYWORD_XOR";
	}
	else if( t == KEYWORD_FLOAT) {
		return "KEYWORD_FLOAT";
	}
	else if( t == KEYWORD_DOUBLE) {
		return "KEYWORD_DOUBLE";
	}
	else if( t == PERIOD) {
		return "PERIOD";
	}
	else if( t == SEMI_COLON) {
		return "SEMI_COLON";
	}
	else if( t == PLUS) {
		return "PLUS";
	}
	else if( t == MINUS) {
		return "MINUS";
	}
	else if( t == ASTERISK) {
		return "ASTERISK";
	}
	else if( t == SLASH) {
		return "SLASH";
	}
	else if( t == AMPERSAND) {
		return "AMPERSAND";
	}
	else if( t == POUND_SIGN) {
		return "POUND_SIGN";
	}
	else if( t == LESS_THAN) {
		return "LESS_THAN";
	}
	else if( t == EQUALS) {
		return "EQUALS";
	}
	else if( t == GREATER_THAN) {
		return "GREATER_THAN";
	}
	else if( t == COMMA) {
		return "COMMA";
	}
	else if ( t == VERTICAL_BAR) {
		return "VERTICAL_BAR";
	}
	else if ( t == PERCENT) {
		return "PERCENT";
	}
	else if ( t == EXCLAMATION_POINT) {
		return "EXCLAMATION_POINT";
	}
	else if ( t == AT) {
		return "AT";
	}
	else if ( t == CARROT) {
		return "CARROT";
	}
	else if ( t == COLON) {
		return "COLON";
	}
	else {
		return "ERROR";
	}
}

void Tokenizer::tokenizeLine(std::string str, long lineNum) {
	int it=0;
	// If we're currently in a multi-line comment, search for the end of that comment
	if (inComment()) {
		for (it=it; it<str.length()-1; it++) {
			if (str.substr(it, 2) == "*/") {
				it++;
				break;
			}
		}
	}
	
	// Count indents
	int startIt = it;
	for (it=it; it<str.length(); it++) {
		if (str[it] != 't') {
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
		}
	}
	
	Token token = Token(UNKNOWN, "", lineNum, it);
	for (it=it; it<str.length(); it++) {
		if (token.type == UNKNOWN) {
			if(isdigit(str[it])) {
				token.type = INTEGER_LITERAL;
				token.str = str[it];
				token.lineNum = lineNum;
				token.charNum = it;
			}
			else if(str[it] == '\"') {
				token.type = STRING_LITERAL;
				token.str = "";
				token.lineNum = lineNum;
				token.charNum = it;
			}
			else if(str[it] == '\'') {
				token.type = CHARACTER_LITERAL;
				token.str = "";
				token.lineNum = lineNum;
				token.charNum = it;
			}
			else if(isStartOfIdentifierLetter(str[it])) {
				token.type = IDENTIFIER;
				token.str = str[it];
				token.lineNum = lineNum;
				token.charNum = it;
			}
			else {
				// single-character tokens
				if(str[it] == ';')
					setToken(token, SEMI_COLON, ";", lineNum, it);
				else if(str[it] == '{')
					setToken(token, OPEN_CURLY_BRACE, "{", lineNum, it);
				else if(str[it] == '}')
					setToken(token, CLOSE_CURLY_BRACE, "}", lineNum, it);
				else if(str[it] == '(')
					setToken(token, OPEN_PARENTHESIS, "(", lineNum, it);
				else if(str[it] == ')')
					setToken(token, CLOSE_PARENTHESIS, ")", lineNum, it);
				else if(str[it] == '[')
					setToken(token, OPEN_BRACKET, "[", lineNum, it);
				else if(str[it] == ']')
					setToken(token, CLOSE_BRACKET, "]", lineNum, it);
				else if(str[it] == '.')
					setToken(token, PERIOD, ".", lineNum, it);
				else if(str[it] == ':')
					setToken(token, PERIOD, ":", lineNum, it);
				else if(str[it] == '+')
					setToken(token, PERIOD, "+", lineNum, it);
				else if(str[it] == '-')
					setToken(token, PERIOD, "-", lineNum, it);
				else if(str[it] == '*')
					setToken(token, PERIOD, "*", lineNum, it);
				else if(str[it] == '/')
					setToken(token, PERIOD, "/", lineNum, it);
				else if(str[it] == '&')
					setToken(token, PERIOD, "&", lineNum, it);
				else if(str[it] == '#')
					setToken(token, PERIOD, "#", lineNum, it);
				else if(str[it] == '<')
					setToken(token, PERIOD, "<", lineNum, it);
				else if(str[it] == '=')
					setToken(token, PERIOD, "=", lineNum, it);
				else if(str[it] == '>')
					setToken(token, PERIOD, ">", lineNum, it);
				else if(str[it] == ',')
					setToken(token, PERIOD, ",", lineNum, it);
				else if(str[it] == '|')
					setToken(token, PERIOD, "|", lineNum, it);
				else if(str[it] == '%')
					setToken(token, PERIOD, "%", lineNum, it);
				else if(str[it] == '!')
					setToken(token, PERIOD, "!", lineNum, it);
				else if(str[it] == '^')
					setToken(token, PERIOD, "^", lineNum, it);
				else
					std::cout << "TOKERNIZER ERROR #1\n";
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
	return !stack.empty() && (stack.back() == multi_line_comment || stack.back() == single_line_comment);
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