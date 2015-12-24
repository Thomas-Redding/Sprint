//
//  Tokenizer.cpp
//  Sprint
//
//  Created by Thomas Redding on 10/21/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "Tokenizer.hpp"

Tokenizer::Tokenizer() {
	brackets.insert('{');
	brackets.insert('(');
	brackets.insert('[');
	brackets.insert(']');
	brackets.insert(')');
	brackets.insert('}');
	
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
	
	keywords.insert("float");
	keywords.insert("double");
}

std::vector<Token> Tokenizer::process(std::string str) {
	str = removeComents(str);
	std::vector<std::string> lines = split(str, '\n');
	std::vector<unsigned int> indents;
	std::vector<Token> rtn;
	for(int i=0; i<lines.size(); i++) {
		int j = 0;
		for(j=0; j<lines[i].length(); j++) {
			if(lines[i][j] != '\t') {
				lines[i] = lines[i].substr(j);
				break;
			}
		}
		indents.push_back(j);
	}
	
	for(int i=0; i<lines.size(); i++) {
		if(i != 0) {
			if(indents[i-1] < indents[i]) {
				Token token;
				token.type = INDENT;
				token.str = "";
				token.lineNum = i;
				for(int j=indents[i-1]; j<indents[i]; j++) {
					token.charNum = j;
					rtn.push_back(token);
				}
			}
			else if(indents[i-1] > indents[i]) {
				Token token;
				token.type = DEDENT;
				token.str = "";
				token.lineNum = i;
				for(int j=indents[i]; j<indents[i-1]; j++) {
					token.charNum = 0;
					rtn.push_back(token);
				}
			}
		}
		tokenizeLine(lines[i], &rtn, i+1, indents[i]);
		Token token;
		token.type = NEWLINE;
		token.str = "\n";
		token.lineNum = i;
		token.charNum = lines[i].size();
		rtn.push_back(token);
	}
	
	// remove redundant NEWLINE tokens
	for(int i=1; i<rtn.size(); i++) {
		if(rtn[i].type == NEWLINE && rtn[i-1].type == NEWLINE) {
			rtn.erase(rtn.begin()+i);
			i --;
		}
	}
	if(rtn[0].type == NEWLINE)
		rtn.erase(rtn.begin());
	if(rtn[rtn.size()-1].type == NEWLINE)
		rtn.erase(rtn.begin() + rtn.size()-1);
	
	int finalIndentLevel = 0;
	for(int i=0; i<rtn.size(); i++) {
		if(rtn[i].type == INDENT)
			finalIndentLevel++;
		if(rtn[i].type == DEDENT)
			finalIndentLevel--;
	}
	for(int i=0; i<finalIndentLevel; i++) {
		rtn.push_back(Token(DEDENT, "", lines.size(), 0));
	}
	
	doMorgansDirtyWork(&rtn);
	
	return rtn;
}

