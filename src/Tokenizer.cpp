#include "../include/Tokenizer.hpp"

/*** PUBLIC ***/

Tokenizer::Tokenizer() {
	keywords.insert("break");
	keywords.insert("case");
	keywords.insert("catch");
	keywords.insert("class");
	keywords.insert("const");
	keywords.insert("continue");
	keywords.insert("do");
	keywords.insert("else");
	keywords.insert("enum");
	keywords.insert("false");
	keywords.insert("for");
	keywords.insert("if");
	keywords.insert("in");
	keywords.insert("mut");
	keywords.insert("NULL");
	keywords.insert("private");
	keywords.insert("public");
	keywords.insert("return");
	keywords.insert("sizeof");
	keywords.insert("static");
	keywords.insert("switch");
	keywords.insert("throw");
	keywords.insert("true");
	keywords.insert("try");
	keywords.insert("while");
	keywords.insert("uint");
	keywords.insert("uint8");
	keywords.insert("uint16");
	keywords.insert("uint32");
	keywords.insert("int");
	keywords.insert("int8");
	keywords.insert("int16");
	keywords.insert("int32");
	keywords.insert("import");
	keywords.insert("and");
	keywords.insert("or");
	keywords.insert("not");
	keywords.insert("xor");
	keywords.insert("float");
	keywords.insert("double");
	keywords.insert("void");
	keywords.insert("bool");
	keywords.insert("char");
	keywords.insert("repeat");
	keywords.insert("default");
	keywords.insert("is");
	keywords.insert("Infinity");
	keywords.insert("NaN");
	keywords.insert("commute");
    keywords.insert("union");
    keywords.insert("virtual");
    keywords.insert("protected");

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
	punctuation.insert('\\');
	punctuation.insert('@');
	punctuation.insert('~');

	brackets.insert('{');
	brackets.insert('(');
	brackets.insert('[');
	brackets.insert(']');
	brackets.insert(')');
	brackets.insert('}');
}

