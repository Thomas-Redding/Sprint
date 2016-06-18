#include "../include/Parser.hpp"
#include <stack>

// get the parse tree of an array of tokens
ParseNode* Parser::getParseTree(const Token* tokens, uint64_t n) {
    ParseNode* rootNode = new ParseNode(tokens, n, root);

    std::unordered_map<const Token*, const Token*> braces = findBraces(tokens, n);

    // find all classes and global functions
    uint64_t i = 0;
    while (i < n) {
        ParseNode* node;
        if ((node = parseClass(tokens + i, n - i, braces)) != nullptr) {
            rootNode->addChild(node);
            i += node->tokenLength;
            continue;
        }
        else if ((node = parseFunction(tokens + i, n - i, braces)) != nullptr) {
            rootNode->addChild(node);
            i += node->tokenLength;
            continue;
        }
        else if (false) {
            // TODO: check for global variables
            i++;
            continue;
        }
        else {
            throw std::runtime_error("Compiler Error on line " + std::to_string(tokens[i].lineNum) + ".\nExpected the start of a global class, global variable, or global function, but found '" + tokens[i].str + "'");
        }
    }

    return rootNode;
}

// TODO: pointers
// TODO: multiple variables declared in one statement
ParseNode* Parser::parseMemberVariable(const Token* tokens, uint64_t n, const std::unordered_map<const Token*, const Token*>& braces) {
    uint64_t i = 0;
    assert(n > 3);
    while (i < n && tokens[i].type == KEYWORD_STATIC) {
        ++i;
    }
    assert(n > 3 + i);

    // check if variable is templated
    if (tokens[i + 1].type != LESS_THAN) {
        if ((tokens[i].type == IDENTIFIER
            || tokens[i].type == KEYWORD_INT
            || tokens[i].type == KEYWORD_INT8
            || tokens[i].type == KEYWORD_INT16
            || tokens[i].type == KEYWORD_INT32
            || tokens[i].type == KEYWORD_INT64
            || tokens[i].type == KEYWORD_UINT
            || tokens[i].type == KEYWORD_UINT8
            || tokens[i].type == KEYWORD_UINT16
            || tokens[i].type == KEYWORD_UINT32
            || tokens[i].type == KEYWORD_UINT64
            )) {
            const uint64_t startingLine = tokens[i].lineNum;
            while (++i < n && tokens[i].type != SEMI_COLON && (tokens[i].type == IDENTIFIER || tokens[i].type == COMMA)) {}
            if (i == n || tokens[i].type != SEMI_COLON) {
                throw std::runtime_error("Error: no semicolon after variable declaration on line " + std::to_string(startingLine) + "\n");
            }
            ParseNode* returnNode = new ParseNode(tokens, i + 1, variable_declaration);
            return returnNode;
        }
        else {
            return nullptr;
        }
    }

    const uint64_t templateStart = tokens[i].lineNum;
    ++i;
    uint64_t templateDepth = 1;
    while (++i < n && templateDepth > 0) {
        if (tokens[i].type == LESS_THAN) {
            ++templateDepth;
        }
        else if (tokens[i].type == GREATER_THAN) {
            --templateDepth;
        }
    }
    if (i == n && templateDepth != 0) {
        throw std::runtime_error("Error: variable template on line " + std::to_string(templateStart) + " is never closed\n");
    }

    while (++i < n && tokens[i].type != SEMI_COLON) {}

    if (i == n || tokens[i].type != SEMI_COLON) {
        throw std::runtime_error("Error: no semicolon after variable declaration on line " + std::to_string(templateStart) + "\n");
    }

    return new ParseNode(tokens, i + 1, variable_declaration);
}

