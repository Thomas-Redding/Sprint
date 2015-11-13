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
	
	keywords.insert("break");
	keywords.insert("case");
	keywords.insert("catch");
	keywords.insert("char8");
	keywords.insert("char16");
	keywords.insert("char32");
	keywords.insert("class");
	keywords.insert("const");
	keywords.insert("continue");
	keywords.insert("delete");
	keywords.insert("do");
	keywords.insert("else");
	keywords.insert("enum");
	keywords.insert("false");
	keywords.insert("float");
	keywords.insert("float8");
	keywords.insert("float16");
	keywords.insert("float32");
	keywords.insert("float64");
	keywords.insert("for");
	keywords.insert("if");
	keywords.insert("inline");
	keywords.insert("int");
	keywords.insert("int8");
	keywords.insert("int16");
	keywords.insert("int32");
	keywords.insert("int64");
	keywords.insert("new");
	keywords.insert("NULL");
	keywords.insert("protected");
	keywords.insert("private");
	keywords.insert("return");
	keywords.insert("static");
	keywords.insert("struct");
	keywords.insert("switch");
	keywords.insert("this");
	keywords.insert("throw");
	keywords.insert("true");
	keywords.insert("try");
	keywords.insert("uint");
	keywords.insert("uint8");
	keywords.insert("uint16");
	keywords.insert("uint32");
	keywords.insert("uint64");
	keywords.insert("virtual");
	keywords.insert("void");
	keywords.insert("while");
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
	
	return rtn;
}

std::string Tokenizer::tokenTypeToString(TokenType t) {
	/*
	 INDENT, DEDENT, NEWLINE, KEYWORD, IDENTIFIER,
	 INTEGER_LITERAL, FLOAT_LITERAL, CHARACTER_LITERAL, STRING_LITERAL,
	 PUNCTUATION, UNKNOWN
	 */
	if(t == INDENT)
		return "INDENT";
	else if(t == DEDENT)
		return "DEDENT";
	else if(t == NEWLINE)
		return "NEWLINE";
	else if(t == KEYWORD)
		return "KEYWORD";
	else if(t == IDENTIFIER)
		return "IDENTIFIER";
	else if(t == INTEGER_LITERAL)
		return "INTEGER_LITERAL";
	else if(t == FLOAT_LITERAL)
		return "FLOAT_LITERAL";
	else if(t == CHARACTER_LITERAL)
		return "CHARACTER_LITERAL";
	else if(t == STRING_LITERAL)
		return "STRING_LITERAL";
	else if(t == PUNCTUATION)
		return "PUNCTUATION";
	else if(t == UNKNOWN)
		return "UNKNOWN";
	else
		return "";
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
	if(str.length() == 1) {
		rtn.push_back(str);
	}
	else if(str.length() == 2) {
		if(str == "--") {
			if(isdigit(nextChar) || isStartOfIdentifierLetter(nextChar)) {
				// minus negative
				rtn.push_back("-");
				rtn.push_back("-");
			}
			else
				rtn.push_back("--");
		}
		else if(str == "+-") {
			// plus negative
			rtn.push_back("+");
			rtn.push_back("-");
		}
		else
			rtn.push_back(str);
	}
	else {
		std::cout << "TOKENIZER: INVALID STRING OF PUNCTUATION\n";
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
