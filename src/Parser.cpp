#include "../include/Parser.hpp"

ParseNode* Parser::skimClass(const Token* tokens, uint64_t n) {
    if (n < 4) {
        return nullptr;
    }
    if (tokens[0].type != KEYWORD_CLASS || tokens[1].type != IDENTIFIER || tokens[2].type != OPEN_CURLY_BRACE) {
        return nullptr;
    }
    
    // TODO: inheritance
    uint64_t braceDepth = 1;
    uint64_t i = 2;
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

    return new ParseNode(tokens, i, classImplementation);
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
    return new ParseNode(tokens, i, functionImplementation);
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

    std::cout << rootNode->number_of_children() << std::endl;

    return rootNode;
}