// returns parse tree of a class if the tokens form a valid class
// otherwise returns nullptr
// valid class looks like "class Foo implements you {...}" or "class Foo extends you {...}"
ParseNode* Parser::parseClass(const Token* tokens, uint64_t n, const std::unordered_map<const Token*, const Token*>& braces) {

    if (n < 4) {
        return nullptr;
    }
    if ((tokens[0].type != KEYWORD_CLASS && tokens[0].type != KEYWORD_STRUCT) || tokens[1].type != IDENTIFIER) {
        return nullptr;
    }

    std::cout << "SKIMMING CLASS @ " << Token::toString(tokens[0]) << " " << Token::toString(tokens[1]) << " " << Token::toString(tokens[2]) << std::endl;

    // find first brace
    uint64_t firstBrace = 1;
    while (++firstBrace < n && tokens[firstBrace].type != OPEN_CURLY_BRACE) {};

    if (firstBrace == n) {
        throw std::runtime_error("Compiler Error: struct declaration does not have an open curly brace");
    }

    // skim through and search for the end of the class
    uint64_t braceDepth = 1;
    uint64_t i = firstBrace;
    while (++i < n && braceDepth > 0) {
        if (tokens[i].type == OPEN_CURLY_BRACE) {
            ++braceDepth;
        }
        else if (tokens[i].type == CLOSE_CURLY_BRACE) {
            --braceDepth;
        }
    }

    if (i == n && braceDepth != 0) {
        throw std::runtime_error("Compiler Error: curly brace from line " + std::to_string(tokens[2].lineNum) + " is never closed\n");
    }

    const uint64_t len = i;
    ParseNode* rtn = new ParseNode(tokens, len, class_implementation);

    // go through the class and further break it down into methods/variables
    i = firstBrace + 1;
    while (i < len - 1) {
        ParseNode* node = parseMemberVariable(tokens + i, len - i, braces);
        if (node != nullptr) {
            rtn->addChild(node);
            i += node->tokenLength;
            continue;
        }
        delete node;

        node = parseFunction(tokens + i, len - i, braces);
        if (node != nullptr) {
            rtn->addChild(node);
            i += node->tokenLength;
            continue;
        }
        delete node;

        if (tokens[i + 1].type == COLON && (tokens[i].type == KEYWORD_PUBLIC || tokens[i].type == KEYWORD_PROTECTED || tokens[i].type == KEYWORD_PRIVATE)) {
            if (tokens[i].type == KEYWORD_PUBLIC) {
                rtn->addChild(new ParseNode(tokens + i, 2, public_colon));
            }
            else if (tokens[i].type == KEYWORD_PROTECTED) {
                rtn->addChild(new ParseNode(tokens + i, 2, protected_colon));
            }
            else {
                rtn->addChild(new ParseNode(tokens + i, 2, private_colon));
            }
            i += 2;
            continue;
        }


        throw std::runtime_error("Token on line " + std::to_string(tokens[i].lineNum) + " is not part of a member variable or method");
    }

    std::cout << "DONE SKIMMING CLASS\n\n";

    return rtn;
}

// returns parse tree of a function if the tokens form a valid function
// otherwise returns nullptr
// valid function looks like "foo<T>(List<T> A, int x) -> T {}"
ParseNode* Parser::parseFunction(const Token* tokens, uint64_t n, const std::unordered_map<const Token*, const Token*>& braces) {
    uint64_t i = _isValidFunctionName(tokens, n);
    if (i == 0) {
        return nullptr;
    }
    while (i < n && tokens[i].type != OPEN_CURLY_BRACE) { ++i; }
    uint64_t braceDepth = 1;
    while (++i < n && braceDepth > 0) {
        if (tokens[i].type == OPEN_CURLY_BRACE) {
            ++braceDepth;
        }
        else if (tokens[i].type == CLOSE_CURLY_BRACE) {
            --braceDepth;
        }
    }
    if (i == n && braceDepth != 0) {
        throw std::runtime_error("Compiler Error: curly brace from line " + std::to_string(tokens[2].lineNum) + " is never closed\n");
    }
    return new ParseNode(tokens, i, function_implementation);
}