std::list<Token> Tokenizer::process(std::string str) {
	// reset member variables to prepare for parsing
	rtn.clear();
	it = 0;
	bool isInMultiLineComment = false;
	bool isInSingleLineComment = false;

	// split by line to figure out how getLineNum() and getCharNum() will determine which character is on which line
	std::vector<std::string> byLine = split(str, '\n');
	cumulativeCharsPerLine.clear();
	for (long i=0; i<byLine.size(); i++)
		cumulativeCharsPerLine.push_back(byLine[i].size());
	for (long i=1; i<byLine.size(); i++)
		cumulativeCharsPerLine[i] = cumulativeCharsPerLine[i] + cumulativeCharsPerLine[i-1] + 1; // +1 is for the \n character

	// set the current state of the Finite State Machine to the default
	Token cur = Token(UNKNOWN, "", getLineNum(), getCharNum());

	// run the Finite State Machine
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
			if (str[it] == '\"') {
				cur.type = STRING_LITERAL;
			}
			else if (str[it] == '\'') {
				cur.type = CHARACTER_LITERAL;
			}
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
				/*if (it == 0) {
					error("Tokenizer: Can't begin a document with '+' or '-'.");
				}*/
				cur.str += str[it];
				long i;
				for (i=it+1; i<str.length(); i++) {
					if (str[i] != ' ') {
						break;
					}
				}

				if (isdigit(str[i])) {
					Token last = rtn.back();
					if (isPunc(last.type) || last.type == OPEN_BRACKET || last.type == OPEN_CURLY_BRACE || last.type == OPEN_PARENTHESIS)
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
			else {
				cur.lineNum = getLineNum();
				cur.charNum = getCharNum();
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
		else if (cur.type == CHARACTER_LITERAL) {
			if (str[it] == '\'') {
				if(str.length() == 0)
					error("Tokenizer Error: character literal is Empty (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
				else if (str.length() == 1)
					resetAndSave(cur);
				else if (str.length() > 2)
					error("Tokenizer Error: character literal contains multiple characters (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
				else {
					if (str[0] == '\\')
						resetAndSave(cur);
					else
						error("Tokenizer Error: character literal contains multiple characters (line: " + std::to_string(getLineNum()) + ", char: " + std::to_string(getCharNum()) + ")");
				}
			}
			else
				cur.str += str[it];
		}
		else if (cur.type == PUNCTUATION) {
			if (isPunc(str[it]))
				cur.str += str[it];
			else {
				int i;
				int go_back;
				for (i = 0; i < cur.str.length(); ++i) {
					go_back = cur.str.length() - i + 1;
					if (cur.str.length() - i >= 3) {
						TokenType tt = categorizePunc(cur.str.substr(i, 3));
						if (tt != UNKNOWN) {
							rtn.push_back(Token(tt, cur.str.substr(i, 3), getLineNum(go_back), getCharNum(go_back)));
							++i;
							continue;
						}
					}
					if (cur.str.length() - i >= 2) {
						TokenType tt = categorizePunc(cur.str.substr(i, 2));
						if (tt != UNKNOWN) {
							rtn.push_back(Token(tt, cur.str.substr(i, 2), getLineNum(go_back), getCharNum(go_back)));
							++i;
							continue;
						}
					}
					TokenType tt = categorizePunc(cur.str.substr(i, 1));
					if (tt != UNKNOWN) {
						if (tt == PLUS) {
							if (rtn.back().type == IDENTIFIER || rtn.back().type == CLOSE_PARENTHESIS || rtn.back().type == CLOSE_BRACKET)
								rtn.push_back(Token(tt, cur.str.substr(i, 1), getLineNum(go_back), getCharNum(go_back)));
							else
								rtn.push_back(Token(POSITIVE, cur.str.substr(i, 1), getLineNum(go_back), getCharNum(go_back)));
						}
						else if (tt == MINUS) {
							if (rtn.back().type == IDENTIFIER || rtn.back().type == CLOSE_PARENTHESIS || rtn.back().type == CLOSE_BRACKET)
								rtn.push_back(Token(tt, cur.str.substr(i, 1), getLineNum(go_back), getCharNum(go_back)));
							else
								rtn.push_back(Token(NEGATIVE, cur.str.substr(i, 1), getLineNum(go_back), getCharNum(go_back)));
						}
						else
							rtn.push_back(Token(tt, cur.str.substr(i, 1), getLineNum(go_back), getCharNum(go_back)));
						continue;
					}
				}
				cur.type = UNKNOWN;
				cur.str = "";
				cur.lineNum = getLineNum();
				cur.charNum = getCharNum();
				--it;
			}
		}
		else {
			error("Tokenizer: Error 1 (it: " + std::to_string(it) + ")");
		}
	}

	std::stack<std::list<Token>::iterator> template_symbols;
	std::set<TokenType> legalTokensForTemplating = {KEYWORD_INT, KEYWORD_INT8, KEYWORD_INT16, KEYWORD_INT32, KEYWORD_UINT, KEYWORD_UINT8, KEYWORD_UINT16, KEYWORD_UINT32, COMMA, OPEN_BRACKET, CLOSE_BRACKET, IDENTIFIER};

	for (std::list<Token>::iterator it = rtn.begin(); it != rtn.end(); ++it) {
		if ((*it).type == LESS_THAN)
			template_symbols.push(it);
		else if ((*it).type == GREATER_THAN) {
			if (template_symbols.size() > 0) {
				template_symbols.top()->type = OPEN_TEMPLATE;
				(*it).type = CLOSE_TEMPLATE;
				template_symbols.pop();
			}
		}
		else if ((*it).type == SHIFT_RIGHT) {
			if (template_symbols.size() > 1) {
				template_symbols.top()->type = OPEN_TEMPLATE;
				template_symbols.pop();
				template_symbols.top()->type = OPEN_TEMPLATE;
				template_symbols.pop();
				(*it).type = CLOSE_TEMPLATE;
				(*it).str = ">";
				rtn.insert(it, *it);
			}
			else if (template_symbols.size() > 0) {
				template_symbols.top()->type = OPEN_TEMPLATE;
				(*it).type = CLOSE_TEMPLATE;
				template_symbols.pop();
				(*it).str = ">";
				rtn.insert(it, *it);
				(*it).type = CLOSE_TEMPLATE;
				++it;
				(*it).type = LESS_THAN;
			}
		}
		else if (template_symbols.empty()) {
			if (legalTokensForTemplating.count((*it).type) == 0)
				while (! template_symbols.empty())
					template_symbols.pop();
		}
	}

	return rtn;
}

/*** PRIVATE ***/

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
		if (it < cumulativeCharsPerLine[i])
			return i;
	}
	return cumulativeCharsPerLine.size() - 1;
}

long Tokenizer::getCharNum() {
	int lineNum = getLineNum();
	if (lineNum == 0)
		return it;
	else
		return it - cumulativeCharsPerLine[lineNum-1];
}

long Tokenizer::getLineNum(int go_back) {
	for (long i=0; i<cumulativeCharsPerLine.size(); ++i) {
		if ((it - go_back) < cumulativeCharsPerLine[i])
			return i;
	}
	return cumulativeCharsPerLine.size() - 1;
}


long Tokenizer::getCharNum(int go_back) {
	int lineNum = getLineNum(go_back);
	if (lineNum == 0)
		return (it - go_back);
	else
		return (it - go_back) - cumulativeCharsPerLine[lineNum-1];
}

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
	else if (str == "\\")
		rtn = BACK_SLASH;
	else if (str == "@")
		rtn = AT;
	else if (str == "~")
		rtn = TILDE;
	else if (str == "+=")
		rtn = PLUS_EQUALS;
	else if (str == "-=")
		rtn = MINUS_EQUALS;
	else if (str == "*=")
		rtn = ASTERISK_EQUALS;
	else if (str == "/=")
		rtn = SLASH_EQUALS;
	else if (str == "%=")
		rtn = PERCENT_EQUALS;
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
	else if (str == "===")
		rtn = EQUAL_EQUAL_EQUALS;
	else if (str == "!==")
		rtn = EXCLAMATION_POINT_EQUAL_EQUALS;
	else if (str == "++")
		rtn = PLUS_PLUS;
	else if (str == "--")
		rtn = MINUS_MINUS;
	else if (str == "->")
		rtn = ARROW;
	else if (str == "<-")
		rtn = LEFT_ARROW;
	else if (str == "<->")
		rtn = LEFT_RIGHT_ARROW;
	else if (str == "**=")
		rtn = ASTERISK_ASTERISK_EQUALS;
	else if (str == "**")
		rtn = ASTERISK_ASTERISK;
	else if (str == "\\=")
		rtn = BACK_SLASH_EQUALS;
	else
		rtn = UNKNOWN;
	return rtn;
}


