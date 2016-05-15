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

std::vector<Token> Tokenizer::process(std::string str) {
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
			else if (isdigit(str[it]) || str[it] == '+' || str[it] == '-') {
				cur.type = INTEGER_LITERAL;
				cur.str += str[it];
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
					error("You cannot put a white-space character after a period. (line: " + std::to_string(getLineNum()) + ", char:" + std::to_string(getCharNum()) + ")\n");
				else if (isPunc(str[it]))
					error("You cannot put a puncutation character after a period. (line: " + std::to_string(getLineNum()) + ", char:" + std::to_string(getCharNum()) + ")\n");
			}
			else if (isStartChar(str[it])) {
				cur.type = IDENTIFIER;
				cur.str = str[it];
			}
			else if (isPunc(str[it])) {
				cur.type = PUNCTUATION;
				if (it > 0 && isspace(str[it-1]))
					cur.str = str[it-1];
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
				error("Float literal cannot contain multiple decimal points (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
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
			else if (isspace(str[it]))
				cur.str += str[it];
			else {
				// todo: magically solve punctuation
				handlePunc(cur);
				it--;
			}
		}
		else {
			error("Error: Type 1");
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
	std::cout << str;
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
	error("Error #2 (it: " + std::to_string(it) + ")");
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

void Tokenizer::handlePunc(Token &cur) {
	// everything in cur.str is punctuation or an empty space

	// "  ab  c    d" -> " ab c d"
	int puncCount = 0;
	std::vector<char> list;
	for (int i=0; i<cur.str.length(); ++i) {
		if(isspace(cur.str[i])) {
			if(list.size() > 0 && list[list.size()-1] != ' ')
				list.push_back(' ');
		}
		else {
			list.push_back(cur.str[i]);
			puncCount++;
		}
	}

	if (puncCount == 1) {
		// no punctuation character
		cur.str.erase(remove_if(cur.str.begin(), cur.str.end(), isspace), cur.str.end());
		if (cur.str == ".")
			cur.type = PERIOD;
		else if (cur.str == ":")
			cur.type = COLON;
		else if (cur.str == ";")
			cur.type = SEMI_COLON;
		else if (cur.str == "+")
			cur.type = PLUS;
		else if (cur.str == "-")
			cur.type = MINUS;
		else if (cur.str == "*")
			cur.type = ASTERISK;
		else if (cur.str == "/")
			cur.type = SLASH;
		else if (cur.str == "&")
			cur.type = AMPERSAND;
		else if (cur.str == "#")
			cur.type = POUND_SIGN;
		else if (cur.str == "<")
			cur.type = LESS_THAN;
		else if (cur.str == "=")
			cur.type = EQUALS;
		else if (cur.str == ">")
			cur.type = GREATER_THAN;
		else if (cur.str == ",")
			cur.type = COMMA;
		else if (cur.str == "|")
			cur.type = VERTICAL_BAR;
		else if (cur.str == "%")
			cur.type = PERCENT;
		else if (cur.str == "!")
			cur.type = EXCLAMATION_POINT;
		else if (cur.str == "^")
			cur.type = CARROT;
		else if (cur.str == "?")
			cur.type = QUESTION_MARK;
		else if (cur.str == "/")
			cur.type = BACK_SLASH;
		else if (cur.str == "@")
			cur.type = AT;
		else
			error("Punctuation Error (" + cur.str + ")");
		resetAndSave(cur);
	}
	else {
		resetAndSave(cur);
	}
}