void Tokenizer::doMorgansDirtyWork(std::vector<Token> *tokens) {
	for(int i=0; i<tokens->size(); i++) {
		Token *t = &tokens->at(i);
		if(t->type == BRACKET) {
			if(t->str == "(")
				t->type = OPEN_PARENTHESIS;
			else if(t->str == ")")
				t->type = CLOSE_PARENTHESIS;
			else if(t->str == "[")
				t->type = OPEN_BRACKET;
			else if(t->str == "]")
				t->type = OPEN_BRACKET;
			else if(t->str == "(")
				t->type = OPEN_CURLY_BRACE;
			else if(t->str == "(")
				t->type = OPEN_CURLY_BRACE;
		}
		else if(t->type == KEYWORD) {
			if(t->str == "abstract")
				t->type = KEYWORD_ABSTRACT;
			if(t->str == "break")
				t->type = KEYWORD_BREAK;
			if(t->str == "case")
				t->type = KEYWORD_CASE;
			if(t->str == "catch")
				t->type = KEYWORD_CATCH;
			if(t->str == "class")
				t->type = KEYWORD_CLASS;
			if(t->str == "const")
				t->type = KEYWORD_CONST;
			if(t->str == "continue")
				t->type = KEYWORD_CONTINUE;
			if(t->str == "delete")
				t->type = KEYWORD_DELETE;
			if(t->str == "do")
				t->type = KEYWORD_DO;
			if(t->str == "else")
				t->type = KEYWORD_ELSE;
			if(t->str == "enum")
				t->type = KEYWORD_ENUM;
			if(t->str == "false")
				t->type = KEYWORD_FALSE;
			if(t->str == "for")
				t->type = KEYWORD_FOR;
			if(t->str == "if")
				t->type = KEYWORD_IF;
			if(t->str == "in")
				t->type = KEYWORD_IN;
			if(t->str == "inline")
				t->type = KEYWORD_INLINE;
			if(t->str == "new")
				t->type = KEYWORD_NEW;
			if(t->str == "NULL")
				t->type = KEYWORD_NULL;
			if(t->str == "protected")
				t->type = KEYWORD_PROTECTED;
			if(t->str == "private")
				t->type = KEYWORD_PRIVATE;
			if(t->str == "ptr")
				t->type = KEYWORD_PTR;
			if(t->str == "ref")
				t->type = KEYWORD_REF;
			if(t->str == "return")
				t->type = KEYWORD_RETURN;
			if(t->str == "sizeof")
				t->type = KEYWORD_SIZEOF;
			if(t->str == "static")
				t->type = KEYWORD_STATIC;
			if(t->str == "struct")
				t->type = KEYWORD_STRUCT;
			if(t->str == "this")
				t->type = KEYWORD_THIS;
			if(t->str == "throw")
				t->type = KEYWORD_THROW;
			if(t->str == "true")
				t->type = KEYWORD_TRUE;
			if(t->str == "try")
				t->type = KEYWORD_TRY;
			if(t->str == "virtual")
				t->type = KEYWORD_VIRTUAL;
			if(t->str == "while")
				t->type = KEYWORD_WHILE;
			if(t->str == "int")
				t->type = KEYWORD_INT;
			if(t->str == "int8")
				t->type = KEYWORD_INT8;
			if(t->str == "int16")
				t->type = KEYWORD_INT16;
			if(t->str == "int32")
				t->type = KEYWORD_INT32;
			if(t->str == "int64")
				t->type = KEYWORD_INT64;
			if(t->str == "uint")
				t->type = KEYWORD_UINT;
			if(t->str == "uint8")
				t->type = KEYWORD_UINT8;
			if(t->str == "uint16")
				t->type = KEYWORD_UINT16;
			if(t->str == "uint32")
				t->type = KEYWORD_UINT32;
			if(t->str == "uint64")
				t->type = KEYWORD_UINT64;
		}
		else if(t->type == PUNCTUATION) {
			if(t->str == ".")
				t->type = PERIOD;
			else if(t->str == ";")
				t->type = SEMI_COLON;
			else if(t->str == "+")
				t->type = PLUS;
			else if(t->str == "-")
				t->type = MINUS;
			else if(t->str == "*")
				t->type = ASTERISK;
			else if(t->str == "/")
				t->type = SLASH;
			else if(t->str == "&")
				t->type = AMPERSAND;
			else if(t->str == "#")
				t->type = POUND_SIGN;
			else if(t->str == "<")
					t->type = LESS_THAN;
			else if(t->str == "=")
				t->type = EQUALS;
			else if(t->str == ">")
				t->type = GREATER_THAN;
			else if(t->str == ",")
				t->type = COMMA;
			else if(t->str == "|")
				t->type = VERTICAL_BAR;
			else if(t->str == "%")
				t->type = PERCENT;
			else if(t->str == "!")
				t->type = EXCLAMATION_POINT;
			else if(t->str == "@")
				t->type = AT;
			else if(t->str == "^")
				t->type = CARROT;
		}
	}
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
	else {
		return "ERROR";
	}
}

std::string Tokenizer::removeComents(std::string str) {
	/*
	 0 - normal
	 1 - string
	 2 - single-line-comment
	 3 - block-comment
	 */
	std::string rtn = "";
	int state = 0;
	for(int i=0; i<str.length()-1; i++) {
		if(state == 0) {
			if(str[i] == '\"') {
				state = 1;
			}
			else if(str[i] == '/' && str[i+1] == '/') {
				state = 2;
			}
			else if(str[i] == '/' && str[i+1] == '*') {
				state = 3;
			}
		}
		else if(state == 1) {
			if(str[i] == '\"' && str[i-1] != '\\')
				state = 0;
		}
		else if(state == 2) {
			if(str[i] == '\n')
				state = 0;
		}
		else if(state == 3) {
			if(str[i-2] == '*' && str[i-1] == '/')
				state = 0;
		}
		
		if(state <= 1) {
			rtn += str[i];
		}
	}
	return rtn;
}

