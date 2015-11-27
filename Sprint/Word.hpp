//
//  Word.hpp
//  Sprint
//
//  Created by Morgan Redding on 11/23/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Word_h
#define Word_h

#include "Token.hpp"

enum WordType {
    
    program,
    pseudo_chunck,
    
    class_declaration,
    function_declaration,
    variable_declaration,
    
    template_parameters,
    template_parameter,
    
    pseudo_line, // can be either a line or a block
    real_line,
    block,
    
    
    etc,     // continue as long as you keep finding the thing after this
    etc_not, // continue UNTIL you find the thing after this
    NULL_WORD_TYPE,
};

struct Word {
    Word() :              _wordType(NULL_WORD_TYPE), _tokenType(NULL_TOKEN_TYPE), _text("") {};
    Word(WordType w) :    _wordType(w),              _tokenType(NULL_TOKEN_TYPE), _text("") {};
    Word(TokenType t) :   _wordType(NULL_WORD_TYPE), _tokenType(t),               _text("") {};
    Word(std::string t) : _wordType(NULL_WORD_TYPE), _tokenType(NULL_TOKEN_TYPE), _text(t)  {};
    bool operator=(const Word& w) const {
        return (_wordType == w._wordType) && (_tokenType == w._tokenType) && (_text == w._text);
    }
    WordType wordType() const {
        return _wordType;
    }
    TokenType tokenType() const {
        return _tokenType;
    }
    std::string text() const {
        return _text;
    }
private:
    WordType _wordType;
    TokenType _tokenType;
    std::string _text;
};

#endif /* Word_h */