void Tokenizer::categorizeIdentifier(Token &cur) {
	if(cur.str == "and")
		cur.type = KEYWORD_AND;
	else if(cur.str == "break")
		cur.type = KEYWORD_BREAK;
	else if(cur.str == "bool")
		cur.type = KEYWORD_BOOL;
	else if(cur.str == "case")
		cur.type = KEYWORD_CASE;
	else if(cur.str == "catch")
		cur.type = KEYWORD_CATCH;
	else if(cur.str == "char")
		cur.type = KEYWORD_CHAR;
	else if(cur.str == "class")
		cur.type = KEYWORD_CLASS;
	else if (cur.str == "commute")
		cur.type = KEYWORD_COMMUTE;
	else if(cur.str == "const")
		cur.type = KEYWORD_CONST;
	else if(cur.str == "continue")
		cur.type = KEYWORD_CONTINUE;
	else if(cur.str == "default")
		cur.type = KEYWORD_DEFAULT;
	else if(cur.str == "do")
		cur.type = KEYWORD_DO;
	else if(cur.str == "double")
		cur.type = KEYWORD_DOUBLE;
	else if(cur.str == "else")
		cur.type = KEYWORD_ELSE;
	else if(cur.str == "enum")
		cur.type = KEYWORD_ENUM;
	else if(cur.str == "false")
		cur.type = KEYWORD_FALSE;
	else if(cur.str == "float")
		cur.type = KEYWORD_FLOAT;
	else if(cur.str == "for")
		cur.type = KEYWORD_FOR;
	else if(cur.str == "if")
		cur.type = KEYWORD_IF;
	else if(cur.str == "import")
		cur.type = KEYWORD_IMPORT;
	else if(cur.str == "in")
		cur.type = KEYWORD_IN;
	else if (cur.str == "Infinity")
		cur.type = KEYWORD_INFINITY;
	else if(cur.str == "int")
		cur.type = KEYWORD_INT;
	else if(cur.str == "int8")
		cur.type = KEYWORD_INT8;
	else if(cur.str == "int16")
		cur.type = KEYWORD_INT16;
	else if(cur.str == "int32")
		cur.type = KEYWORD_INT32;
	else if (cur.str == "is")
		cur.type = KEYWORD_IS;
	else if (cur.str == "mut")
		cur.type = KEYWORD_MUT;
	else if (cur.str == "NaN")
		cur.type = KEYWORD_NAN;
	else if(cur.str == "not")
		cur.type = KEYWORD_NOT;
	else if(cur.str == "NULL")
		cur.type = KEYWORD_NULL;
	else if(cur.str == "or")
		cur.type = KEYWORD_OR;
	else if(cur.str == "private")
		cur.type = KEYWORD_PRIVATE;
	else if(cur.str == "public")
		cur.type = KEYWORD_PUBLIC;
    else if (cur.str == "protected")
        cur.type = KEYWORD_PROTECTED;
	else if(cur.str == "repeat")
		cur.type = KEYWORD_REPEAT;
	else if(cur.str == "return")
		cur.type = KEYWORD_RETURN;
	else if(cur.str == "sizeof")
		cur.type = KEYWORD_SIZEOF;
	else if(cur.str == "static")
		cur.type = KEYWORD_STATIC;
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
	else if(cur.str == "uint")
		cur.type = KEYWORD_UINT;
	else if(cur.str == "uint8")
		cur.type = KEYWORD_UINT8;
	else if(cur.str == "uint16")
		cur.type = KEYWORD_UINT16;
	else if(cur.str == "uint32")
		cur.type = KEYWORD_UINT32;
    else if(cur.str == "union")
        cur.type = KEYWORD_UNION;
    else if(cur.str == "virtual")
        cur.type = KEYWORD_VIRTUAL;
	else if(cur.str == "void")
		cur.type = KEYWORD_VOID;
	else if(cur.str == "while")
		cur.type = KEYWORD_WHILE;
	else if(cur.str == "xor")
		cur.type = KEYWORD_XOR;
	else
		cur.type = IDENTIFIER;
}