// returns how many tokens form a valid function name
// "0" indicates it is an invalid name
uint64_t Parser::_isValidFunctionName(const Token* tokens, uint64_t n) {
    assert(n != 0);
    if (tokens[0].type == IDENTIFIER) {
        return 1;
    }
    // TODO: \= (BACKSLASH_EQUALS doesn't exist yet)
    if (   tokens[0].type == EQUALS
        || tokens[0].type == PLUS
        || tokens[0].type == MINUS
        || tokens[0].type == ASTERISK
        || tokens[0].type == SLASH
        || tokens[0].type == BACK_SLASH
        || tokens[0].type == CARROT
        || tokens[0].type == PLUS_EQUALS
        || tokens[0].type == MINUS_EQUALS
        || tokens[0].type == ASTERISK_EQUALS
        || tokens[0].type == SLASH_EQUALS
        || tokens[0].type == CARROT_EQUALS
        || tokens[0].type == LESS_THAN
        || tokens[0].type == GREATER_THAN
        || tokens[0].type == EQUAL_EQUALS
        || tokens[0].type == EXCLAMATION_POINT_EQUALS
        || tokens[0].type == LESS_THAN_EQUALS
        || tokens[0].type == GREATER_THAN_EQUALS
        || tokens[0].type == VERTICAL_BAR
        || tokens[0].type == AMPERSAND
        || tokens[0].type == SHIFT_LEFT
        || tokens[0].type == SHIFT_RIGHT
        || tokens[0].type == SHIFT_LEFT_EQUALS
        || tokens[0].type == SHIFT_RIGHT_EQUALS
        || tokens[0].type == TILDE
        ) {
        return 1;
    }
    if (n == 1) {
        return 0;
    }
    if (   (tokens[0].type == OPEN_BRACKET && tokens[1].type == CLOSE_BRACKET)
        || (tokens[0].type == OPEN_PARENTHESIS && tokens[1].type == CLOSE_PARENTHESIS)
        || (tokens[0].type == OPEN_CURLY_BRACE && tokens[1].type == CLOSE_CURLY_BRACE)
        ) {
        return 2;
    }
    return 0;
}

std::unordered_map<const Token*, const Token*> Parser::findBraces(const Token* tokens, const uint64_t n) {
    
    uint64_t count = 0;
    for (uint64_t i = 0; i < n; ++i) {
        if (tokens[i].type == OPEN_CURLY_BRACE || tokens[i].type == OPEN_PARENTHESIS || tokens[i].type == OPEN_BRACKET) {
            count += 2;
        }
    }

    std::stack<uint64_t> unclosed;

    std::unordered_map<const Token*, const Token*> braces;
    uint64_t i = -1;
    while (++i < n && tokens[i].type != OPEN_PARENTHESIS && tokens[i].type != OPEN_BRACKET && tokens[i].type != OPEN_CURLY_BRACE) {}
    unclosed.push(i);

    while (++i < n) {
        if (tokens[i].type == OPEN_CURLY_BRACE || tokens[i].type == OPEN_PARENTHESIS || tokens[i].type == OPEN_BRACKET) {
            unclosed.push(i);
        }
        else if (tokens[i].type == CLOSE_CURLY_BRACE || tokens[i].type == CLOSE_PARENTHESIS || tokens[i].type == CLOSE_BRACKET) {
            uint64_t x = unclosed.top();
            unclosed.pop();
            if (tokens[i].type == CLOSE_CURLY_BRACE) {
                if (tokens[x].type != OPEN_CURLY_BRACE) {
                    throw std::runtime_error("Error: open curly brace on line " + std::to_string(tokens[x].lineNum) + ", character " + std::to_string(tokens[x].charNum) + " is not closed.");
                }
                braces[tokens + x] = tokens + i;
                braces[tokens + i] = tokens + x;
            }
            else if (tokens[i].type == CLOSE_PARENTHESIS) {
                if (tokens[x].type != OPEN_PARENTHESIS) {
                    throw std::runtime_error("Error: open parenthesis on line " + std::to_string(tokens[x].lineNum) + ", character " + std::to_string(tokens[x].charNum) + " is not closed.");
                }
                braces[tokens + x] = tokens + i;
                braces[tokens + i] = tokens + x;
            }
            else if (tokens[i].type == CLOSE_BRACKET) {
                if (tokens[x].type != OPEN_BRACKET) {
                    throw std::runtime_error("Error: open bracket on line " + std::to_string(tokens[x].lineNum) + ", character " + std::to_string(tokens[x].charNum) + " is not closed.");
                }
                braces[tokens + x] = tokens + i;
                braces[tokens + i] = tokens + x;
            }
            else {
                std::cout << Token::toString(tokens[i].type) << std::endl;
                throw std::runtime_error("????");
            }
        }
    }
    return braces;
}