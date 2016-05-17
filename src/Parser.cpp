#include "../include/Parser.hpp"

ParseNode* Parser::skimClassVariable(const Token* tokens, uint64_t n) {
    uint64_t i = 0;
    assert(n > 3);
    while (i < n && (tokens[i].type == KEYWORD_STATIC || tokens[i].type == KEYWORD_PUBLIC || tokens[i].type == KEYWORD_PROTECTED || tokens[i].type == KEYWORD_PRIVATE)) {
        ++i;
    }
    assert(n > 3 + i);

    // check if variable is templated
    if (tokens[i + 1].type != LESS_THAN) {
        if ((tokens[i].type == IDENTIFIER || tokens[i].type == KEYWORD_INT32 || tokens[i].type == KEYWORD_INT16)) {
            return new ParseNode(tokens, i + 2, variable_declaration);
        }
        else {
            throw std::runtime_error("Error on line " + std::to_string(tokens[i + 1].lineNum) + "\n");
        }
    }

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
        throw std::runtime_error("Error on line " + std::to_string(tokens[n - 1].lineNum) + "\n");
    }
    return new ParseNode(tokens, i, variable_declaration);
}

ParseNode* Parser::skimClass(const Token* tokens, uint64_t n) {

    if (n < 4) {
        return nullptr;
    }
    if ((tokens[0].type != KEYWORD_CLASS && tokens[0].type != KEYWORD_STRUCT) || tokens[1].type != IDENTIFIER) {
        return nullptr;
    }

    // find first brace
    uint64_t firstBrace = 1;
    while (tokens[++firstBrace].type != OPEN_CURLY_BRACE) {};

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

    // go through the class and further break it down into member/static methods/variables
    i = firstBrace;
    while (++i < len - 1) {
        ParseNode* node = skimFunction(tokens + i, len - i);
        if (node != nullptr) {
            rtn->addChild(node);
            i += node->tokenLength;
            continue;
        }
        delete node;

        // if it is not a function, it may be a variable
        node = skimClassVariable(tokens + i, len - i);
        if (node != nullptr) {
            rtn->addChild(node);
            i += node->tokenLength;
            continue;
        }
        delete node;

        throw std::runtime_error("Token on line " + std::to_string(tokens[i].lineNum) + " is not part of a member variable or method");
    }

    return rtn;
}

// returns how many tokens form a valid function name
// "0" indicates it is an invalid name
uint64_t Parser::isValidFunctionName(const Token* tokens, uint64_t n) {
    assert(n != 0);
    if (tokens[0].type == IDENTIFIER) {
        return 1;
    }
    // TODO: ~
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

ParseNode* Parser::skimFunction(const Token* tokens, uint64_t n) {
    uint64_t i = isValidFunctionName(tokens, n);
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

ParseNode* Parser::getParseTree(const Token* tokens, uint64_t n) {
    ParseNode* rootNode = new ParseNode(tokens, n, root);

    // find all classes and global functions
    uint64_t i = 0;
    while (i < n) {
        ParseNode* node;
        if ((node = skimClass(tokens + i, n - i)) != nullptr) {
            rootNode->addChild(node);
            i += node->tokenLength;
            continue;
        }
        else if ((node = skimFunction(tokens + i, n - i)) != nullptr) {
            rootNode->addChild(node);
            i += node->tokenLength;
            continue;
        }
        else {
            throw std::runtime_error("Compiler Error on line " + std::to_string(tokens[i].lineNum) + "\n");
        }
    }

    return rootNode;
}