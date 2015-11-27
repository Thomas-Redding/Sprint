//
//  Parser.cpp
//  Sprint
//
//  Created by Thomas Redding on 10/22/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#include "Parser.hpp"
#include "Node.hpp"

Parser::Parser() {
    
    grammar.addRule(WordType::pseudo_chunck, {
        Word(WordType::function_declaration)
    });
    grammar.addRule(WordType::pseudo_chunck, {
        Word(WordType::class_declaration)
    });
    grammar.addRule(WordType::pseudo_chunck, {
        Word(WordType::variable_declaration)
    });
    
    // block
    grammar.addRule(WordType::pseudo_line, { Word(real_line) });
    grammar.addRule(WordType::pseudo_line, { Word(block) });
    grammar.addRule(WordType::block, {
        Word(TokenType::INDENT),
        
        Word(WordType::etc_not),
        Word(WordType::block),
        
        Word(TokenType::DEDENT),
    });
    
    // templating rules
    grammar.addRule(template_parameters, {
        Word(WordType::etc),
        Word(WordType::template_parameter)
    });
    
    grammar.addRule(template_parameter, {
        Word(TokenType::IDENTIFIER),
        Word(TokenType::IDENTIFIER)
    });
    
    // class rules
    grammar.addRule(class_declaration, {
        Word("class"),
        Word(TokenType::IDENTIFIER),
        Word(TokenType::INDENT),
        // continue until you find a 'dedent'
        Word(TokenType::DEDENT),
        Word(WordType::etc_not),
    });


}

Node* Parser::parse(const std::vector<Token>& tokens) {
    return parse(&tokens[0], tokens.size(), Rule(WordType::program, {Word(pseudo_chunck), Word(WordType::etc)}));
}

// returns the rule and the length of tokens used to satisfy it
std::pair<Rule, size_t> Parser::findGrammarMatch(const Token* tokens, const size_t n) const {
    for (auto it = grammar.rules.begin(); it != grammar.rules.end(); ++it) {
        std::pair<Rule, size_t> x = findGrammarMatch(tokens, n, WordType(it->first));
        if (x.second != 0) {
            return x;
        }
    }
    throw std::runtime_error("No rule matched to grammar");
    return std::pair<Rule, size_t>(Rule(), 0);
}

// returns the rule and the length of tokens used to satisfy it
std::pair<Rule, size_t> Parser::findGrammarMatch(const Token* tokens, const size_t n, WordType output) const {
    auto it = grammar.rules.find(output);
    if (it == grammar.rules.end()) {
        throw std::runtime_error("No rule matched to grammar");
    }
    const std::vector< Rule >& rules = it->second;
    for (size_t i = 0; i < rules.size(); ++i) {
        // TODO
    }
    throw std::runtime_error("No rule matched to grammar");
    return std::pair<Rule, size_t>(Rule(), 0);
}

Node* Parser::parse(const Token* tokens, const size_t n, const Rule& grammarRule) {
    Node* rtn = new Node();
    size_t i = 0;
    for (auto it = grammarRule.components.begin(); it != grammarRule.components.end(); ++it) {
        const WordType& word = it->wordType();
        if (word == WordType::NULL_WORD_TYPE) {
            throw std::runtime_error("Tried to parse a terminal node");
        }
        std::pair<Rule, size_t> x = findGrammarMatch(&tokens[i], n - i, word);
        rtn->add_child(parse(&tokens[i], n - i, x.first));
        i += x.second;
    }
    return rtn;
}

void Parser::exit(std::string message) {
	std::cout << message << "\n";
	_exit(1);
}
