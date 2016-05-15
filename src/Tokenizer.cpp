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
			else if (isspace(str[it]))
				cur.str += str[it];
			else {
				// todo: magically solve punctuation
				handlePunc(cur, str[it]);
				it--;
			}
		}
		else {
			error("Tokenizer: Error 1 (it: " + std::to_string(it) + ")");
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

void Tokenizer::handlePunc(Token &cur, char c) {
	// everything in cur.str is punctuation or an empty space

	// "  ab  c    d" -> [" ", "ab", " ", "c", " ", "d"]

	std::vector<std::string> list;
	std::string str;
	for (int i=0; i<cur.str.length(); ++i) {
		if(isspace(cur.str[i])) {
			if (str != "") {
				list.push_back(str);
				str = "";
			}
		}
		else {
			str += cur.str[i];
		}
	}
	if (str != "")
		list.push_back(str);

	if (list.size() == 1) {
		TokenType cat = categorize(list[0]);
		std::cout << list[0] << " : " << cat << "\n";
		if (cat == PLUS_PLUS) {
			if(c == ')' || c == '}' || c == ']' || c == ',' || c == ';') {
				cur.type = cat;
				cur.str = list[0];
				resetAndSave(cur);
			}
			else if(isdigit(c)) {
				cur.type = PLUS;
				cur.str = "+";
				resetAndSave(cur);
				cur.type = PLUS;
				cur.str = "+";
				resetAndSave(cur);
			}
			else {
				error("Tokenizer: Punctuation Error 5");
			}
		}
		else if (cat == MINUS_MINUS) {
			if(c == ')' || c == '}' || c == ']' || c == ',' || c == ';') {
				cur.type = cat;
				cur.str = list[0];
				resetAndSave(cur);
			}
			else if(isdigit(c) || isStartChar(c)) {
				cur.type = MINUS;
				cur.str = "-";
				resetAndSave(cur);
				cur.type = MINUS;
				cur.str = "-";
				resetAndSave(cur);
			}
			else {
				error("Tokenizer: Punctuation Error 5");
			}
		}
		else if (cat == PUNCTUATION) {
			// need to break up with virtual spaces
			// this should have some punctuation followed by a "-" or "+"
			std::string str1 = list[0].substr(0, list[0].length()-1);
			std::string str2 = list[0].substr(list[0].length()-1);
			TokenType cat1 = categorize(str1);
			TokenType cat2 = categorize(str2);

			// the punctuation before the "+" or "-"
			if (cat1 == PUNCTUATION) {
				error("Tokenizer: Punctuation Error 4 (" + str1 + ")");
			}
			else {
				cur.str = str1;
				cur.type = cat1;
				resetAndSave(cur);
			}

			// the "+" or "-"
			if (cat2 == PLUS) {
				cur.str = "+";
				cur.type = PLUS;
			}
			else if (cat2 == MINUS) {
				cur.str = "-";
				cur.type = MINUS;
			}
			else {
				error("Tokenizer: Punctuation Error 3 (" + str2 + ")");
			}
			resetAndSave(cur);
		}
		else if (cat == UNKNOWN) {
			error("Tokenizer: Punctuation Error 3 (" + cur.str + ")");
		}
		else {
			cur.str = list[0];
			cur.type = cat;
			resetAndSave(cur);
		}
	}
	else if (list.size() == 2) {
		TokenType cat = categorize(list[0]);
		if (cat == PLUS_PLUS)
			error("Tokenizer: cannot follow ++ by more punctuation. (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
		else if (cat == PLUS_PLUS)
			error("Tokenizer: cannot follow -- by more punctuation. (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
		else if (cat == PUNCTUATION)
			error("Tokenizer: cannot use three strings of puncutation in a row. (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
		else {
			cur.str = list[0];
			cur.type = cat;
			resetAndSave(cur);
		}

		cat = categorize(list[1]);
		if (cat == PLUS_PLUS)
			error("Tokenizer: cannot follow ++ by more punctuation. (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
		else if (cat == PLUS_PLUS)
			error("Tokenizer: cannot follow -- by more punctuation. (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
		else if (cat == PUNCTUATION)
			error("Tokenizer: cannot use three strings of puncutation in a row. (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
		else {
			cur.str = list[1];
			cur.type = cat;
			resetAndSave(cur);
		}
	}
	else if (list.size() == 0) {
		error("Tokenizer: Punctuation Error 2 (" + cur.str + ")");
	}
	else {
		error("Tokenizer: More than two strings of punctuation in a row are illegal. (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ", str: " + cur.str + ")");
	}
}

TokenType Tokenizer::categorize(std::string &str) {
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
	else if (str == "+-")
		rtn = PUNCTUATION;
	else
		rtn = UNKNOWN;
	return rtn;
}

