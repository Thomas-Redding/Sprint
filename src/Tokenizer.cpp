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
	keywords.insert("void");

	punctuation.insert('.');
	punctuation.insert(':');
	punctuation.insert(';');
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
	punctuation.insert('/');
	punctuation.insert('@');

	brackets.insert('{');
	brackets.insert('(');
	brackets.insert('[');
	brackets.insert(']');
	brackets.insert(')');
	brackets.insert('}');
}

std::list<Token> Tokenizer::process(std::string str) {
	rtn.clear();
	it = 0;
	bool isInMultiLineComment = false;
	bool isInSingleLineComment = false;


	std::vector<std::string> byLine = split(str, '\n');
	cumulativeCharsPerLine.clear();
	for (long i=0; i<byLine.size(); i++)
		cumulativeCharsPerLine.push_back(byLine[i].size());
	for (long i=1; i<byLine.size(); i++)
		cumulativeCharsPerLine[i] = cumulativeCharsPerLine[i] + cumulativeCharsPerLine[i-1] + 1;


	Token cur = Token(UNKNOWN, "", getLineNum(), getCharNum());


	for (it=0; it<str.length(); it++) {
		if (isInMultiLineComment) {
			if (str[it] == '*' && it < str.length()-1 && str[it+1] == '/') {
				isInMultiLineComment = false;
				it++;
			}
		}
		else if (isInSingleLineComment) {
			if (str[it] == '\n')
				isInSingleLineComment = false;
		}
		else if (cur.type == UNKNOWN) {
			if (str[it] == '\"')
				cur.type = STRING_LITERAL;
			else if (str[it] == '/' && it < str.length()-1 && str[it+1] == '*') {
				isInMultiLineComment = true;
				it++;
			}
			else if (str[it] == '/' && it < str.length()-1 && str[it+1] == '/') {
				isInSingleLineComment = true;
			}
			else if (isdigit(str[it])) {
				cur.type = INTEGER_LITERAL;
				cur.str += str[it];
			}
			else if (str[it] == '+' || str[it] == '-') {
				if (it == 0) {
					error("Tokenizer: Can't begin a document with '+' or '-'.");
				}
				cur.str += str[it];

				long i;
				for (i=it+1; i<str.length(); i++) {
					if (str[i] != ' ') {
						break;
					}
				}
				if (isdigit(i)) {
					Token last = rtn.back();
					if (isPunc(last .type) || last.type == OPEN_BRACKET || last.type == OPEN_CURLY_BRACE || last.type == OPEN_PARENTHESIS)
						cur.type = INTEGER_LITERAL;
					else
						cur.type = PUNCTUATION;
				}
				else {
					cur.type = PUNCTUATION;
				}
			}
			else if (str[it] == '.') {
				if (it == str.length()-1) {
					cur.type = PERIOD;
					cur.str = ".";
				}
				else if (isdigit(str[it+1])) {
					cur.type = FLOAT_LITERAL;
					cur.str = ".";
				}
				else if (isStartChar(str[it+1])) {
					cur.type = PERIOD;
					cur.str = ".";
					resetAndSave(cur);
				}
				else if (isspace(str[it]))
					error("Tokenizer: You cannot put a white-space character after a period. (line: " + std::to_string(getLineNum()) + ", char:" + std::to_string(getCharNum()) + ")\n");
				else if (isPunc(str[it]))
					error("Tokenizer: You cannot put a puncutation character after a period. (line: " + std::to_string(getLineNum()) + ", char:" + std::to_string(getCharNum()) + ")\n");
			}
			else if (isStartChar(str[it])) {
				cur.type = IDENTIFIER;
				cur.str = str[it];
			}
			else if (isPunc(str[it])) {
				cur.type = PUNCTUATION;
				cur.str += str[it];
			}
			else if (isBracket(str[it])) {
				cur.str = str[it];
				if (str[it] == '{')
					cur.type = OPEN_CURLY_BRACE;
				else if (str[it] == '(')
					cur.type = OPEN_PARENTHESIS;
				else if (str[it] == '[')
					cur.type = OPEN_BRACKET;
				else if (str[it] == '}')
					cur.type = CLOSE_CURLY_BRACE;
				else if (str[it] == ')')
					cur.type = CLOSE_PARENTHESIS;
				else if (str[it] == ']')
					cur.type = CLOSE_BRACKET;
				resetAndSave(cur);
			}
		}
		else if (cur.type == IDENTIFIER) {
			if (isMiddleChar(str[it]))
				cur.str += str[it];
			else {
				categorizeIdentifier(cur);
				resetAndSave(cur);
				it--;
			}
		}
		else if (cur.type == INTEGER_LITERAL) {
			if (isdigit(str[it]))
				cur.str += str[it];
			else if (str[it] == '.') {
				cur.type = FLOAT_LITERAL;
				cur.str += '.';
			}
			else if (str[it] == 'e' || str[it] == 'E') {
				cur.type = FLOAT_LITERAL2;
				cur.str += str[it];
			}
			else if (str[it] == ' ') {
				// ignore spaces
			}
			else {
				resetAndSave(cur);
				it--;
			}
		}
		else if (cur.type == FLOAT_LITERAL) {
			if (isdigit(str[it]))
				cur.str += str[it];
			else if (str[it] == '.') {
				error("Tokenizer: Float literal cannot contain multiple decimal points (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
			}
			else if (str[it] == 'e' || str[it] == 'E') {
				cur.type = FLOAT_LITERAL2;
				cur.str += str[it];
			}
			else if (str[it] == ' ') {
				// ignore spaces
			}
			else {
				resetAndSave(cur);
				it--;
			}
		}
		else if (cur.type == FLOAT_LITERAL2) {
			if (isdigit(str[it]))
				cur.str += str[it];
			else if (str[it] == ' ') {
				// ignore space
			}
			else {
				cur.type = FLOAT_LITERAL;
				resetAndSave(cur);
				it--;
			}
		}
		else if (cur.type == STRING_LITERAL) {
			if (str[it] == '\"') {
				long backslashes = 0;
				for (int j=it-1; j>=0; --j) {
					if (str[j] == '\\')
						backslashes++;
					else
						break;
				}
				if (backslashes%2 == 0)
					resetAndSave(cur);
				else
					cur.str += str[it];
			}
			else
				cur.str += str[it];
		}
		else if (cur.type == PUNCTUATION) {
			if (isPunc(str[it]))
				cur.str += str[it];
			else {
				resetAndSave(cur);
				it--;
			}
		}
		else {
			error("Tokenizer: Error 1 (it: " + std::to_string(it) + ")");
		}
	}

	// handle punctuation - todo
	std::list<Token>::iterator i;
	for (i = rtn.begin(); i != rtn.end(); ++i) {
		if (i->type == PUNCTUATION) {
			TokenType t = categorizePunc(i->str);
			if (t == UNKNOWN) {
				error("Tokenizer: could not recognize punctuation (" + i->str + ")");
			}
			else if (t == PUNCTUATION) {
				// +-
				i->str = "+";
				i->type = PLUS;
				++i;
				rtn.insert(i, Token(MINUS, "-", i->lineNum, i->charNum+1));
			}
			else if (t == PLUS_PLUS) {
				// ++
				std::list<Token>::iterator j = i;
				++j;
				if (j->type == CLOSE_BRACKET || j->type == CLOSE_PARENTHESIS || j->type == CLOSE_CURLY_BRACE || j->type == SEMI_COLON || j->type == COMMA)
					i->type = PLUS_PLUS;
				else {
					i->type = PLUS;
					i->str = "+";
					rtn.insert(j, Token(PLUS, "+", i->lineNum, i->charNum+1));
				}
			}
			else if (t == MINUS_MINUS) {
				std::list<Token>::iterator j = i;
				++j;
				if (j->type == CLOSE_BRACKET || j->type == CLOSE_PARENTHESIS || j->type == CLOSE_CURLY_BRACE || j->type == SEMI_COLON || j->type == COMMA)
					i->type = MINUS_MINUS;
				else {
					i->type = MINUS;
					i->str = "-";
					rtn.insert(j, Token(MINUS, "-", i->lineNum, i->charNum+1));
				}
			}
			else {
				i->type = t;
			}
		}
	    
	}

	return rtn;
}

bool Tokenizer::isStartChar(char c) {
	if (isdigit(c))
		return false;
	else if (isPunc(c))
		return false;
	else if (isspace(c))
		return false;
	else if (isBracket(c))
		return false;
	return true;
}

bool Tokenizer::isMiddleChar(char c) {
	if (isPunc(c))
		return false;
	else if (isspace(c))
		return false;
	else if (isBracket(c))
		return false;
	return true;
}

void Tokenizer::resetAndSave(Token &cur) {
	rtn.push_back(cur);
	cur.type = UNKNOWN;
	cur.str = "";
	cur.lineNum = getLineNum();
	cur.charNum = getCharNum();
}

inline bool Tokenizer::isPunc(char c) {
	return punctuation.find(c) != punctuation.end();
}

inline bool Tokenizer::isBracket(char c) {
	return brackets.find(c) != brackets.end();
}

void Tokenizer::error(std::string str) {
	std::cout << str << "\n";
	exit(1);
}

std::vector<std::string> Tokenizer::split(std::string str, char delim) {
	std::vector<std::string> elems;
	std::string item;
	while(true) {
		size_t index = str.find(delim);
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

long Tokenizer::getLineNum() {
	for (long i=0; i<cumulativeCharsPerLine.size(); ++i) {
		if (it <= cumulativeCharsPerLine[i])
			return i;
	}
	error("Tokenizer: Error 2 (it: " + std::to_string(it) + ")");
	return -1;
}

long Tokenizer::getCharNum() {
	int lineNum = getLineNum();
	if (lineNum == 0)
		return it;
	else
		return it - cumulativeCharsPerLine[lineNum-1];
}

/*

only c++
#	##	<:	:>	<%	%>	%:	%:%:	::	.*
->*

both
;	:	...	?	.	+	-	*	/	%	^
&	|	~	!	=	<	>	+=	-=	*=	/=
%=	^=	&=	|=	<<	>>	<<=	>>=	==	!=	<=
>=	&&	||	++	--	,
->

only ours
@

c++ - maybe ours


*/

TokenType Tokenizer::categorizePunc(const std::string &str) {
	TokenType rtn;
	if (str == ".")
		rtn = PERIOD;
	else if (str == ":")
		rtn = COLON;
	else if (str == ";")
		rtn = SEMI_COLON;
	else if (str == "+")
		rtn = PLUS;
	else if (str == "-")
		rtn = MINUS;
	else if (str == "*")
		rtn = ASTERISK;
	else if (str == "/")
		rtn = SLASH;
	else if (str == "&")
		rtn = AMPERSAND;
	else if (str == "#")
		rtn = POUND_SIGN;
	else if (str == "<")
		rtn = LESS_THAN;
	else if (str == "=")
		rtn = EQUALS;
	else if (str == ">")
		rtn = GREATER_THAN;
	else if (str == ",")
		rtn = COMMA;
	else if (str == "|")
		rtn = VERTICAL_BAR;
	else if (str == "%")
		rtn = PERCENT;
	else if (str == "!")
		rtn = EXCLAMATION_POINT;
	else if (str == "^")
		rtn = CARROT;
	else if (str == "?")
		rtn = QUESTION_MARK;
	else if (str == "/")
		rtn = BACK_SLASH;
	else if (str == "@")
		rtn = AT;
	else if (str == "+=")
		rtn = PLUS_EQUALS;
	else if (str == "-=")
		rtn = MINUS_EQUALS;
	else if (str == "*=")
		rtn = ASTERISK_EQUALS;
	else if (str == "&=")
		rtn = AMPERSAND_EQUALS;
	else if (str == "^=")
		rtn = CARROT_EQUALS;
	else if (str == ":=")
		rtn = COLON_EQUALS;
	else if (str == "|=")
		rtn = VERTICAL_BAR_EQUALS;
	else if (str == "<<")
		rtn = SHIFT_LEFT;
	else if (str == ">>")
		rtn = SHIFT_RIGHT;
	else if (str == ">=")
		rtn = GREATER_THAN_EQUALS;
	else if (str == "<=")
		rtn = LESS_THAN_EQUALS;
	else if (str == "<<=")
		rtn = SHIFT_LEFT_EQUALS;
	else if (str == ">>=")
		rtn = SHIFT_RIGHT_EQUALS;
	else if (str == "!=")
		rtn = EXCLAMATION_POINT_EQUALS;
	else if (str == "==")
		rtn = EQUAL_EQUALS;
	else if (str == "++")
		rtn = PLUS_PLUS;
	else if (str == "--")
		rtn = MINUS_MINUS;
	else if (str == "->")
		rtn = ARROW;
	else if (str == "+-")
		rtn = PUNCTUATION;
	else
		rtn = UNKNOWN;
	return rtn;
}


void Tokenizer::categorizeIdentifier(Token &cur) {
	if(cur.str == "abstract")
		cur.type = KEYWORD_ABSTRACT;
	else if(cur.str == "break")
		cur.type = KEYWORD_BREAK;
	else if(cur.str == "case")
		cur.type = KEYWORD_CASE;
	else if(cur.str == "catch")
		cur.type = KEYWORD_CATCH;
	else if(cur.str == "class")
		cur.type = KEYWORD_CLASS;
	else if(cur.str == "const")
		cur.type = KEYWORD_CONST;
	else if(cur.str == "continue")
		cur.type = KEYWORD_CONTINUE;
	else if(cur.str == "delete")
		cur.type = KEYWORD_DELETE;
	else if(cur.str == "do")
		cur.type = KEYWORD_DO;
	else if(cur.str == "else")
		cur.type = KEYWORD_ELSE;
	else if(cur.str == "enum")
		cur.type = KEYWORD_ENUM;
	else if(cur.str == "false")
		cur.type = KEYWORD_FALSE;
	else if(cur.str == "for")
		cur.type = KEYWORD_FOR;
	else if(cur.str == "if")
		cur.type = KEYWORD_IF;
	else if(cur.str == "in")
		cur.type = KEYWORD_IN;
	else if(cur.str == "inline")
		cur.type = KEYWORD_INLINE;
	else if(cur.str == "new")
		cur.type = KEYWORD_NEW;
	else if(cur.str == "NULL")
		cur.type = KEYWORD_NULL;
	else if(cur.str == "protected")
		cur.type = KEYWORD_PROTECTED;
	else if(cur.str == "private")
		cur.type = KEYWORD_PRIVATE;
	else if(cur.str == "ptr")
		cur.type = KEYWORD_PTR;
	else if(cur.str == "public")
		cur.type = KEYWORD_PUBLIC;
	else if(cur.str == "ref")
		cur.type = KEYWORD_REF;
	else if(cur.str == "return")
		cur.type = KEYWORD_RETURN;
	else if(cur.str == "sizeof")
		cur.type = KEYWORD_SIZEOF;
	else if(cur.str == "static")
		cur.type = KEYWORD_STATIC;
	else if(cur.str == "struct")
		cur.type = KEYWORD_STRUCT;
	else if(cur.str == "switch")
		cur.type = KEYWORD_SWITCH;
	else if(cur.str == "this")
		cur.type = KEYWORD_THIS;
	else if(cur.str == "throw")
		cur.type = KEYWORD_THROW;
	else if(cur.str == "true")
		cur.type = KEYWORD_TRUE;
	else if(cur.str == "try")
		cur.type = KEYWORD_TRY;
	else if(cur.str == "virtual")
		cur.type = KEYWORD_VIRTUAL;
	else if(cur.str == "while")
		cur.type = KEYWORD_WHILE;
	else if(cur.str == "uint")
		cur.type = KEYWORD_UINT;
	else if(cur.str == "uint8")
		cur.type = KEYWORD_UINT8;
	else if(cur.str == "uint16")
		cur.type = KEYWORD_UINT16;
	else if(cur.str == "uint32")
		cur.type = KEYWORD_UINT32;
	else if(cur.str == "uint64")
		cur.type = KEYWORD_UINT64;
	else if(cur.str == "int")
		cur.type = KEYWORD_INT;
	else if(cur.str == "int8")
		cur.type = KEYWORD_INT8;
	else if(cur.str == "int16")
		cur.type = KEYWORD_INT16;
	else if(cur.str == "int32")
		cur.type = KEYWORD_INT32;
	else if(cur.str == "int64")
		cur.type = KEYWORD_INT64;
	else if(cur.str == "and")
		cur.type = KEYWORD_AND;
	else if(cur.str == "or")
		cur.type = KEYWORD_OR;
	else if(cur.str == "not")
		cur.type = KEYWORD_NOT;
	else if(cur.str == "xor")
		cur.type = KEYWORD_XOR;
	else if(cur.str == "float")
		cur.type = KEYWORD_FLOAT;
	else if(cur.str == "double")
		cur.type = KEYWORD_DOUBLE;
	else
		cur.type = IDENTIFIER;
}

const bool Tokenizer::isPunc(TokenType t) {
	return t == PUNCTUATION || t == PERIOD || t == COLON || t == SEMI_COLON || t == PLUS || t == MINUS || t == ASTERISK || t == SLASH || t == AMPERSAND || t == POUND_SIGN || t == LESS_THAN || t == EQUALS || t == GREATER_THAN || t == COMMA || t == VERTICAL_BAR || t == PERCENT || t == EXCLAMATION_POINT || t == CARROT || t == QUESTION_MARK || t == BACK_SLASH || t == AT || t == PLUS_EQUALS || t == MINUS_EQUALS || t == SLASH_EQUALS || t == ASTERISK_EQUALS || t == AMPERSAND_EQUALS || t == CARROT_EQUALS || t == VERTICAL_BAR_EQUALS || t == PLUS_PLUS || t == MINUS_MINUS || t == SHIFT_LEFT || t == SHIFT_RIGHT || t == GREATER_THAN_EQUALS || t == LESS_THAN_EQUALS || t == SHIFT_LEFT_EQUALS || t == SHIFT_RIGHT_EQUALS || t == EXCLAMATION_POINT_EQUALS || t == EQUAL_EQUALS;
}

const bool Tokenizer::isKeyWord(TokenType t) {
		return t == KEYWORD_ABSTRACT || t == KEYWORD_BREAK || t == KEYWORD_CASE || t == KEYWORD_CATCH || t == KEYWORD_CLASS || t == KEYWORD_CONST || t == KEYWORD_CONTINUE || t == KEYWORD_DELETE || t == KEYWORD_DO || t == KEYWORD_ELSE || t == KEYWORD_ENUM || t == KEYWORD_FALSE || t == KEYWORD_FOR || t == KEYWORD_IF || t == KEYWORD_IN || t == KEYWORD_INLINE || t == KEYWORD_NEW || t == KEYWORD_NULL || t == KEYWORD_PROTECTED || t == KEYWORD_PRIVATE || t == KEYWORD_PTR || t == KEYWORD_REF || t == KEYWORD_RETURN || t == KEYWORD_SIZEOF || t == KEYWORD_STATIC || t == KEYWORD_STRUCT || t == KEYWORD_SWITCH || t == KEYWORD_THIS || t == KEYWORD_THROW || t == KEYWORD_TRUE || t == KEYWORD_TRY || t == KEYWORD_VIRTUAL || t == KEYWORD_WHILE || t == KEYWORD_INT || t == KEYWORD_INT8 || t == KEYWORD_INT16 || t == KEYWORD_INT32 || t == KEYWORD_INT64 || t == KEYWORD_UINT || t == KEYWORD_UINT8 || t == KEYWORD_UINT16 || t == KEYWORD_UINT32 || t == KEYWORD_UINT64 || t == KEYWORD_AND || t == KEYWORD_OR || t == KEYWORD_NOT || t == KEYWORD_XOR || t == KEYWORD_FLOAT || t == KEYWORD_DOUBLE || t == KEYWORD_PUBLIC || t == KEYWORD_VOID;
}

const std::string Tokenizer::tokenToString2(const Token& t) {
	if (t.str == "")
		return "(" + Token::tokenTypeToString(t.type) + ")";
	else
		return "(" + Token::tokenTypeToString(t.type) + " '" + t.str + "')";
}