void Tokenizer::tokenizeLine(std::string str, std::vector<Token> *rtn, int lineNum, int tabs) {
	Token token;
	token.type = UNKNOWN;
	str += " ";
	for(int i=0; i<str.length(); i++) {
		if(token.type == UNKNOWN) {
			if(isdigit(str[i])) {
				token.type = INTEGER_LITERAL;
				token.str = str[i];
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
			else if(str[i] == '\"') {
				token.type = STRING_LITERAL;
				token.str = "";
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
			else if(str[i] == '\'') {
				token.type = CHARACTER_LITERAL;
				token.str = "";
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
			else if(isStartOfIdentifierLetter(str[i])) {
				token.type = IDENTIFIER;
				token.str = str[i];
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
			else if(isStartOfIdentifierLetter(str[i])) {
				token.type = IDENTIFIER;
				token.str = str[i];
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
			else if(str[i] == ';') {
				token.type = NEWLINE;
				token.str = ";";
				token.lineNum = lineNum;
				token.charNum = i+tabs;
				rtn->push_back(token);
				token.type = UNKNOWN;
				token.str = "";
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
			else if(brackets.find(str[i]) != brackets.end()) {
				token.type = BRACKET;
				token.str = str[i];
				token.lineNum = lineNum;
				token.charNum = i+tabs;
				rtn->push_back(token);
				token.type = UNKNOWN;
				token.str = "";
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
			else if(punctuation.find(str[i]) != punctuation.end()) {
				token.type = PUNCTUATION;
				token.str = str[i];
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
		}
		else if(token.type == INTEGER_LITERAL) {
			if(isdigit(str[i]))
				token.str += str[i];
			else if(str[i] == '.' || str[i] == 'e' || str[i] == 'E') {
				token.type = FLOAT_LITERAL;
				token.str += str[i];
			}
			else {
				rtn->push_back(token);
				token.type = UNKNOWN;
				token.str = "";
				i--;
			}
		}
		else if(token.type == FLOAT_LITERAL) {
			if(isdigit(str[i]) || str[i] == '.' || str[i] == 'e' || str[i] == 'E')
				token.str += str[i];
			else {
				rtn->push_back(token);
				token.type = UNKNOWN;
				token.str = "";
				token.lineNum = lineNum;
				token.charNum = i+tabs;
				i--;
			}
		}
		else if(token.type == STRING_LITERAL) {
			if(str[i] == '\"' && str[i-1] != '\\') {
				rtn->push_back(token);
				token.type = UNKNOWN;
				token.str = "";
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
			else
				token.str += str[i];
		}
		else if(token.type == CHARACTER_LITERAL) {
			if(str[i] == '\'' && str[i-1] != '\\') {
				rtn->push_back(token);
				token.type = UNKNOWN;
				token.str = "";
				token.lineNum = lineNum;
				token.charNum = i+tabs;
			}
			else
				token.str += str[i];
		}
		else if(token.type == IDENTIFIER) {
			if(isIdentifierLetter(str[i]))
				token.str += str[i];
			else {
				if(keywords.find(token.str) != keywords.end())
					token.type = KEYWORD;
				rtn->push_back(token);
				token.type = UNKNOWN;
				token.lineNum = lineNum;
				token.str = i;
				token.lineNum = lineNum;
				token.charNum = i+tabs;
				i--;
			}
		}
		else if(token.type == PUNCTUATION) {
			if(punctuation.find(str[i]) != punctuation.end()) {
				token.str += str[i];
			}
			else {
				std::vector<std::string> newTokenStrings;
				newTokenStrings = dividePunctuations(token.str, str[i]);
				for(int j=0; j<newTokenStrings.size(); j++) {
					Token token;
					token.type = PUNCTUATION;
					token.str = newTokenStrings[j];
					token.lineNum = lineNum;
					token.charNum = i+j;
					rtn->push_back(token);
				}
				token.type = UNKNOWN;
				i--;
			}
		}
	}
}

std::vector<std::string> Tokenizer::dividePunctuations(std::string str, char nextChar) {
	std::vector<std::string> rtn = std::vector<std::string>();
	for(int i=0; i<str.length(); i++) {
		rtn.push_back(str.substr(i, 1));
	}
	return rtn;
}

bool Tokenizer::isStartOfIdentifierLetter(char c) {
	return isalnum(c);
}

bool Tokenizer::isIdentifierLetter(char c) {
	return isalnum(c);
}

std::vector<std::string> Tokenizer::split(std::string str, char delim) {
	std::vector<std::string> elems;
	std::string item;
	while(true) {
		int index = str.find(delim);
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
