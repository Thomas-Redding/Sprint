//
//  Grammar.hpp
//  Sprint
//
//  Created by Morgan Redding on 11/23/15.
//  Copyright © 2015 Thomas Redding. All rights reserved.
//

#ifndef Grammar_hpp
#define Grammar_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Word.hpp"
#include "Node.hpp"

struct Rule {
    Rule() {};
    Rule(WordType w, std::initializer_list<Word> comp) : components(comp.size()) {
        for (size_t i = 0; i < comp.size(); i++) {
            components[i] = comp.begin()[i];
        }
    }
    WordType word;
    std::vector<Word> components;
};

struct Grammar {
    void addRule(WordType w, std::initializer_list<Word> components) {
        rules[w].push_back(Rule(w, components));
    }
    std::unordered_map< int, std::vector< Rule > > rules;
};

#endif /* Grammar_hpp */