const bool Tokenizer::isPunc(TokenType t) {
	return t == PUNCTUATION || t == PERIOD || t == COLON || t == SEMI_COLON || t == PLUS || t == MINUS || t == ASTERISK || t == SLASH || t == AMPERSAND || t == POUND_SIGN || t == LESS_THAN || t == EQUALS || t == GREATER_THAN || t == COMMA || t == VERTICAL_BAR || t == PERCENT || t == EXCLAMATION_POINT || t == CARROT || t == QUESTION_MARK || t == BACK_SLASH || t == AT || t == PLUS_EQUALS || t == MINUS_EQUALS || t == SLASH_EQUALS || t == ASTERISK_EQUALS || t == AMPERSAND_EQUALS || t == CARROT_EQUALS || t == VERTICAL_BAR_EQUALS || t == PLUS_PLUS || t == MINUS_MINUS || t == SHIFT_LEFT || t == SHIFT_RIGHT || t == GREATER_THAN_EQUALS || t == LESS_THAN_EQUALS || t == SHIFT_LEFT_EQUALS || t == SHIFT_RIGHT_EQUALS || t == EXCLAMATION_POINT_EQUALS || t == EQUAL_EQUALS || t == LEFT_RIGHT_ARROW || t == LEFT_ARROW;
}

const bool Tokenizer::isKeyWord(TokenType t) {
	return t == KEYWORD_AND || t == KEYWORD_BOOL || t == KEYWORD_BREAK || t == KEYWORD_CASE || t == KEYWORD_CATCH || t == KEYWORD_CHAR || t == KEYWORD_CLASS || t == KEYWORD_COMMUTE || t == KEYWORD_CONST || t == KEYWORD_CONTINUE || t == KEYWORD_DEFAULT || t == KEYWORD_DO || t == KEYWORD_DOUBLE || t == KEYWORD_ELSE || t == KEYWORD_ENUM || t == KEYWORD_FALSE || t == KEYWORD_FLOAT || t == KEYWORD_FOR || t == KEYWORD_IF || t == KEYWORD_IMPORT || t == KEYWORD_IN || t == KEYWORD_INFINITY || t == KEYWORD_INT || t == KEYWORD_INT16 || t == KEYWORD_INT32 || t == KEYWORD_INT8 || t == KEYWORD_IS || t == KEYWORD_MUT || t == KEYWORD_NAN || t == KEYWORD_NOT || t == KEYWORD_NULL || t == KEYWORD_OR || t == KEYWORD_PRIVATE || t == KEYWORD_PUBLIC || t == KEYWORD_PROTECTED || t == KEYWORD_REPEAT || t == KEYWORD_RETURN || t == KEYWORD_SIZEOF || t == KEYWORD_STATIC || t == KEYWORD_SWITCH || t == KEYWORD_THIS || t == KEYWORD_THROW || t == KEYWORD_TRUE || t == KEYWORD_TRY || t == KEYWORD_UINT || t == KEYWORD_UINT16 || t == KEYWORD_UINT32 || t == KEYWORD_UINT8 || t == KEYWORD_UNION || t == KEYWORD_VIRTUAL || t == KEYWORD_VOID || t == KEYWORD_WHILE || t == KEYWORD_XOR;
}

const std::string Tokenizer::tokenToString2(const Token& t) {
	if (t.str == "")
		return "(" + Token::toString(t.type) + ")";
	else
		return "(" + Token::toString(t.type) + " '" + t.str + "')";
}